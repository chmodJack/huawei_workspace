/*
 * cache.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: uzleo
 */





// tries to model cache hierarchy in ARM A-series MPCOre chips
// strictly inclusive caching


#include "cache.hpp"
#include "../sc_ext.hh"
#include <algorithm>

// to clean up allocated stuff globally for the whole class
std::vector< cache::req_extension * > cache::req_extension::req_extension_clones;
#ifndef CACHE_DEBUG
    FILE *cache::common_fid = NULL;
#else
    FILE *cache::common_fid = fopen("log/cache_system.log", "w+");
#endif

// constructor
cache::cache(
                sc_core::sc_module_name name, 
                uint32_t id,
                cache::cache_specs specs,
                uint32_t level,
                uint64_t regbase,
                const char *logfile
            )
	        :	sc_module(name),
		        m_trans(),																            // transaction object
		        m_num_upstream_masters(specs.num_masters),									            // total upstream masters (num of children i.e num of caches that share this cache)
		        m_block_size(specs.block_size),												            // total cache size
		        m_num_of_sets(specs.size/(specs.block_size*specs.num_ways)),								            // total number of sets
		        m_num_of_ways(specs.num_ways),												            // total number of ways
		        m_write_back(specs.write_back),												            // write back enable/disable
		        m_write_allocate(specs.write_allocate),										            // write allocate enable/disable
		        m_evict_policy(specs.evict_policy),											            // cache block replacement policy
		        m_log(false),															            // enable logging (on text files referred to by logfile
		        m_level(level),															            // cache hierarchy level (first level cache (L1) is 1)
		        m_current_set(0),														            // set for current request
		        m_current_tag(0),														            // tag for current request
		        m_current_blockAddr(0),													            // cache block address for current request
		        m_current_way(0),														            // cache way for current request
		        m_current_delay(NULL),                              					            // delay object for current request
		        m_id(id),
		        m_misses_type_read(0),
		        m_cache_regspace_base(regbase),		        
		        m_lookup_delay(sc_core::SC_ZERO_TIME),
		        m_read_delay(sc_core::SC_ZERO_TIME),
		        m_write_delay(sc_core::SC_ZERO_TIME),
		        m_fake_back_invalidation(false),
		        m_debug(0),
		        m_CPU(specs.CPU_id)
{
	// ensuring that set bits <= mem_size_bits.....sort of a corner case but not expected 
	// to occur in real cache organizations if this does occur though then cache would be 
	// inefficient as it will have slots for memory words which actually dont exist in 
	// memory......cache will be bigger than memory!
	assert(log2((double) m_num_of_sets) <= log2((double) MEM_SIZE));


	// cache blocks structure
	m_blocks.resize(m_num_of_sets);
	for (uint32_t i=0; i<m_num_of_sets; i++) {
		m_blocks[i].resize(m_num_of_ways);
		for (uint32_t j=0; j<m_num_of_ways; j++) {
			m_blocks[i][j].state = cache_block::I;
			m_blocks[i][j].tag = 0x0;
			m_blocks[i][j].evict_tag = 0x0;
		}
	}
	
	m_alloc_blocks_coremask.resize(specs.num_alloc_blocks);
	for (uint32_t i=0; i<m_alloc_blocks_coremask.size(); i++) {
        m_alloc_blocks_coremask[i].resize(m_num_of_ways);
        for (uint32_t j=0; j<m_num_of_ways; j++) {
            // initially all cores can cache data in any way at any alloc_block
            // right now assuming max 32 smp cores
            m_alloc_blocks_coremask[i][j] = 0xffffffff;
        }        
	}

	// req type indicator
	m_ext = new req_extension();
	m_trans.set_extension(m_ext);

	// target sockets for upstream masters
	m_tsocket_d = new tlm_utils::simple_target_socket< cache >[m_num_upstream_masters];
	for (uint32_t i=0; i<m_num_upstream_masters; i++) {
		m_tsocket_d[i].register_b_transport(this, &cache::b_transport);
	}

	// initiator sockets to upstream slaves (for back invalidation)
	if (m_level > 1) {		// not first level cache
		m_isocket_u = new tlm_utils::simple_initiator_socket< cache >
		                                                 [m_num_upstream_masters];
	} else {
        m_isocket_u = NULL;
    }

	if (m_level < LLC_LEVEL) {			// not last level cache
    	// target sockets for downstream masters (for back invalidation)	
		m_tsocket_u = new tlm_utils::simple_target_socket< cache >("m_tsocket_u");
		m_tsocket_u->register_b_transport(this, &cache::b_transport);
		
		// initiator socket for downstream slave
        m_isocket_d = new tlm_utils::simple_initiator_socket< cache >("m_isocket_d");        		
	} else {
        m_tsocket_u = NULL;
        m_isocket_d = NULL;
    }
    
    m_cfgsocket = new tlm_utils::simple_target_socket< cache >("m_cfgsocket");
    m_cfgsocket->register_b_transport(this, &cache::b_transport);
    
    if (m_level > 1) {
        m_miss_register = new uint64_t[NUM_CPUS + 1];
        m_access_register = new uint64_t[NUM_CPUS + 1];
        for (int i=0; i<=NUM_CPUS; i++) {
            m_access_register[i] = 0;
            m_miss_register[i] = 0;
        }
    } else {
        m_miss_register = new uint64_t;
        m_access_register = new uint64_t;
        *m_access_register = 0;
        *m_miss_register = 0;
    }

	// logging
	if (logfile) {
    	m_fid = fopen(logfile, "w+");
	    assert(m_fid);
	    fprintf(m_fid, "CACHE_CONFIG \r\n");
	    fprintf(m_fid, "total_cache_size:%dB", specs.size);
	    fprintf(m_fid, "..cache_block_size:%dB", m_block_size);
	    fprintf(m_fid, "..num_of_sets:%d", m_num_of_sets);
	    fprintf(m_fid, "..num_of_ways:%d", m_num_of_ways);
	    fprintf(m_fid, "..write_back:%d", m_write_back);
	    fprintf(m_fid, "..write_allocate:%d", m_write_allocate);
	    fprintf(m_fid, "..level:%d", m_level);
	    fprintf(m_fid, "..evict_policy:%d\r\n\r\n", m_evict_policy);
//	    fflush(m_fid);    	
	} else {
	    m_fid = NULL;
	}
}


