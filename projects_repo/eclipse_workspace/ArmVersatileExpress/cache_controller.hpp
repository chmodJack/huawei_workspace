/*
 * cache_controller.hpp
 *
 *  Created on: Oct 13, 2015
 *      Author: uzair
 */

#ifndef CACHE_CONTROLLER_HPP_
#define CACHE_CONTROLLER_HPP_


#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <stdio.h>
#include "cache.hpp"


#define L1_TO_L2_CACHEBLOCK_DELAY sc_core::sc_time(20, sc_core::SC_NS)
#define L2_TO_L1_CACHEBLOCK_DELAY sc_core::sc_time(20, sc_core::SC_NS)

#define L2_TO_MEM_CACHEBLOCK_DELAY sc_core::sc_time(40, sc_core::SC_NS)
#define MEM_TO_L2_CACHEBLOCK_DELAY sc_core::sc_time(200, sc_core::SC_NS)

#define L1_LOOKUP_DELAY sc_core::sc_time(1, sc_core::SC_NS)
#define L1_READ_DELAY sc_core::sc_time(5, sc_core::SC_NS)
#define L1_WRITE_DELAY sc_core::sc_time(5, sc_core::SC_NS)

#define L2_LOOKUP_DELAY sc_core::sc_time(2, sc_core::SC_NS)
#define L2_READ_DELAY sc_core::sc_time(10, sc_core::SC_NS)
#define L2_WRITE_DELAY sc_core::sc_time(10, sc_core::SC_NS)

#define L1_TO_CPU_DELAY sc_core::sc_time(10, sc_core::SC_NS)
#define CPU_TO_L1_DELAY sc_core::sc_time(10, sc_core::SC_NS)


class cache_controller : public sc_core::sc_module {
private:
	uint32_t m_num_smp_cores;
	bool m_dmi_mode;
	bool m_do_dmi;
	tlm::tlm_dmi m_dmi_data;
	int m_req_count;			// for debugging
	bool m_debug;
	std::vector< cache * > m_l1cache_i;
	std::vector< cache * > m_l1cache_d;
	cache m_l2cache;

	void b_transport(int SocektId, tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
	unsigned int transport_dbg(int SocketId, tlm::tlm_generic_payload &payload);
	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,  sc_dt::uint64 end_range);
	bool get_direct_mem_ptr(int SocketId, tlm::tlm_generic_payload &payload, tlm::tlm_dmi &dmi_data);
	bool set_dmi_mode(bool dmi_enable);
	bool get_dmi_mode();

public:
	//TODO: use a multipassthroughsocket for target interface
	std::vector< tlm_utils::simple_target_socket_tagged<cache_controller> * > m_tsocket;
	tlm_utils::simple_initiator_socket<cache_controller> m_Ibus_isocket;
	tlm_utils::simple_initiator_socket<cache_controller> m_Dbus_isocket;

	cache_controller(sc_core::sc_module_name name, int num_smp_cores);
	~cache_controller();
};






//NOTES:

// right now no support for GIC Register interface

// for this specific platform we know that both Instruction-Access and Data-Access use the same memory so we are going to use same DMI
// infrastructure for both of them i.e no separate handling for DMI for either of them


// TODO: should delete the pushed back pointers in "vector< ClassName * > ..." before this vector gets out of scope or gets destroyed to avoid memory leaks


#endif /* CACHE_CONTROLLER_HPP_ */






















