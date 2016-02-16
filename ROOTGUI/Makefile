#----------------------------------------------------#
# Craig Reingold, Dec 05, 2015, PHYS60070                #
# Makefile for C++ codes with ROOT libraries  #
#----------------------------------------------------#
 
# NOTE, FOR THE NEXT 3 LINES TO WORK YOU HAVE TO 
# HAVE $(ROOTSYS)/bin in your PATH (load ROOT module first!)
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -O2 -fPIC -g $(ROOTCFLAGS)
# -Wall
LD            = g++
LDFLAGS       = -O2
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#----------------------------------------------------#
# Here is where you define the files that will be used 
# and the output file that will be created.
FILES          = rangeMover.cpp 
PROGRAMS      = rangeMover
OBJS := $(FILES:.cpp=.o)
#----------------------------------------------------#

$(PROGRAMS):     $(OBJS)
		$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
		@echo "$@ is ready!"

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		@rm -f $(OBJS)
		@rm -f $(PROGRAMS)
