CHANGE=../MPI_change

# Path
MY_SEND_SRC_DIR := ./my_send
MY_SEND_BUILD_DIR := ./my_send_build

MY_SEND_SRCS := $(wildcard $(MY_SEND_SRC_DIR)/*.cpp)
MY_SEND_S := $(MY_SEND_SRCS:$(MY_SEND_SRC_DIR)/%.cpp=$(MY_SEND_BUILD_DIR)/%.s)

# Path
C_SRC_DIR := ./c_bench
C_BUILD_DIR := ./c_build
C_OBJ_DIR := $(C_BUILD_DIR)/obj

C_SRCS := $(wildcard $(C_SRC_DIR)/*.c)
C_BCC := $(C_SRCS:$(C_SRC_DIR)/%.c=$(C_BUILD_DIR)/%.bcc)
C_S := $(C_SRCS:$(C_SRC_DIR)/%.c=$(C_BUILD_DIR)/%.s)
C_TARGET := $(C_SRCS:$(C_SRC_DIR)/%.c=$(C_OBJ_DIR)/%.out)

# original makefile
default: test_orig

test_orig: test.cpp
	clang++ -I /usr/include/mpi test.cpp -o test
# 	clang++ -I /usr/include/mpi -emit-llvm -c test.cpp -o test.bc
# 	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

# MPI_change makefile
change: $(MY_SEND_S) ping_pong test_change test_struct_change $(C_TARGET)
	@echo "\033[36mfinished\033[0m"

$(MY_SEND_BUILD_DIR)/%.s: $(MY_SEND_SRC_DIR)/%.cpp
	@mkdir -p $(MY_SEND_BUILD_DIR)
	@echo "\033[36mgenerate $*.s\033[0m"
	$(QUIET)clang++ -I /usr/include/mpi -I /usr/include/boost -I /usr/local/include -S $^ -o $@

$(C_BUILD_DIR)/%.bcc: $(C_BUILD_DIR)/%.bc
	@mkdir -p $(C_BUILD_DIR)
	$(CHANGE) $^ > $@ 2>$@_stderr.txt

$(C_BUILD_DIR)/%.bc: $(C_SRC_DIR)/%.c
	@mkdir -p $(C_BUILD_DIR)
	clang -I /usr/include/mpi -emit-llvm -c $^ -o $@
	clang -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

test_struct_change: test_struct.s $(wildcard $(MY_SEND_BUILD_DIR)/*.s)
	clang++ -I /usr/include/mpi -lrt -lpthread -lmpi $^ -o $@

test_change: test.s $(wildcard $(MY_SEND_BUILD_DIR)/*.s)
	clang++ -I /usr/include/mpi -lrt -lpthread -lmpi $^ -o $@

ping_pong: ping_pong.s $(wildcard $(MY_SEND_BUILD_DIR)/*.s)
	clang++ -I /usr/include/mpi -lrt -lpthread -lmpi $^ -o $@

%.o: %.s
	clang++ -I /usr/include/mpi -c  $^

%.s: %.bcc
	llc $^ -o $@
	llvm-dis $^ -o $*_after.ll

%.bcc: %.bc
	$(CHANGE) $^ > $@ 2>$*_stderr.txt

%.bc: %.cpp
	clang++ -I /usr/include/mpi -emit-llvm -c $^ -o $@
	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $*.ll

# cleaning
.PHONY: clean
clean:
	rm -f *.o *.ll *.bc *.bcc *.s *.txt
	rm -rf $(C_BUILD_DIR)