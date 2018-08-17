CXX     = g++
CXXFLAGS = -m64 -pipe -W -Wall -Woverloaded-virtual \
           -fsigned-char -fno-common -std=c++11 -I$(ROOTSYS)/include
INCLUDE = $(shell root-config --cflags)
LIBS    = $(shell root-config --libs) 

LD            = g++
LDFLAGS       = -O2 -m64 

SOURCES = HGCNtupleVariables.cc AnalyzeHGCMuons.cc 
HEADERS = HGCNtupleVariables.h AnalyzeHGCMuons.h 
OBJECTS = $(SOURCES:.cc=.o)
#OBJS          = HGCNtupleVariables.o AnalyzeTTJets.o 

EXECUTABLE = analyzeHGCMuons

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking $(PROGRAM) ..."
	@echo "@$(CXX) $(LIBS) $(OBJECTS) -o $@"
	@$(CXX) $(LIBS) $(OBJECTS) -o $@
	@echo "done"

%.o: %.cc
	@echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c -o $@ $< 


# Specifying the object files as intermediates deletes them automatically after the build process.
.INTERMEDIATE:  $(OBJECTS)

# The default target, which gives instructions, can be called regardless of whether or not files need to be updated.
.PHONY : clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

###
HGCNtupleVariables.o: HGCNtupleVariables.h
AnalyzeHGCMuons.o:HGCNtupleVariables.h AnalyzeHGCMuons.h
