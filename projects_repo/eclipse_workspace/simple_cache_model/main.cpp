/*
 * main.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: uzair
 */




#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <stdio.h>
#include "cache.hpp"


#define MEM_SIZE 536870912



struct req_type {
	uint64_t addr;
	tlm::tlm_command cmd;
	unsigned char wr_data[4];
	unsigned char rd_data[4];
};

#define NUM_REQ 6

class req_generator : public sc_core::sc_module {
public:
	tlm_utils::simple_initiator_socket<req_generator> m_isocket;

	SC_HAS_PROCESS(req_generator);
	req_generator(sc_core::sc_module_name name)
		:	m_isocket("m_isocket")
	{
		SC_THREAD(main);
	}

	void main() {
		//unsigned char *mem_no_cache = new unsigned char[MEM_SIZE];
		req_type req_stimuli[NUM_REQ] = {
									{0x12345678, tlm::TLM_READ_COMMAND},
									{0x1234567c, tlm::TLM_WRITE_COMMAND, {1,2,3,4}},
									{0x00005678, tlm::TLM_READ_COMMAND},
									{0x0fff5678, tlm::TLM_READ_COMMAND},
									{0x1fff5678, tlm::TLM_READ_COMMAND},
									{0x10ff5678, tlm::TLM_READ_COMMAND}
								  };

		while(1) {
#if 0
			tlm::tlm_generic_payload trans;
			trans.set_address(rand()%0x20000000);
			//printf("@0x%08x..", trans.get_address());
			trans.set_data_length(4);
			unsigned char *ptr = new unsigned char[4];
			if (rand()%2) {
				trans.set_read();
				printf("RD..");
			} else {
				trans.set_write();
				printf("WR..");
				for (int i=0; i<3; i++) {
					*(ptr+i) = rand();
				}
				/*printf("WR data..data[0]=0x%02x | ", *ptr);
				printf("data[1]=0x%02x | ", *(ptr+1));
				printf("data[2]=0x%02x | ", *(ptr+2));
				printf("data[3]=0x%02x..", *(ptr+3));*/

				//std::memcpy(&mem_no_cache[(trans.get_address()/4)*4], ptr, 4);				// for verification
			}
			trans.set_data_ptr(ptr);
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

			m_isocket->b_transport(trans, delay);

			if (trans.get_command() == tlm::TLM_READ_COMMAND) {
				/*printf("RD data..data[0]=0x%02x | ", *ptr);
				printf("data[1]=0x%02x | ", *(ptr+1));
				printf("data[2]=0x%02x | ", *(ptr+2));
				printf("data[3]=0x%02x\r\n", *(ptr+3));*/
				/*for (int i=0; i<4; i++) {
					assert(mem_no_cache[(trans.get_address()/4)*4 + i] == *(ptr+i));		// for verification
				}*/
			}
#endif

			tlm::tlm_generic_payload trans;

			trans.set_data_length(4);
			for (int i=0; i<NUM_REQ; i++) {
				sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

				trans.set_address(req_stimuli[i].addr);
				if (req_stimuli[i].cmd == tlm::TLM_WRITE_COMMAND) {
					trans.set_command(tlm::TLM_WRITE_COMMAND);
					trans.set_data_ptr((unsigned char *)&req_stimuli[i].wr_data);
				} else {
					trans.set_command(tlm::TLM_READ_COMMAND);
					trans.set_data_ptr((unsigned char *)&req_stimuli[i].rd_data);
				}

				m_isocket->b_transport(trans, delay);

				printf("delay:%s\r\n---------------\r\n", delay.to_string().c_str());

				wait(2, sc_core::SC_NS);
			}

			assert(0);
		}
	}
};

#define WORD_SIZE 4

class target : public sc_core::sc_module {
public:
	tlm_utils::simple_target_socket<target> m_tsocket;

	target(sc_core::sc_module_name name, unsigned char *mem)
		:	m_tsocket("m_tsocket"),
			m_mem(mem),
			m_cache("m_cache", 65536, 16, 4, false)
	{
		m_tsocket.register_b_transport(this, &target::b_transport);
	}

	void b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
		uint64_t addr = (payload.get_address()/WORD_SIZE)*WORD_SIZE;
		tlm::tlm_command cmd = payload.get_command();
		uint32_t len = payload.get_data_length();
		unsigned char *ptr = payload.get_data_ptr();

		assert(addr>=0x00000000 && addr<0x20000000);

		m_cache.update(payload, delay);

		if (cmd == tlm::TLM_WRITE_COMMAND) {
			std::memcpy(&m_mem[addr], ptr, (unsigned long) len);
		} else {
			std::memcpy(ptr, &m_mem[addr], (unsigned long) len);
		}
	}

private:
	unsigned char *m_mem;
	cache m_cache;
};




int sc_main(int argc, char *argv[]) {
	unsigned char *mem = new unsigned char[MEM_SIZE];

	req_generator m_req("m_req");
	target m_target("m_target", mem);

	m_req.m_isocket(m_target.m_tsocket);

	sc_core::sc_start(1, sc_core::SC_MS);

	return 0;
}
