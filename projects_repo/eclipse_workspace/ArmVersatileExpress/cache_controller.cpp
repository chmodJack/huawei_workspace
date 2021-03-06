/*
 * cache_controller.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: uzair
 */

#include "cache_controller.hpp"

#define MEM_BASE 0x60000000 		// physical base address for DDR2 RAM in our platform (in agreement with already built linux kernel)

cache_controller::cache_controller(sc_core::sc_module_name name, int num_smp_cores)
	:	sc_core::sc_module(name),
		m_num_smp_cores(num_smp_cores),
		m_dmi_mode(true),
		m_do_dmi(false),
		m_req_count(0),
		m_debug(false),
		m_l2cache("m_l2cache", "log/l2cache.log", 2097152, 16, 32, 8),				// 2MB cache
		m_Ibus_isocket("m_Ibus_isocket"),
		m_Dbus_isocket("m_Dbus_isocket")
{
	m_tsocket.reserve(m_num_smp_cores*3);					// 3 groups: INSTRUCTION, DATA, GICREGISTERS
	m_l1cache_i.reserve(m_num_smp_cores);
	m_l1cache_d.reserve(m_num_smp_cores);

	m_l2cache.set_parent(NULL);
	m_l2cache.set_delays((sc_core::sc_time)MEM_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time)L2_TO_MEM_CACHEBLOCK_DELAY, (sc_core::sc_time) L2_LOOKUP_DELAY, (sc_core::sc_time) L2_WRITE_DELAY, (sc_core::sc_time) L2_READ_DELAY);

	for (uint32_t i=0; i<m_num_smp_cores*3; i++) {
		char socket_name[20], log_name[40];
		sprintf(socket_name, "m_tsocket[%d]", i);
		m_tsocket.push_back(new tlm_utils::simple_target_socket_tagged<cache_controller>(socket_name));
		m_tsocket[i]->register_b_transport(this, &cache_controller::b_transport, i);
		m_tsocket[i]->register_transport_dbg(this, &cache_controller::transport_dbg, i);
		m_tsocket[i]->register_get_direct_mem_ptr(this, &cache_controller::get_direct_mem_ptr, i);

		// constructing l1 caches (split i/d pair for each smp)
		if (i<m_num_smp_cores) {
			sprintf(socket_name, "m_l1cache_i[%d]", i);
			sprintf(log_name, "log/smp%d_l1cache_i.log", i);
			m_l1cache_i.push_back(new cache(socket_name, log_name, 65536, 16, 4, 0, false));			// WT cache to avoid coherency issues b/w smp l1 caches as they all are sharing l2 cache
			m_l1cache_i[i]->set_parent(&m_l2cache);
			m_l1cache_i[i]->set_children(NULL);
			m_l1cache_i[i]->set_delays((sc_core::sc_time)L2_TO_L1_CACHEBLOCK_DELAY, (sc_core::sc_time)L1_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time) L1_LOOKUP_DELAY, (sc_core::sc_time) L1_WRITE_DELAY, (sc_core::sc_time) L1_READ_DELAY);

			sprintf(socket_name, "m_l1cache_d[%d]", i);
			sprintf(log_name, "log/smp%d_l1cache_d.log", i);
			m_l1cache_d.push_back(new cache(socket_name, log_name, 65536, 16, 4, 0, false));			// WT cache to avoid coherency issues b/w smp l1 caches as they all are sharing l2 cache
			m_l1cache_d[i]->set_parent(&m_l2cache);
			m_l1cache_d[i]->set_children(NULL);
			m_l1cache_d[i]->set_delays((sc_core::sc_time)L2_TO_L1_CACHEBLOCK_DELAY, (sc_core::sc_time)L1_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time) L1_LOOKUP_DELAY, (sc_core::sc_time) L1_WRITE_DELAY, (sc_core::sc_time) L1_READ_DELAY);

			m_l2cache.set_children(m_l1cache_i[i]);
			m_l2cache.set_children(m_l1cache_d[i]);
		}
	}

	m_Ibus_isocket.register_invalidate_direct_mem_ptr(this, &cache_controller::invalidate_direct_mem_ptr);
	m_Dbus_isocket.register_invalidate_direct_mem_ptr(this, &cache_controller::invalidate_direct_mem_ptr);
}

cache_controller::~cache_controller() {
	for (uint32_t i=0; i<m_num_smp_cores; i++) {
		delete m_tsocket[i];
		delete m_l1cache_i[i];
		delete m_l1cache_d[i];
	}
}