// deleting allocated stuff to avoid memory leaks
cache::~cache() {
	delete[] m_tsocket_d;
	if (m_level > 1) {
		delete[] m_isocket_u;
	}
	if (m_level < LLC_LEVEL) {
		delete m_tsocket_u;
        delete m_isocket_d;
	}
    
    if (m_fid) {
    	fclose(m_fid);
    }

	m_trans.clear_extension(m_ext);
	delete m_ext;
	
	if (m_level > 1) {
	    delete[] m_miss_register;
	    delete[] m_access_register;
    } else {
        delete m_miss_register;
        delete m_access_register;
    }
}



// enable logging
void 
cache::do_logging() 
{
    if (m_fid) {
    	m_log = true;
	}
}


// port interface method
void 
cache::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) 
{    
    req_extension *ext;
    trans.get_extension(ext);
    uint64_t req_addr = trans.get_address();
    
    if (req_addr>=MEM_BASE && req_addr<(MEM_BASE + MEM_SIZE)) {
        // memory request!!        
	    req_extension::req_type type;
	    if (!ext) {
		    assert(m_level == 1);		// has to be first level
		    type = req_extension::NORMAL;
	    } else {
		    type = ext->m_type;
		    if (m_level > 1) {
		        m_CPU = ext->core_id;
		    }
	    }    	    	    
	    
	    if (type == req_extension::FLUSH) {
	        flush_cache();
	        trans.set_response_status(tlm::TLM_OK_RESPONSE);
	        return;
	    }
	    
	    bool evict_needed;
	    uint64_t current_blockAddr_orig, current_tag_orig;
	    uint32_t current_set_orig;
	    uint32_t current_way_orig;

	    // for the back-invalidation case, keeping track of original 
	    // m_current_blockAddr,_tag,_set so as not to corrupt them with the 
	    // back-invalidate-block that is going to be evicted in some downstream cache
	    if (type!=req_extension::NORMAL) {
		    current_blockAddr_orig = m_current_blockAddr;
		    current_set_orig = m_current_set;
		    current_tag_orig = m_current_tag;
		    current_way_orig = m_current_way;
	    }	    
	    
	    // update for current request (this only works if there are no outstanding request 
	    // like in LT modeling)
	    m_current_blockAddr = (req_addr/m_block_size)*m_block_size;
	    m_current_set = (m_current_blockAddr >> 
	                         (uint32_t)(log2((double)WORD_SIZE) + 
	                                    log2((double)m_block_size/WORD_SIZE))) & 
                        ((1 << (uint32_t)log2((double)m_num_of_sets)) - 1);
	    m_current_tag = (req_addr >> 
	                         (uint32_t)(log2((double)WORD_SIZE) + 
	                                    log2((double)m_block_size/WORD_SIZE) + 
	                                    log2((double)m_num_of_sets)));

        if (m_log) {   
	        if (m_level == 1 && type==req_extension::NORMAL) {                	        
	            if (common_fid) {
		            if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
			            fprintf(common_fid, "-------------------------------writing at ");
		            } else {
			            fprintf(common_fid, "-------------------------------reading at ");
		            }
		            fprintf(common_fid, "0x%08lx------------------------------------\r\n", 
		                                req_addr);
//		            fflush(common_fid);
	            }		
	        }	
	        
	                
            m_debug++;        
        	std::string req_type[4] = {"NORMAL", "M_UPDATE", 
        	                           "BACK_INVALIDATE", "WB_UPDATE"};      	                           
            fprintf(common_fid, "(%s) begin tag:0x%08lx, %d, %d    ", 
                                            req_type[type].c_str(), m_current_tag, 
                                            trans.get_command(),
                                            m_debug);
		    print_cache_set(m_current_set);	                	    
        }                

	    // updating the access register
	    if (type == req_extension::NORMAL) {
            m_access_register[0]++;
            if (m_level > 1) {
                m_access_register[m_CPU + 1]++;
            }     
            m_current_delay = &delay;	    	        
            // doing cache lookup for this new memory request
            *m_current_delay += m_lookup_delay;            
	    }   	            	    

	    if (cache_lookup(evict_needed, m_current_way, 
	                       (type == req_extension::WB_UPDATE))) {
		    // cache hit
		    if (type == req_extension::NORMAL) {
			    // normal request
			    process_hit(trans);
		    } else {
			    // special request
			    process_special_request(type);
		    }
	    } else {
		    // cache miss
		    if (type == req_extension::BACK_INVALIDATE) {
			    // this block is already invalidated (nor present in cache) so do nothing
		    } else {
			    // normal request path			
			    // any special request except back-invalidate shouldn't result in a miss
			    assert(type==req_extension::NORMAL);		
        	    // updating the misses register
                m_miss_register[0]++;
                if (m_level > 1) {
                   m_miss_register[m_CPU + 1]++;
                } 			    
			    	    	
			    process_miss(trans, evict_needed);		                                   
		    }
	    }

	    trans.set_response_status(tlm::TLM_OK_RESPONSE);
	    
	    if (m_log) {       
            fprintf(common_fid, "end     ");
	        print_cache_set(m_current_set);
	    }	    

	    
	    // when done with back invalidation, setting back to original values
	    if (type!=req_extension::NORMAL) {
		    m_current_blockAddr = current_blockAddr_orig;
		    m_current_set = current_set_orig;
		    m_current_tag = current_tag_orig;
		    m_current_way = current_way_orig;
	    }	    	    
    } else if (req_addr>=m_cache_regspace_base && 
                            req_addr<(m_cache_regspace_base + 4096)) {                            
        // request accessing this cache registers
        
        unsigned char *ptr = trans.get_data_ptr();
        assert(ptr);
        unsigned int len = trans.get_data_length();
        assert(len==8);
        if (req_addr == m_cache_regspace_base) {
            memcpy(ptr, &m_access_register[m_misses_type_read], len);
        } else if (req_addr == m_cache_regspace_base + 8) {
            memcpy(ptr, &m_miss_register[m_misses_type_read], len);            
        } else if (req_addr == m_cache_regspace_base + 16) {             
            uint64_t reg_data = *((uint64_t *) ptr);
            char core = reg_data & 0xff;
            assert(core <= NUM_CPUS);
            unsigned short block = (reg_data >> 8) & 0xffff;    
            assert(block <= m_alloc_blocks_coremask.size());
            uint32_t way_mask = (reg_data >> 24) & 0xffffffff;
            for (uint32_t i=0; i<m_num_of_ways; i++) {
                if (way_mask>>i & 1) {
                    m_alloc_blocks_coremask[block][i] |= (1 << core);
                } else {
                    m_alloc_blocks_coremask[block][i] &= ~(1 << core);
                }
            }               
        } else if (req_addr == m_cache_regspace_base + 24) {
            flush_cache();
        } else if (req_addr == m_cache_regspace_base + 32) {
            uint64_t reg_data = *((uint64_t *) ptr);
            if (m_level == 1) {
                assert(reg_data == 0);
            } else {
                assert(reg_data <= NUM_CPUS+1);
            }
            
            m_misses_type_read = reg_data;            
        } else {
            assert(0);
        }
        
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    } else {
        // invalid request received
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    }

}


