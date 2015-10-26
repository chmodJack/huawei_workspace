/*
 * main.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: uzair
 */




#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <stdio.h>
#include "cache.hpp"

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
		unsigned char *mem_no_cache = new unsigned char[536870912];

		while(1) {
			tlm::tlm_generic_payload trans;
			trans.set_address(rand()%1000);
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

				std::memcpy(&mem_no_cache[(trans.get_address()/4)*4], ptr, 4);				// for verification
			}
			trans.set_data_ptr(ptr);
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

			m_isocket->b_transport(trans, delay);

			if (trans.get_command() == tlm::TLM_READ_COMMAND) {
				/*printf("RD data..data[0]=0x%02x | ", *ptr);
				printf("data[1]=0x%02x | ", *(ptr+1));
				printf("data[2]=0x%02x | ", *(ptr+2));
				printf("data[3]=0x%02x\r\n", *(ptr+3));*/
				for (int i=0; i<4; i++) {
					assert(mem_no_cache[(trans.get_address()/4)*4 + i] == *(ptr+i));		// for verification
				}
			}

			wait(2, sc_core::SC_NS);
		}
	}
};

int sc_main(int argc, char *argv[]) {
	req_generator m_req("m_req");
	//cache m_cache("m_cache", 1024, 8, 4);
	cache m_cache("m_cache", 65536, 16, 4);

	m_req.m_isocket(m_cache.m_tsocket);

	sc_core::sc_start(1, sc_core::SC_MS);

	return 0;
}
