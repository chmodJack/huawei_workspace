IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined")
endif


CROSS?=OR1K

all: model.$(IMPERAS_SHRSUF) \
     semihosting/model.$(IMPERAS_SHRSUF) \
     application/application.$(CROSS).elf \
     platform/platform.$(IMPERAS_ARCH).exe

# Building Processor
NOVLNV?=1
-include $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.host

# Build semihost library
semihosting/model.$(IMPERAS_SHRSUF):
	@echo "Building Semihosting"
	$(MAKE) -C semihosting NOVLNV=$(NOVLNV)

application/application.$(CROSS).elf: application/application.c
	@echo "Building Application"
	$(MAKE) -C application CROSS=$(CROSS)
	 
platform/platform.$(IMPERAS_ARCH).exe: platform/platform.c
	@echo "Building Platform"
	$(MAKE) -C platform NOVLNV=$(NOVLNV)

clean::
	$(MAKE) -C application clean CROSS=$(CROSS)
	$(MAKE) -C platform    clean NOVLNV=$(NOVLNV)
 