// performs a cache lookup for the passed req_addr and returns true if hit else false
// also assert evict_needed incase if eviction needed
// way_free logic:
//      -- incase of hit, it is set to found way
//		-- incase of miss, it is set to the way which is going to be refilled with 
//         new block from downstream
bool 
cache::cache_lookup(bool &evict_needed, uint32_t &way_free, bool WB_UPDATE) 
{
    // finding ways inwhich this core has privilege to cache data in case of miss
    m_current_ways_lookup.clear();
    if (m_alloc_blocks_coremask.size()) {
        uint32_t current_alloc_block = (m_current_set*m_alloc_blocks_coremask.size()) 
                                                            /
                                                    m_num_of_sets;
        for (uint32_t i=0; i<m_num_of_ways; i++) {
            if ((m_alloc_blocks_coremask[current_alloc_block][i]>>m_CPU) & 1) {
                m_current_ways_lookup.push_back(i);
            }
        }
    } else {
        for (uint32_t i=0; i<m_num_of_ways; i++) {
            m_current_ways_lookup.push_back(i);
        }
    }    
    // there should atleast be one way for caching missed data for a given smp core
    assert(m_current_ways_lookup.size() > 0);
    
    
    // doing lookup
	evict_needed = true;
	for (uint32_t i=0; i<m_num_of_ways; i++) {
	    bool hit = (m_blocks[m_current_set][i].tag == m_current_tag); 
	    
		if (m_blocks[m_current_set][i].state != cache_block::I) {
			if (hit) {
				evict_needed = false;
				way_free = i;
				return true;
			}
		} else {
            // can this 'I' way be considered for caching for this core??
            if (std::find(m_current_ways_lookup.begin(), m_current_ways_lookup.end(), i) 
                            !=  m_current_ways_lookup.end()) 
            {
                evict_needed = false;            
                way_free = i;                              
            } 			          
		    if (hit && WB_UPDATE) {	    
		        way_free = i;
		        return true;
		    }
		}
	}

	if (evict_needed) {
		way_free = find_way_evict();
	}

	return false;
}


