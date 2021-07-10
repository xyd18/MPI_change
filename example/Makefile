CHANGE=../MPI_change

# original makefile
default: test_orig

test_orig: test.cpp
	clang++ -I /usr/include/mpi test.cpp -o test
# 	clang++ -I /usr/include/mpi -emit-llvm -c test.cpp -o test.bc
# 	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

# MPI_change makefile
change: test_change test_struct_change
	@echo "finished"

test_change: test.bcc

test_struct_change: test_struct.bcc
# test_change: test.s
# 	clang++ -I /usr/include/mpi -lrt -lpthread $^ -o $@

# %.s: %.bcc
# 	llc $^ -o $@
# 	llvm-dis $^ -o $^.ll

%.bcc: %.bc
	$(CHANGE) $^ > $@ 2>$@_stderr.txt


%.bc: %.cpp
	clang++ -I /usr/include/mpi -emit-llvm -c $^ -o $@
	clang++ -I /usr/include/mpi -emit-llvm -S $^ -o $@.ll

# cleaning
.PHONY: clean
clean:
	rm -f *.o *.ll *.bc *.bcc *.s