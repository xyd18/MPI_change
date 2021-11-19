CHANGE=../MPI_change

# Path
C_SRC_DIR := ./c_bench
C_BUILD_DIR := ./c_build
# C_OBJ_DIR := $(C_BUILD_DIR)/obj

C_SRCS := $(wildcard $(C_SRC_DIR)/*.c)
C_BCC := $(C_SRCS:$(C_SRC_DIR)/%.c=$(C_BUILD_DIR)/%.bcc)

# original makefile
default: test_orig

test_orig: test.cpp
	clang++ -I /usr/include/mpi test.cpp -o test
# 	clang++ -I /usr/include/mpi -emit-llvm -c test.cpp -o test.bc
# 	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

# MPI_change makefile
change: test_change test_struct_change $(C_BCC)
	@echo "\033[36mfinished\033[0m"

$(C_BUILD_DIR)/%.bcc: $(C_BUILD_DIR)/%.bc
	@mkdir -p $(C_BUILD_DIR)
	$(CHANGE) $^ > $@ 2>$@_stderr.txt

$(C_BUILD_DIR)/%.bc: $(C_SRC_DIR)/%.c
	@mkdir -p $(C_BUILD_DIR)
	clang -I /usr/include/mpi -emit-llvm -c $^ -o $@
	clang -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll



test_struct_change: test_struct.s my_send.s mem_manager.s
	clang++ -I /usr/include/mpi -lrt -lpthread $^ -o $@

test_change: test.s my_send.s mem_manager.s
	clang++ -I /usr/include/mpi -lrt -lpthread $^ -o $@

%.s: %.bcc
	llc $^ -o $@
	llvm-dis $^ -o $^_after.ll

%.bcc: %.bc
	$(CHANGE) $^ > $@ 2>$@_stderr.txt


%.bc: %.cpp
	clang++ -I /usr/include/mpi -emit-llvm -c $^ -o $@
	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

# cleaning
.PHONY: clean
clean:
	rm -f *.o *.ll *.bc *.bcc *.s
	rm -rf $(C_BUILD_DIR)