// based on the eviction policy, choose the way to be replaced
uint32_t 
cache::find_way_evict() 
{
	int way_free = -1;

	switch(m_evict_policy) {
		case LRU:
		case FIFO:
		{
		    // LRU/FIFO eviction policies supported only for no cache-partitioning stuff
		    assert(m_current_ways_lookup.size() == m_num_of_ways);		     
			for (uint32_t j=0; j<m_num_of_ways; j++) {
				if (m_blocks[m_current_set][j].evict_tag == m_num_of_ways) {
					way_free = j;
				}
			}
			assert(way_free != -1);
			break;
		}
		case LFU:
		{
		    // LFU eviction policies supported only for no cache-partitioning stuff
		    assert(m_current_ways_lookup.size() == m_num_of_ways);		
			uint64_t tmp = m_blocks[m_current_set][0].evict_tag;
			way_free = 0;
			for (uint32_t j=1; j<m_num_of_ways; j++) {
				if (tmp > m_blocks[m_current_set][j].evict_tag) {
					tmp = m_blocks[m_current_set][j].evict_tag;
					way_free = j;
				}
			}
			break;
		}
		case RAND:
		{
			way_free = m_current_ways_lookup[rand()%m_current_ways_lookup.size()];
			break;
		}
		default:
		{
			assert(0);
		}
	}

	return (uint32_t)way_free;
}