void cache_controller::b_transport(int SocketId, tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
	uint64_t addr = payload.get_address();
	tlm::tlm_command cmd = payload.get_command();
	unsigned char *trans_ptr = payload.get_data_ptr();
	bool mem_transaction = (addr>=(uint64_t)MEM_BASE && addr<(uint64_t)MEM_BASE+0x20000000);


	// updating cache stuff if it is enabled
	if (m_debug && mem_transaction) {
		delay = sc_core::SC_ZERO_TIME;
		if (cmd == tlm::TLM_WRITE_COMMAND) {
			delay += CPU_TO_L1_DELAY;
		}

		if (SocketId < 4) {
			// instruction request
			m_l1cache_i[SocketId]->update(payload, delay);
		} else if (SocketId < 8) {
			// data request
			m_l1cache_d[SocketId-4]->update(payload, delay);
		}

		wait(delay);
	}

	// checking for startup_app to have run on linux/platform...the startup_app writes 0x12345678 @ 0x70000000 platform physical address
	if (cmd == tlm::TLM_WRITE_COMMAND && SocketId >= 4 && !m_debug) {
		if (addr == 0x70000000) {
			if (*trans_ptr==0x78 && *(trans_ptr+1)==0x56 && *(trans_ptr+2)==0x34 && *(trans_ptr+3)==0x12) {
				m_debug = true;
				printf("detect the running of startup_app on linux/platform\r\n");
				printf("cache system enabled from now on\r\n");
			}
		}
	}


	if (m_do_dmi && mem_transaction) {
		// first trying out DMI
		unsigned int mem_req_offset = payload.get_address() - MEM_BASE;
		// doing dmi
		if (payload.get_command() == tlm::TLM_READ_COMMAND) {
			assert(m_dmi_data.is_read_allowed());
			std::memcpy(payload.get_data_ptr(), m_dmi_data.get_dmi_ptr()+mem_req_offset, payload.get_data_length());
		} else if (payload.get_command() == tlm::TLM_WRITE_COMMAND) {
			assert(m_dmi_data.is_write_allowed());
			std::memcpy(m_dmi_data.get_dmi_ptr()+mem_req_offset, payload.get_data_ptr(), payload.get_data_length());
		}
		payload.set_response_status(tlm::TLM_OK_RESPONSE);
	} else {
		// accessing memory over the bus
		if (SocketId<4) {			// request from INSTRUCTION BUS
			m_Ibus_isocket->b_transport(payload, delay);
		} else if (SocketId>=4 && SocketId<8) {		// request from DATA BUS
			m_Dbus_isocket->b_transport(payload, delay);
		} else {
			// gicregisters interface
		}

		// if set in dmi mode then looking out for possiblity for using dmi
		if (m_dmi_mode && mem_transaction) {
			// checking if there is possibility to use dmi for future accesses
			if (payload.is_dmi_allowed()) {
				// getting dmi pointer and noting that we should use dmi from now on
				m_dmi_data.init();
				tlm::tlm_generic_payload trans;
				trans.set_address(MEM_BASE);
				if (get_direct_mem_ptr(SocketId, trans, m_dmi_data)) {
					m_do_dmi = true;
				} else {
					assert(0);					// dmi allowed from target but couldnt get valid data pointer from it!!
				}
			}
		}
	}
}


unsigned int cache_controller::transport_dbg(int SocketId, tlm::tlm_generic_payload &payload) {
	// this debug interface will only be used by INSTRUCTION port for simulation efficiency

	bool mem_transaction = (payload.get_address()>=(uint64_t)MEM_BASE && payload.get_address()<(uint64_t)MEM_BASE+0x20000000);
	assert(mem_transaction);				// instruction should always be fetched from (MEM_BASE --> MEM_BASE + MEM_SIZE)
	if (m_do_dmi) {
		// doing dmi
		unsigned int mem_req_offset = payload.get_address() - MEM_BASE;
		if (payload.get_command() == tlm::TLM_READ_COMMAND) {
			assert(m_dmi_data.is_read_allowed());
			std::memcpy(payload.get_data_ptr(), m_dmi_data.get_dmi_ptr()+mem_req_offset, payload.get_data_length());
		} else if (payload.get_command() == tlm::TLM_WRITE_COMMAND) {			// to place initial bootload code
			assert(m_dmi_data.is_write_allowed());
			std::memcpy(m_dmi_data.get_dmi_ptr()+mem_req_offset, payload.get_data_ptr(), payload.get_data_length());
		}
		payload.set_response_status(tlm::TLM_OK_RESPONSE);
	} else {
		// the cpu fetches instructions via the debug interface from socket0 irrespective of which smp-cpu actually requested it
		if (SocketId<4) {
			assert(SocketId == 0);						// all cpu debug transactions are routed via socket0 irrespective of which cpu actually made the transaction
			// accessing debug interface via the IBus
			return m_Ibus_isocket->transport_dbg(payload);
		} else if (SocketId>=4 && SocketId<8) {
			assert(SocketId == 4);						// all cpu debug transactions are routed via socket0 irrespective of which cpu actually made the transaction
			// accessing debug interface via the DBus
			return m_Dbus_isocket->transport_dbg(payload);
		}
	}

	return 1;
}


void cache_controller::invalidate_direct_mem_ptr(sc_dt::uint64 start_range,  sc_dt::uint64 end_range) {
	// invalidating DMI for every smp core
	for (uint32_t i=0; i<m_num_smp_cores; i++) {
		(*m_tsocket[i])->invalidate_direct_mem_ptr(start_range, end_range);
	}

	// also invalidating this module's dmi pointers as well
	m_dmi_data.init();
	m_do_dmi = false;
}


bool cache_controller::get_direct_mem_ptr(int SocketId, tlm::tlm_generic_payload &payload, tlm::tlm_dmi &dmi_data) {
	if (SocketId<4) {
		return m_Ibus_isocket->get_direct_mem_ptr(payload, dmi_data);
	} else if (SocketId>=4 && SocketId<8) {
		return m_Dbus_isocket->get_direct_mem_ptr(payload, dmi_data);
	} else {
		// gicregisters interface
	}

	return false;
}



bool cache_controller::set_dmi_mode(bool dmi_enable) {
	if (dmi_enable && m_do_dmi) {
		assert(0);				// dmi already enabled and being used then why we got request to enable dmi again???
	}

	m_dmi_mode = dmi_enable;
	// forcefully requesting dmi ptr from the target
	tlm::tlm_generic_payload trans;
	trans.set_address(MEM_BASE);
	m_dmi_data.init();
	if (get_direct_mem_ptr(0, trans, m_dmi_data)) {
		// successful in getting dmi pointer
		m_do_dmi = true;
		return true;
	} else {
		// unsuccessful, so returning false
		return false;
	}
}

bool cache_controller::get_dmi_mode() {
	return m_dmi_mode;
}







