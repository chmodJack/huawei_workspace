IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
CFLAGS=-I$(IMPERAS_LIB)/External/include/SDL
CFLAGS+=-I../../../../../ovpworld.org/modelSupport/sdlInput/1.0/model
LDFLAGS=-L$(IMPERAS_LIB)/External/lib -lSDL
include $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.host