// handle normal hit request
void 
cache::process_hit(tlm::tlm_generic_payload &trans) 
{
	// logging
	if (m_log) 	{
		fprintf(m_fid, "cache hit for 0x%08llux", trans.get_address());
		fprintf(m_fid, "..tag=0x%08lx", m_current_tag);
		fprintf(m_fid, "..set=%d\r\n", m_current_set);
//		fflush(m_fid);			// disable this when not debugging
	}

	tlm::tlm_command cmd = trans.get_command();
	cache_block::cache_block_state *state = &m_blocks[m_current_set][m_current_way].state;
	assert(*state != cache_block::I);			// this shouldn't be invalid state

	if (cmd == tlm::TLM_WRITE_COMMAND) {
		// write hit
		if (m_write_back) {
			if (*state != cache_block::M) {
				if (*state == cache_block::S) {				    
					if (m_level < LLC_LEVEL) {
						// notifying the downstream caches to update their block states 
						// (from S to MBS) via special request						
						m_ext->m_type = req_extension::M_UPDATE;
						m_ext->core_id = m_CPU;
						m_trans.set_command(tlm::TLM_IGNORE_COMMAND);
						send_request(true);
					}
					
					// because of back-invalidations of this block (via M_UPDATE path above)
					// would be messed up so bringing it back up
					if (m_blocks[m_current_set][m_current_way].evict_tag == 0) {
    					m_blocks[m_current_set][m_current_way].evict_tag = m_num_of_ways;
					}
				}
				*state = cache_block::M;
			}
		} else {
			// writing through downstream
			m_trans.set_write();
			m_ext->m_type = req_extension::NORMAL;
			m_ext->core_id = m_CPU;
			send_request(true);
		}
		assert(m_current_delay);
        *m_current_delay += m_write_delay;
	} else {	    
		// read hit
		
		// if this block is in M state somewhere upstream then that is invalidated and
		// forced to write-back to this cache
        if (*state == cache_block::MBS) {
            m_fake_back_invalidation = true;
            send_request(false);
            // writeback should have been done by now
            assert(*state == cache_block::M);
        }
		
		assert(m_current_delay);		
		*m_current_delay += m_read_delay;
	}

	// update eviction policy stuff		
	if (m_evict_policy == LRU) {        	
		for (uint32_t j=0; j<m_num_of_ways; j++) {
			if (j!=m_current_way && m_blocks[m_current_set][j].state!=cache_block::I && 
			             (m_blocks[m_current_set][j].evict_tag <=
			              m_blocks[m_current_set][m_current_way].evict_tag)
	            ) 
	        {
				m_blocks[m_current_set][j].evict_tag++;
			}
			assert(m_blocks[m_current_set][j].evict_tag <= m_num_of_ways);
		}
		m_blocks[m_current_set][m_current_way].evict_tag = 1;
	} else if (m_evict_policy == LFU) {
		m_blocks[m_current_set][m_current_way].evict_tag++;
	}
}

