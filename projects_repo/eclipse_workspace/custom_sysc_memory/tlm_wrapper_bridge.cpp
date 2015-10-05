/*
 * tlm_wrapper_bridge.cpp
 *
 *  Created on: Sep 30, 2015
 *      Author: uzair
 */


#include "tlm_wrapper_bridge.hpp"
#include <stdio.h>

//#define DO_LOGGING

tlm_wrapper_bridge::tlm_wrapper_bridge(sc_core::sc_module_name name)
		: sc_module(name), m_isocket("m_isocket"), m_tsocket("m_tsocket")	{
	m_tsocket.register_b_transport(this, &tlm_wrapper_bridge::b_transport);
	m_tsocket.register_transport_dbg(this, &tlm_wrapper_bridge::debug_transport);
	m_tsocket.register_get_direct_mem_ptr(this, &tlm_wrapper_bridge::get_direct_mem_ptr);
}


void tlm_wrapper_bridge::b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
	// forwarding to main memory module via the initiator socket
	m_isocket->b_transport(payload, delay);

	return;
}


unsigned int tlm_wrapper_bridge::debug_transport(tlm::tlm_generic_payload &payload) {
	return m_isocket->transport_dbg(payload);
}



bool tlm_wrapper_bridge::get_direct_mem_ptr(tlm::tlm_generic_payload &payload, tlm::tlm_dmi &dmi_data) {
	return m_isocket->get_direct_mem_ptr(payload, dmi_data);
}



