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

# Path
MY_SEND_SRC_DIR := ./my_send
MY_SEND_BUILD_DIR := ./my_send_build

MY_SEND_SRCS := $(wildcard $(MY_SEND_SRC_DIR)/*.cpp)
MY_SEND_S := $(MY_SEND_SRCS:$(MY_SEND_SRC_DIR)/%.cpp=$(MY_SEND_BUILD_DIR)/%.s)

TARGETS= $(MY_SEND_S) MPI_change

default: $(TARGETS)

# temp.s: temp.cpp
# 	@echo "\033[36mgenerate temp.s\033[0m"
# 	$(QUIET)clang++ -I /usr/include/mpi -I /usr/include/boost -I /usr/local/include -S $^ -o $@

$(MY_SEND_BUILD_DIR)/%.s: $(MY_SEND_SRC_DIR)/%.cpp
	@mkdir -p $(MY_SEND_BUILD_DIR)
	@echo "\033[36mgenerate $*.s\033[0m"
	# $(QUIET)clang++ -I /usr/include/mpi -I /usr/include/boost -I /usr/local/include -S $^ -o $@
	$(QUIET)clang++ -I /usr/include/x86_64-linux-gnu/mpich/ -I /usr/include/boost -I /usr/local/include -S $^ -o $@
	# mv $(MY_SEND_BUILD_DIR)/$*.s ./example/$*.s

%.o: %.cpp
	@echo "\033[36mCompiling $*.cpp\033[0m"
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

MPI_change: MPI_change.o
	@echo "\033[36mLinking $@\033[0m"
	$(QUIET)$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS) $(LIBS)

.PHONY:clean
clean:
	$(QUIET)rm -f *.o $(TARGETS)
