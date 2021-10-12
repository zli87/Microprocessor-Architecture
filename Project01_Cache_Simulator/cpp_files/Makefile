CC = g++
OPT = -O3
OPT = -std=c++11 #-g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC = sim_cache.cc

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim_cache.o

#################################

# default rule

all: sim_cache
	@echo "my work is done here..."


# rule for making sim_cache

sim_cache: $(SIM_OBJ)
	$(CC) -o sim_cache $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim_cache-----------"


# generic rule for converting any .cpp file to any .o file

.cc.o:
	$(CC) $(CFLAGS)  -c *.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the sim_cache binary

clean:
	rm -f *.o sim_cache


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f *.o

cc:
	$(CC) $(CFLAGS)  -c $(SIM_SRC)
	$(CC) -o sim_cache $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim_cache-----------"
run:
	./sim_cache 32 8192 4 7 65536 8 ../example_trace.txt

run_less:
	./sim_cache 32 8192 4 7 65536 8 ../example_trace.txt | less

run0: cc
	./sim_cache 16 1024 2 0 0 0 gcc_trace.txt > ../my.output0.txt

run1: cc
	./sim_cache 16 1024 2 16 0 0 gcc_trace.txt > ../my.output1.txt

run2: cc
	./sim_cache 16 1024 2 0 8192 4 gcc_trace.txt > ../my.output2.txt

run3: cc
	./sim_cache 16 1024 2 16 8192 4 gcc_trace.txt > ../my.output3.txt

run4: cc
	./sim_cache 16 1024 1 0 0 0 gcc_trace.txt > ../my.output4.txt
run5: cc
	./sim_cache 16 1024 1 16 0 0 gcc_trace.txt > ../my.output5.txt

run6: cc
	./sim_cache 16 1024 1 0 8192 4 gcc_trace.txt > ../my.output6.txt
run7: cc
	./sim_cache 16 1024 1 16 8192 4 gcc_trace.txt > ../my.output7.txt
debug:
	gdb --args ./sim_cache 16 1024 2 0 0 0 gcc_trace.txt

diff0:
	diff -iw ../my.output0.txt ../gcc.output0.txt

diff1:
	diff -iw ../my.output1.txt ../gcc.output1.txt

diff2:
	diff -iw ../my.output2.txt ../gcc.output2.txt
diff3:
	diff -iw ../my.output3.txt ../gcc.output3.txt
diff4:
	diff -iw ../my.output4.txt ../gcc.output4.txt
diff5:
	diff -iw ../my.output5.txt ../gcc.output5.txt
diff6:
	diff -iw ../my.output6.txt ../gcc.output6.txt
diff7:
	diff -iw ../my.output7.txt ../gcc.output7.txt
val1: cc
	./sim_cache 32 1536 3 0 8192 8 gcc_trace.txt > ../my.1.val.txt
vdiff1:
	diff -iw ../my.1.val.txt ../extra-runs/extra.1.val.txt
val2: cc
	./sim_cache 16 1024 64 0 0 0 gcc_trace.txt > ../my.2.val.txt
vdiff2:
	diff -iw ../my.2.val.txt ../extra-runs/extra.2.val.txt
val3: cc
	./sim_cache 16 1024 2 0 0 0 perl_trace.txt > ../my.3.val.txt
vdiff3:
	diff -iw ../my.3.val.txt ../extra-runs/extra.3.val.txt
val4: cc
	./sim_cache 16 1024 2 16 8192 4 vortex_trace.txt > ../my.4.val.txt
vdiff4:
	diff -iw ../my.4.val.txt ../extra-runs/extra.4.val.txt
val5: cc
	./sim_cache 32 512 1 7 0 0 go_trace.txt > ../my.5.val.txt
vdiff5:
	diff -iw ../my.5.val.txt ../extra-runs/extra.5.val.txt