// handle special request on hit
void 
cache::process_special_request(req_extension::req_type type) 
{
	cache_block::cache_block_state *state = &m_blocks[m_current_set][m_current_way].state;
	bool invalidation_done = false;

	switch(type) {
		case req_extension::M_UPDATE : {
			// request from child that it is now caching this block in M state
			
            // can't be in invalid state (strictly inclusive caches)			
			assert(*state != cache_block::I);							
			if (*state != cache_block::MBS) {
				// this has to be writeback cache if in M state
                if (*state == cache_block::M) {
                    assert(m_write_back);
                }
				
				// sending back invalidations to upstream caches so that anyone caching 
				// in 'S' state should invalidate it as their data is not up-to-date
				m_fake_back_invalidation = false;
				send_request(false);				
													
				*state = cache_block::MBS;			    
			}
									
			// forwarding this special request to further downstream caches if present
			if (m_level < LLC_LEVEL) {
				m_ext->m_type = req_extension::M_UPDATE;
				m_ext->core_id = m_CPU;
				m_trans.set_command(tlm::TLM_IGNORE_COMMAND);
				send_request(true);
			}
			break;
		}
		case req_extension::BACK_INVALIDATE : {
			// request from parent for back-invalidation
			if (*state == cache_block::S) {
				*state = cache_block::I;
				invalidation_done = true;				
			} else if (*state == cache_block::M) {
				assert(m_level < LLC_LEVEL);			// this can't be last level
				*state = cache_block::I;
                invalidation_done = true;
				// writing back downstream
				m_ext->m_type = req_extension::WB_UPDATE;
				m_ext->core_id = m_CPU;
				m_trans.set_command(tlm::TLM_IGNORE_COMMAND);
				send_request(true);
			}
			// else no need to do anything if (state = MBS) path as it will be invalidated 
			// on WB_UPDATE path

			// sending back invalidation to its upstream masters as well
			m_fake_back_invalidation = false;
			send_request(false);
			break;
		}
		case req_extension::WB_UPDATE : {
			// request from child for WB update
			if (*state == cache_block::I) {
				// this is the cache that initiated the back-invalidation path
				// writing through downstream
				m_trans.set_write();
				m_ext->m_type = req_extension::NORMAL;
				m_ext->core_id = m_CPU;
				send_request(true);
			} else {
                // this has to be in MBS state
                // TODO: right now for write-through caches as well..can MBS for 
                //       write-through cache be avoided???			
				assert(*state == cache_block::MBS);		
				if (!m_fake_back_invalidation) {		
				    assert(m_level < LLC_LEVEL);					// this can't be last level
				    *state = cache_block::I;
                    invalidation_done = true;
				    // writing back downstream
				    m_ext->m_type = req_extension::WB_UPDATE;
				    m_ext->core_id = m_CPU;
				    m_trans.set_command(tlm::TLM_IGNORE_COMMAND);
				    send_request(true);
			    } else {
			        *state = cache_block::M;
			        break;
			    }
			}
			break;
		}
		case req_extension::NORMAL : {
			assert(0);			// shouldn't come here
			break;
		}
		default : {
			assert(0);			// shouldn't come here
			break;
		}
	}
	
	if (invalidation_done) {
		// updating cache evition stuff				
		// TODO: right now support only for LRU do for others as well
        if (m_evict_policy == cache::LRU) {                    
		    for (uint32_t i=0; i<m_num_of_ways; i++) {
		        if (m_blocks[m_current_set][i].evict_tag > 
		                        m_blocks[m_current_set][m_current_way].evict_tag) 
                {
			        m_blocks[m_current_set][i].evict_tag--;
		        }
		    }
		    m_blocks[m_current_set][m_current_way].evict_tag = 0;	
	    }
	}
}


