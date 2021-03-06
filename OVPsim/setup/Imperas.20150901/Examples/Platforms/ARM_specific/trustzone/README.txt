 
  Copyright (c) 2005-2015 Imperas Software Ltd. All Rights Reserved.

  The contents of this file are provided under the Software License
  Agreement that you accepted before downloading this file.

  This source forms part of the Software and can be used for educational,
  training, and demonstration purposes but cannot be used for derivative
  works except in cases where the derivative works require OVP technology
  to run.

  For open source models released under licenses that you can use for
  derivative works, please visit www.ovpworld.org or www.imperas.com
  for the location of the open source models.


FILE:Imperas/Examples/Platforms/ARM_specific/trustzone/README.txt

INTRODUCTION -------------------------------------------------------
This directory contains examples from the Imperas ARM TrustZone Platform
Modeling Application Note. It should be used in conjunction with the 
application note but is briefly described here.

This example includes several example platforms illustrating methods to model 
TrustZone aware peripherals in OVP, and applications to run on these platforms.

The set of examples contained in this directory are described in the application
note available in the installation

Imperas/doc/appnotes/Imperas_ARM_TrustZone_Platform_Modeling_Application_Note.pdf

FILES --------------------------------------------------------------
There are several different examples provided, each including:
1. Application - found in the application directory along with a Makefile
2. platform.tcl - describes the example platform for the iGen program to create
3. Makefile - compiles the application, builds the platform and runs it

PREREQUISITES ------------------------------------------------------

-- ARM Cross Compiler ----------------------------------------------

To use the provided Makefiles you will need to install the armv7 toolchain 
package available from the download page of the OVPworld website at:
    http://www.ovpworld.org/dlp/#arm.toolchains


BUILDING AND RUNNING THE EXAMPLE -------------------------------------
to build and run any of the examples:

> make -C <example directory> run
