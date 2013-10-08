# --------------------------------------------------------------
# GNUmakefile for polymerFim (layeredPolymerTracking)
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/8/2012
# --------------------------------------------------------------
ifdef G4MPIUSE
  name := mpiPolymerFilm
  G4TARGET := $(name)
  G4EXLIB := true
  CPPFLAGS += -I$(MPI_INCLUDE)  -DG4MPIUSE=1
else
  name := polymerFilm
  G4TARGET := $(name)
  G4EXLIB := true
endif

CPPFLAGS += -g

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all clean
all: lib bin

install:
	@cp ${G4WORKDIR}/bin/${G4SYSTEM}/${G4TARGET} .

#### G4ANALYSIS_USE := true

include $(G4INSTALL)/config/binmake.gmk
ifdef G4MPIUSE
  include /home/murffer/MPI/mpi_interface/G4MPI.gmk
  LDLIBS1 += -lG4UImpi
endif

histclean:
	rm ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/HistoManager.o

ifdef ROOTSYS
  CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
  CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
  LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)

   ifeq ($(G4SYSTEM),Linux-g++)
     LDLIBS += -Wl,-rpath,$(shell $(ROOTSYS)/bin/root-config --libdir)
   endif
endif