// handle miss request
void 
cache::process_miss(tlm::tlm_generic_payload &trans, bool evict_needed) 
{ 
	if (m_log) {
		fprintf(m_fid, "cache miss for 0x%08llux", trans.get_address());
		fprintf(m_fid, "..tag=0x%08lx", m_current_tag);
		fprintf(m_fid, "...set=%d\r\n", m_current_set);
//		fflush(m_fid);			
	}

	if (evict_needed) {
		do_eviction();     
	}			
    // this block should be refilled with new data
    assert(m_blocks[m_current_set][m_current_way].state == cache_block::I);		

	tlm::tlm_command cmd = trans.get_command();
	bool no_allocate_on_write_miss = (cmd==tlm::TLM_WRITE_COMMAND && !m_write_allocate);
	if (!no_allocate_on_write_miss) {
		// fetch the block from downstream
		m_trans.set_read();
		m_ext->m_type = req_extension::NORMAL;
		m_ext->core_id = m_CPU;
		send_request(true);			

        // only cache data if there is an allocated way in this set
	    // update tag of block
	    m_blocks[m_current_set][m_current_way].tag = m_current_tag;
	    // update state of block
	    if (cmd == tlm::TLM_WRITE_COMMAND) {
		    // write miss
		    if (m_write_back) {
			    if (m_level < LLC_LEVEL) {
				    m_ext->m_type = req_extension::M_UPDATE;
				    m_ext->core_id = m_CPU;
				    m_trans.set_command(tlm::TLM_IGNORE_COMMAND);
				    send_request(true);
			    }
			    m_blocks[m_current_set][m_current_way].state = cache_block::M;				
		    } else {
			    m_blocks[m_current_set][m_current_way].state = cache_block::S;
			    // writing through downstream
			    m_trans.set_write();
			    m_ext->m_type = req_extension::NORMAL;
			    m_ext->core_id = m_CPU;
			    send_request(true);
		    }
		    assert(m_current_delay);
            *m_current_delay += m_write_delay;
	    } else {
	        // read miss
		    m_blocks[m_current_set][m_current_way].state = cache_block::S;
    		assert(m_current_delay);			
            *m_current_delay += m_read_delay;
	    }

	    // update eviction policy stuff
	    if (m_evict_policy==LRU || m_evict_policy==FIFO) {	        		    
		    for (uint32_t j=0; j<m_num_of_ways; j++) {
			    if (m_current_way!=j && m_blocks[m_current_set][j].state!=cache_block::I) 
			    {
				    m_blocks[m_current_set][j].evict_tag++;
			    }
			    assert(m_blocks[m_current_set][m_current_way].evict_tag <= m_num_of_ways);
		    }
		    m_blocks[m_current_set][m_current_way].evict_tag = 1;
	    } else {
		    m_blocks[m_current_set][m_current_way].evict_tag = 0;
	    }
	} else {
		// no allocation on write miss
		// just bypass this cache and write through downstream
		m_trans.set_write();
		m_ext->m_type = req_extension::NORMAL;
		m_ext->core_id = m_CPU;
		send_request(true);
	}
}

// performs cache block eviction
void 
cache::do_eviction() 
{
    uint64_t m_current_blockAddr_prev = m_current_blockAddr;
	cache_block::cache_block_state *state = &m_blocks[m_current_set][m_current_way].state;
	m_current_blockAddr = ((m_blocks[m_current_set][m_current_way].tag << 
	                             (uint32_t)(log2((double) m_num_of_sets))) | m_current_set
	                            ) << (uint32_t)(log2((double) WORD_SIZE) + 
	                                            log2((double) m_block_size/WORD_SIZE));	
	                                            	                                            
	// incase this block is in 'S' then no writeback is needed
	
	// incase this block is in 'MBS' then this means that this block is being cached 
	// somewhere upstream in 'M' state which would have been written back via the 
	// back-invalidation path above so no writeback is needed
	
	// incase this block is in 'M' then write back is needed downstream
	if (*state==cache_block::M) {
		assert(m_write_back);			// this has to be a writeback cache
		// writing through to-be-evicted block downstream..if a cache exists on the 
		// downstream path then it can update the status of its block (MBS) to M
		m_trans.set_write();
		m_ext->m_type = req_extension::NORMAL;
		m_ext->core_id = m_CPU;
		send_request(true);
	}
	*state = cache_block::I;

	// BACK INVALIDATION
	m_fake_back_invalidation = false;
	send_request(false);
	
	m_current_blockAddr = m_current_blockAddr_prev;
}