exp1:
	run=1; l1_size=1024 ; while [[ $$l1_size -le 1048576 ]] ; do \
	    l1_assoc=1 ; while [[ $$l1_assoc -le 8 ]] ; do \
		echo "run:"$$run $$l1_size $$l1_assoc ; \
		./sim_cache 32 $$l1_size $$l1_assoc 0 0 0 gcc_trace.txt > ../exp1/my.output.$$l1_assoc.$$l1_size.txt ;\
		((l1_assoc = l1_assoc * 2)) ; \
		((run = run+1)); \
	    done ; \
	((l1_assoc = l1_size / 32)); \
	echo "run:"$$run $$l1_size $$l1_assoc "(full assoc)"; \
	./sim_cache 32 $$l1_size $$l1_assoc 0 0 0 gcc_trace.txt > ../exp1/my.output.full.$$l1_size.txt ;\
        ((l1_size = l1_size * 2 )) ; \
	((run = run+1)); \
	done
exp3:
	run=1; l1_size=1024 ; while [[ $$l1_size -le 262144 ]] ; do \
	    l1_assoc=1 ; while [[ $$l1_assoc -le 8 ]] ; do \
		echo "run:"$$run $$l1_size $$l1_assoc ; \
		./sim_cache 32 $$l1_size $$l1_assoc 0 524288 8 gcc_trace.txt > ../exp3/my.output.$$l1_assoc.$$l1_size.txt ;\
		((l1_assoc = l1_assoc * 2)) ; \
		((run = run+1)); \
	    done ; \
	((l1_assoc = l1_size / 32)); \
	echo "run:"$$run $$l1_size $$l1_assoc "(full assoc)"; \
	./sim_cache 32 $$l1_size $$l1_assoc 0 524288 8 gcc_trace.txt > ../exp3/my.output.full.$$l1_size.txt ;\
        ((l1_size = l1_size * 2 )) ; \
	((run = run+1)); \
	done
exp4:
	run=1; l1_size=1024 ; while [[ $$l1_size -le 32768 ]] ; do \
	    blk_size=16 ; while [[ $$blk_size -le 128 ]] ; do \
		echo "run:"$$run $$l1_size $$blk_size ; \
		./sim_cache $$blk_size $$l1_size 4 0 0 0 gcc_trace.txt > ../exp4/my.output.$$l1_size.$$blk_size.txt ;\
		((blk_size = blk_size * 2)) ; \
		((run = run+1)); \
	    done ; \
        ((l1_size = l1_size * 2 )) ; \
	done
exp5:
	run=1; l1_size=1024 ; while [[ $$l1_size -le 262144 ]] ; do \
	    l2_size=32768 ; while [[ $$l2_size -le 1048576 ]] ; do \
		if [ $$l2_size -gt $$l1_size ]; then \
		    echo "run:"$$run $$l1_size $$l2_size ; \
		    ./sim_cache 32 $$l1_size 4 0 $$l2_size 8 gcc_trace.txt > ../exp5/my.output.$$l2_size.$$l1_size.txt ;\
		    ((run = run+1)); \
		fi; \
		((l2_size = l2_size * 2)) ; \
	    done ; \
        ((l1_size = l1_size * 2 )) ; \
	done
exp6:
	run=1; l1_size=1024 ; while [[ $$l1_size -le 32768 ]] ; do \
	    assoc=1 ; while [[ $$assoc -le 4 ]] ; do \
		echo "run:"$$run $$l1_size $$assoc "0"; \
		./sim_cache 32 $$l1_size $$assoc 0 65536 8 gcc_trace.txt > ../exp6/my.output.$$assoc.0.$$l1_size.txt ;\
		if [ $$assoc == 1 ]; then \
		    vc_entry=2; while [[ $$vc_entry -le 16 ]] ; do \
		    echo "run:"$$run $$l1_size $$assoc $$vc_entry; \
		    ./sim_cache 32 $$l1_size $$assoc $$vc_entry 65536 8 gcc_trace.txt > ../exp6/my.output.$$assoc.$$vc_entry.$$l1_size.txt ;\
		    ((vc_entry = vc_entry * 2 )); \
		    ((run = run+1)); \
		    done ; \
		fi; \
		((assoc = assoc * 2)) ; \
		((run = run+1)); \
	    done ; \
        ((l1_size = l1_size * 2 )) ; \
	done
