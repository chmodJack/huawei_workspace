/*
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL).  The
   contribution of Infineon in the development of this tracing
   technology is hereby acknowledged.

 *****************************************************************************/

#include "ovpworld.org/modelSupport/tlmPlatform/1.0/tlm2.0/tlmPlatform.hpp"

void time_advance::cycle(bool this_is_a_delta_cycle)
{
    if (!this_is_a_delta_cycle && m_advanceFn) {
        if (!m_advanceFn(sc_time_stamp().to_seconds(), m_user)) {
            sc_stop();
        }
    }
}

icmTLMPlatform *icmTLMPlatform::m_singleton;

icmTLMPlatform::icmTLMPlatform(
    sc_module_name module_name,
    icmInitAttrs   simAttrs,
    const char    *dbgProtocol,
    Uns32          dbgPort
):
    sc_module(module_name),
    icmPlatform(module_name, simAttrs, dbgProtocol, dbgPort),
    m_quantum(1, SC_MS)
{
    // register the trace callback
    sc_get_curr_simcontext()->add_trace_file(&m_timeAdvance);

    // register the time callback
    m_timeAdvance.setCallback(advance, this);

    if (m_singleton) {
        icmPrintf("TLM: Trying to create more than one platform\n");
        exit(1);
    }
    // set global pointer to platform
    m_singleton = this;
}

// defining this virtual fn from sc_module causes our startup to be called
// before simulation starts
void icmTLMPlatform::start_of_simulation(void){
    simulationStarting();
}

//
// defining this virtual function from sc_module causes our shutdown to be called
// before SystemC starts calling destructors.
// This only gets called by sc_core::sc_stop()
//
void icmTLMPlatform::end_of_simulation(void) {
    simulationEnding();
}

