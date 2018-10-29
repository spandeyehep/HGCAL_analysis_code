CXX     = g++

CXXFLAGS= $(shell root-config --cflags)
LIBS    = $(shell root-config --libs) 


SOURCES = RecHit.cc KDTree.cc Cluster2D.cc HGCNtupleVariables.cc AnalyzeHGCOctTB.cc 
HEADERS = RecHit.h Cluster2D.h KDTree.h HGCNtupleVariables.h AnalyzeHGCOctTB.h
OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = analyzeHGCOctTB

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
RecHit.o: RecHit.h
Cluster2D.o: Cluster2D.h
KDTree.o: KDTree.h
HGCNtupleVariables.o: HGCNtupleVariables.h
AnalyzeHGCOctTB.o: RecHit.h Cluster2D.h KDTree.h HGCNtupleVariables.h AnalyzeHGCOctTB.h
