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

class cache_controller : public sc_core::sc_module {
public:
	std::vector< tlm_utils::simple_target_socket_tagged<cache_controller> * > m_tsocket;
	tlm_utils::simple_initiator_socket<cache_controller> m_Ibus_isocket;
	tlm_utils::simple_initiator_socket<cache_controller> m_Dbus_isocket;

	cache_controller(sc_core::sc_module_name name, int num_smp_cores);

private:
	int m_num_smp_cores;

	void b_transport(int SocektId, tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
	unsigned int transport_dbg(int SocketId, tlm::tlm_generic_payload &payload);
	//TODO: DMI stuff
};


#endif /* CACHE_CONTROLLER_HPP_ */





















