// instantiates the OVP ram module

#ifndef __MY_MEM_H__
#define __MY_MEM_H__

#include "tlm.h"            // include systemc+tlm2.0 stuff
#include "tlm_utils/simple_target_socket.h"

#include "ovpworld.org/memory/ram/1.0/tlm2.0/tlmMemory.hpp"

class my_mem: public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<my_mem> m_socket;

    my_mem(sc_core::sc_module_name module_name, const char *memory_socket, sc_dt::uint64 high_address, unsigned int memory_width = 4);      // memory width in bytes!!
    void dmi(bool on);      // DMI for master
    memory* getMemory();    // return a pointer to the memory

private:
    ram m_ram;

    void b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
    unsigned int debug_transport (tlm::tlm_generic_payload &payload);
    bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data);
};


#endif
