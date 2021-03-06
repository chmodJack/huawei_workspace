/*
 *
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "icm/icmCpuManager.h"

#define PROC_ATTRS ICM_ATTR_TRACE|ICM_ATTR_SIMEX

//
// Simulation main loop
//
static Bool simulate(icmProcessorP processor, Uns64 instructions) {

    icmStopReason stopReason = icmSimulate(processor, instructions);

    switch(stopReason) {

       case ICM_SR_SCHED:
           // hit the scheduler limit
           return True;

       case ICM_SR_EXIT:
           // processor has exited
           return False;

       case ICM_SR_FINISH:
           // simulation must end
           return False;

       default:
           icmPrintf("unimplemented stopReason %u\n", stopReason);
           return False;
    }
}

//
// Report AxPROT for the processor access
//
static void reportAxPROT(
    icmProcessorP processor,
    Bool          isFetch,
    icmRegInfoP   transactPL,
    Addr          address
) {
    // take action only for non-artifact accesses
    if(processor) {

        Bool isNS = (address & 0x10000000000ULL) && True;
        Uns32 PL;

        icmReadRegInfoValue(processor, transactPL, &PL);
        icmPrintf("    AxPROT=%u%u%u VA=0x"FMT_640Nx"\n", !isFetch, isNS, PL, address);
    }
}

//
// Watch callbacks for memory accesses
//
static ICM_MEM_WATCH_FN(watchRead)  {reportAxPROT(processor, False, userData, address);}
static ICM_MEM_WATCH_FN(watchWrite) {reportAxPROT(processor, False, userData, address);}
static ICM_MEM_WATCH_FN(watchFetch) {reportAxPROT(processor, True,  userData, address);}

//
// Main simulation routine
//
int main(int argc, char ** argv) {

    // check for the application program name argument
    if(argc!=2) {
        icmPrintf("Usage: %s <application file name>\n", argv[0]);
    }

    // initialize CpuManager
    icmInitPlatform(ICM_VERSION, 0, 0, 0, "platform");

    // create busses
    icmBusP busPhys = icmNewBus("busPhys", 40);
    icmBusP busNS   = icmNewBus("busNS",   41);

    // create physical memory
    icmMemoryP mem  = icmNewMemory("memory", ICM_PRIV_RWX, 0xffffffffffULL);
    icmConnectMemoryToBus(busPhys, "p1", mem, 0);

    // create aliases
    icmNewBusBridge(busNS, busPhys, "secure",     "sp1", "mp1", 0x0, 0xffffffffffULL, 0x00000000000ULL);
    icmNewBusBridge(busNS, busPhys, "non-secure", "sp2", "mp2", 0x0, 0xffffffffffULL, 0x10000000000ULL);

    // set up processor instance attributes
    icmAttrListP userAttrs = icmNewAttrList();
    icmAddStringAttr(userAttrs, "endian", "little");
    icmAddStringAttr(userAttrs, "variant", "Cortex-A9UP");

    // create a processor and connect to bus
    const char *model    =
        icmTryVlnvString(NULL, "arm.ovpworld.org", "processor",    "arm",         "1.0", "model");
    const char *semihost =
        icmTryVlnvString(NULL, "ovpworld.org",     "modelSupport", "imperasExit", "1.0", "model");
    icmProcessorP processor = icmNewProcessor(
        "cpu1",                             // CPU name
        "arm",                              // CPU type
        0,                                  // CPU cpuId
        0,                                  // CPU model flags
        41,                                 // address bits
        model,                              // model file
        "modelAttrs",                       // morpher attributes
        PROC_ATTRS,                         // processor attributes
        userAttrs,                          // user-defined attributes
        semihost,                           // semi-hosting file
        "modelAttrs"                        // semi-hosting attributes
    );
    icmConnectProcessorBusses(processor, busNS, busNS);

    // get register descriptor for transactPL register
    icmRegInfoP transactPL = icmGetRegByName(processor, "transactPL");

    // add callbacks to monitor the accesses
    icmAddBusReadCallback (busNS, NULL, 0x00000000000ULL, 0x1ffffffffffULL, watchRead,  transactPL);
    icmAddBusWriteCallback(busNS, NULL, 0x00000000000ULL, 0x1ffffffffffULL, watchWrite, transactPL);
    icmAddBusFetchCallback(busNS, NULL, 0x00000000000ULL, 0x1ffffffffffULL, watchFetch, transactPL);

    // load the processor object file
    icmLoadProcessorMemory(processor, argv[1], ICM_LOAD_DEFAULT, False, True);

    // run processor
    simulate(processor, 20000);

    icmTerminate();

    return 0;
}