void 
cache::flush_cache() {    
    if (m_level > 1) {
        // fllushing all upstream master caches for inclusivity
        for (uint32_t i=0; i<m_num_upstream_masters; i++) {
            m_ext->m_type = req_extension::FLUSH;
            m_trans.set_command(tlm::TLM_IGNORE_COMMAND);		        
            m_trans.set_address(MEM_BASE);
		    m_isocket_u[i]->b_transport(m_trans, *m_current_delay);		
        	assert(m_trans.get_response_status() == tlm::TLM_OK_RESPONSE);	            
        }
        
        for (uint32_t i=1; i<=NUM_CPUS; i++) {
            m_miss_register[i] = 0;
            m_access_register[i] = 0;
        }
    }

	for (uint32_t i=0; i<m_num_of_sets; i++) {
		for (uint32_t j=0; j<m_num_of_ways; j++) {
		    if (m_blocks[i][j].state==cache_block::M || m_blocks[i][j].state==cache_block::MBS) {		   
        		assert(m_write_back);			// this has to be a writeback cache
		        // writing through this block downstream..if a cache exists on the 
		        // downstream path then it can update the status of its block (MBS to M)
	            uint64_t m_current_blockAddr = ((m_blocks[i][j].tag << 
	                                         (uint32_t)(log2((double) m_num_of_sets))) | i
	                                        ) << (uint32_t)(log2((double) WORD_SIZE) + 
	                                                        log2((double) m_block_size/WORD_SIZE));	        		        
		        m_trans.set_write();
		        m_ext->m_type = req_extension::NORMAL;
		        m_ext->core_id = m_CPU;
		        send_request(true);		        
		    }
			m_blocks[i][j].state = cache_block::I;
			m_blocks[i][j].tag = 0x0;
			m_blocks[i][j].evict_tag = 0x0;
		}
	}  
	m_miss_register[0] = 0;
	m_access_register[0] = 0;
}



// request transport mechanism
void 
cache::send_request(bool downstream) 
{
    m_trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    m_trans.set_address(m_current_blockAddr);
	
	if (downstream) {
		// downstream 'true' will send the request downstream if there is any
	    if (m_level < LLC_LEVEL) {
    		(*m_isocket_d)->b_transport(m_trans, *m_current_delay);
			assert(m_trans.get_response_status() == tlm::TLM_OK_RESPONSE);
		} else {
		    if (m_trans.get_command() == tlm::TLM_READ_COMMAND) {
		        // reading cache block from memory
        		assert(m_current_delay);		        
                *m_current_delay += sc_core::sc_time((uint32_t)MEM_DELAY, sc_core::SC_NS);
            } else {
                // assuming write buffers on way to memory so ignoring this delay of 
                // writing the cache block to main memory!!
            }
		}
	} else {	    	
		// send request to all upstream masters if they are present
		if (m_level > 1) {   		
		    for (uint32_t i=0; i<m_num_upstream_masters; i++) {
		        // TODO: right now this interface is only used to send back-invalidates!!
                m_ext->m_type = req_extension::BACK_INVALIDATE;
                m_trans.set_command(tlm::TLM_IGNORE_COMMAND);		        
			    m_isocket_u[i]->b_transport(m_trans, *m_current_delay);		
            	assert(m_trans.get_response_status() == tlm::TLM_OK_RESPONSE);			        
		    }
	    }
	}	
}


// prints for a given set status of all cache blocks (ways) in common log file for whole 
// cache system
void 
cache::print_cache_set(uint32_t set) 
{
	std::string state[4] = {"M", "S", "I", "MBS"};
    if (common_fid) {
	    fprintf(common_fid, "(%s)..", name());
	    fprintf(common_fid, "set:%d\r\n", set);
	    fprintf(common_fid, "------------------\r\n");
	    for (uint32_t x=0; x<m_num_of_ways; x++) {
		    fprintf(common_fid, "way[%d]..", x);
		    fprintf(common_fid, "state=%s..", state[m_blocks[set][x].state].c_str());
		    fprintf(common_fid, "tag=0x%08lx..", m_blocks[set][x].tag);
		    fprintf(common_fid, "lru=%d\r\n", m_blocks[set][x].evict_tag);
	    }
	    fprintf(common_fid, "\r\n\r\n");
//	    fflush(common_fid);			
    }
}



// set delays (in ns) for this cache module
void 
cache::set_delays(uint32_t lookup, uint32_t read, uint32_t write)
{
    m_lookup_delay = sc_core::sc_time(lookup, sc_core::SC_NS);
    m_read_delay = sc_core::sc_time(read, sc_core::SC_NS);
    m_write_delay = sc_core::sc_time(write, sc_core::SC_NS);
}







//TODO: more accurate delay modeling
//TODO: can M_UPDATE and WB_UPDATE be merged???
//TODO: code cleanup




