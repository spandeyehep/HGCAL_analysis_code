CXX     = g++

CXXFLAGS= $(shell root-config --cflags)
LIBS    = $(shell root-config --libs) 


SOURCES = HGCNtupleVariables.cc AnalyzeHGCMuons.cc 
HEADERS = HGCNtupleVariables.h AnalyzeHGCMuons.h 
OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = analyzeHGCMuons

all: $(SOURCES) $(EXECUTABLE)

%.o: %.cc
	@echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c -o $@ $< 


$(EXECUTABLE): $(OBJECTS)
	@echo "Linking $(PROGRAM) ..."
	@echo "@$(CXX) $(LIBS) $(OBJECTS) -o $@"
	@$(CXX) -o $@ $^ $(LIBS)
	@echo "done"


# Specifying the object files as intermediates deletes them automatically after the build process.
.INTERMEDIATE:  $(OBJECTS)

# The default target, which gives instructions, can be called regardless of whether or not files need to be updated.
.PHONY : clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

###
HGCNtupleVariables.o: HGCNtupleVariables.h
AnalyzeHGCMuons.o:HGCNtupleVariables.h AnalyzeHGCMuons.h
