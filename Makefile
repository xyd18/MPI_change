LLVM_CONFIG=llvm-config
CXX=clang++

ifndef VERBOSE
QUIET:=@
endif

LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) -lpthread -lncurses -lz
COMMON_FLAGS=-Wall -Wextra
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR)

TARGETS= MPI_change

default: $(TARGETS)

%.o: %.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

MPI_change: MPI_change.o
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ `$(LLVM_CONFIG) --libs bitreader core support nativecodegen`

.PHONY:clean
clean:
	$(QUIET)rm -f *.o $(TARGETS)