# LLVM_CONFIG=llvm-config
LLVM_CONFIG=/usr/lib/llvm-10/bin/llvm-config


CXX=clang++

# VERBOSE = true

ifndef VERBOSE
QUIET:=@
endif

LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) -lpthread -lncurses -lz -ldl
COMMON_FLAGS=-Wall -Wextra -Wno-unused-parameter
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags)
LIBS+=$(shell $(LLVM_CONFIG) --libs bitreader core support nativecodegen)



TARGETS= MPI_change

default: $(TARGETS)

# temp.s: temp.cpp
# 	@echo "\033[36mgenerate temp.s\033[0m"
# 	$(QUIET)clang++ -I /usr/include/mpi -I /usr/include/boost -I /usr/local/include -S $^ -o $@


%.o: %.cpp
	@echo "\033[36mCompiling $*.cpp\033[0m"
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

MPI_change: MPI_change.o
	@echo "\033[36mLinking $@\033[0m"
	$(QUIET)$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS) $(LIBS)

.PHONY:clean
clean:
	$(QUIET)rm -f *.o $(TARGETS)
