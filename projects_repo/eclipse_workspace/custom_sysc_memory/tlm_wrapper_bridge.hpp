/*
 * tlm_wrapper_bridge.h
 *
 *  Created on: Sep 30, 2015
 *      Author: uzair
 */

#ifndef TLM_WRAPPER_BRIDGE_HPP_
#define TLM_WRAPPER_BRIDGE_HPP_

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class tlm_wrapper_bridge : public sc_core::sc_module {
public:
	tlm_wrapper_bridge(sc_core::sc_module_name);
	void b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
	unsigned int debug_transport(tlm::tlm_generic_payload &payload);
	bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data);

	tlm_utils::simple_initiator_socket<tlm_wrapper_bridge> m_isocket;
	tlm_utils::simple_target_socket<tlm_wrapper_bridge> m_tsocket;

private:

};



#endif /* TLM_WRAPPER_BRIDGE_HPP_ */