/*
 * my_icmCpuMasterPort.hpp
 *
 *  Created on: Oct 12, 2015
 *      Author: uzair
 */

// duplicate of the code from OVP (tlmProcessor.hpp) with minor modifications


#ifndef MY_ICMCPUMASTERPORT_HPP_
#define MY_ICMCPUMASTERPORT_HPP_

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include "icm/icmCpuManager.hpp"
#include "ovpworld.org/modelSupport/tlmProcessor/1.0/tlm2.0/tlmProcessor.hpp"

using namespace icmCpuManager;

class icmCpu;
class icmInitiatorExtension;


class my_icmCpuMasterPort {
private:
	Uns32 m_addrBits;
	const char *m_name;
	icmBusObject m_IBus;
	icmBusObject m_DBus;
	icmBusObject m_GICRegBus;
	//icmBusObject m_bus;
	sc_core::sc_time m_zero_delay;
	bool m_warn_data_ptr;
	icmCpu *m_cpu;
	icmProcessorP m_smp_core;
	Uns32 m_maxBytes;
	my_icmCpuMasterPort *m_dflt;
	icmInitiatorExtension *m_initiator;
	tlm::tlm_generic_payload m_trans;

	icmMemCallback *cbTryDMI;
	icmMemCallback *cbNoDMI;
	icmMemCallback *cbRdDMI;
	icmMemCallback *cbWrDMI;
	icmMemCallback *cbRWDMI;

	// text conversion function
	const char *response();
	// used by each bus master operation
	void readUpcall(Addr addr, unsigned char *value, Uns32 bytes, void *simulate, bool &first, bool tryDMI);
	void writeUpcall(Addr addr, const unsigned char *value, Uns32 bytes, void *simulate, bool &first, bool tryDMI);
	// convert bits to mask
	Addr maxValue(Uns32 bits) { return (bits >= 64) ? 0xFFFFFFFFFFFFFFFFll : ((Addr)1 << bits)-1; }
	// static callback
	static void readTryDMI(Addr address, Uns32 bytes, void *value, void *userData, void *simulate);
	static void writeTryDMI(Addr address, Uns32 bytes, const void *value, void *userData, void *simulate);
	static void readNoDMI(Addr address, Uns32 bytes, void *value, void *userData, void *processor);
	static void writeNoDMI(Addr address, Uns32 bytes, const void *value, void *userData, void *simulate);
	// see if DMI is available and use it to map the memory if possible.
	void tryDmi(Addr addr, Bool read);
    /// Invalidate this DMI region
    /// @param low       Lower extent of the region.
    /// @param high      Upper extent of the region.
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);

public:
    /// The TLM initiator socket. This name  must be used in the
    /// binding in the platform.
    tlm_utils::simple_initiator_socket<my_icmCpuMasterPort> m_IBUS_isocket;
    tlm_utils::simple_initiator_socket<my_icmCpuMasterPort> m_DBUS_isocket;
    tlm_utils::simple_initiator_socket<my_icmCpuMasterPort> m_GICRegBUS_isocket;

    /// The constructor.
    /// @param pse      Pointer to PSE.
    /// @param name     Namer of bus port as appears in OVP model.
    /// @param addrBits Number of address bits supported by this port.
    my_icmCpuMasterPort(icmCpu *cpu, const char *name, Uns32 addrBits, icmProcessorP smp_core);

    /// Destructor (not usually called explicitly).
    ~my_icmCpuMasterPort() {}

    /// Memory mapping function. Note that any region within the processor's address
    /// space not mapped by one of these functions will default to TLM memory.
    /// Map this region to local OVP memory.
    /// @param name      A unique name for this memory region.
    /// @param low       Lower extent of the region.
    /// @param high      Upper extent of the region.
    void mapLocalMemory(Addr low, Addr high, icmBusObject* &localBus);

    /// Map this region to native memory.
    /// @param name      A unique name for this memory region.
    /// @param ptr       Pointer to the native memory.
    /// @param low       Lower extent of the region.
    /// @param high      Upper extent of the region.
    /// @param priv      Read, write and alignment Privileges
    void mapNativeMemory(unsigned char *ptr, Addr low, Addr high, icmPriv priv);

    /// Un-map this region (back to callbacks)
    /// @param low       Lower extent of the region.
    /// @param high      Upper extent of the region.
    void unmapNativeMemory(Addr low, Addr high);

    /// Bind to a dummy port if unbound
    /// This will suppress end-of-elaboration errors if the port is unbound
    void bindIfNotBound();
    void bindIfNotBound(my_icmCpuMasterPort *dflt);

    /// Invalidate the whole DMI region
    void invalidateDMI(void);
};





#endif /* MY_ICMCPUMASTERPORT_HPP_ */

