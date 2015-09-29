#include "my_mem.hpp"
#include <stdio.h>

my_mem::my_mem(sc_core::sc_module_name module_name, const char *memory_socket, sc_dt::uint64 high_address, unsigned int memory_width)
    : sc_module(module_name), m_socket("my_mem_target_socket"), m_ram(module_name, memory_socket, high_address, memory_width) {
    m_socket.register_b_transport(this, &my_mem::b_transport);
    m_socket.register_transport_dbg(this, &my_mem::debug_transport);
    m_socket.register_get_direct_mem_ptr(this, &my_mem::get_direct_mem_ptr);
}


void my_mem::dmi(bool on) {
    m_ram.dmi(on);
}

memory* my_mem::getMemory() {
    return m_ram.getMemory();
}

void my_mem::b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
    std::cout << "Got a new memory transaction via b_transport!!" << std::endl;
    printf("transaction addr = %x, command = %d, length = %d\r\n", (unsigned int) payload.get_address(), payload.get_command(), payload.get_data_length());

    // TODO: do something regarding ram here!!

    // for now setting Ok response
    tlm::tlm_response_status response_status = tlm::TLM_OK_RESPONSE;
    return;
}

unsigned int my_mem::debug_transport (tlm::tlm_generic_payload &payload) {
	payload.set_dmi_allowed(false);

	std::cout << "Got a new memory transaction via debug_transport!!" << std::endl;
	printf("transaction addr = %x, command = %d, length = %d\r\n", (unsigned int) payload.get_address(), payload.get_command(), payload.get_data_length());
	return payload.get_data_length();
}

bool my_mem::get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) {
	std::cout << "Got a request for DMI on memory!!" << std::endl;
	return false;
}



