CC = g++
OPT = -O3
OPT = -std=c++11 #-g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)
VALDIR = ../val
OUTDIR = ../out
#TRADIR = ../trace/
# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC = sim_bp.cc

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim_bp.o

#################################

# default rule

all: sim_bp
	@echo "my work is done here..."


# rule for making sim_bp

sim_bp: $(SIM_OBJ)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim_bp-----------"


# generic rule for converting any .cpp file to any .o file

.cc.o:
	$(CC) $(CFLAGS)  -c *.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the sim_bp binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim_bp binary)

clobber:
	rm -f *.o

cc:
	$(CC) $(CFLAGS)  -c $(SIM_SRC)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim_bp-----------"

run: clean cc
	./sim gshare 9 3 $(TRADIR)gcc_trace.txt
run0: clean cc
	./sim gshare 9 3 $(TRADIR)gcc_trace.txt

valb1: clean cc
	./sim bimodal 6 $(TRADIR)gcc_trace.txt  > $(OUTDIR)/my_bimodal_1.txt
	diff -iw $(OUTDIR)/my_bimodal_1.txt $(VALDIR)/val_bimodal_1.txt
valb2: clean cc
	./sim bimodal 12 $(TRADIR)gcc_trace.txt  > $(OUTDIR)/my_bimodal_2.txt
	diff -iw $(OUTDIR)/my_bimodal_2.txt $(VALDIR)/val_bimodal_2.txt
valb3: clean cc
	./sim bimodal 4 $(TRADIR)jpeg_trace.txt  > $(OUTDIR)/my_bimodal_3.txt
	diff -iw $(OUTDIR)/my_bimodal_3.txt $(VALDIR)/val_bimodal_3.txt
valb4: clean cc
	./sim bimodal 5 $(TRADIR)perl_trace.txt  > $(OUTDIR)/my_bimodal_4.txt
	diff -iw $(OUTDIR)/my_bimodal_4.txt $(VALDIR)/val_bimodal_4.txt
valg1: clean cc
	./sim gshare 9 3 $(TRADIR)gcc_trace.txt  > $(OUTDIR)/my_gshare_1.txt
	diff -iw $(OUTDIR)/my_gshare_1.txt $(VALDIR)/val_gshare_1.txt
valg2: clean cc
	./sim gshare 14 8 $(TRADIR)gcc_trace.txt  > $(OUTDIR)/my_gshare_2.txt
	diff -iw $(OUTDIR)/my_gshare_2.txt $(VALDIR)/val_gshare_2.txt
valg3: clean cc
	./sim gshare 11 5 $(TRADIR)jpeg_trace.txt  > $(OUTDIR)/my_gshare_3.txt
	diff -iw $(OUTDIR)/my_gshare_3.txt $(VALDIR)/val_gshare_3.txt
valg4: clean cc
	./sim gshare 10 6 $(TRADIR)perl_trace.txt  > $(OUTDIR)/my_gshare_4.txt
	diff -iw $(OUTDIR)/my_gshare_4.txt $(VALDIR)/val_gshare_4.txt
valh1: clean cc
	./sim hybrid 8 14 10 5 $(TRADIR)gcc_trace.txt  > $(OUTDIR)/my_hybrid_1.txt
	diff -iw $(OUTDIR)/my_hybrid_1.txt $(VALDIR)/val_hybrid_1.txt
valh2: cc
	./sim hybrid 5 10 7 5 $(TRADIR)jpeg_trace.txt  > $(OUTDIR)/my_hybrid_2.txt
	diff -iw $(OUTDIR)/my_hybrid_2.txt $(VALDIR)/val_hybrid_2.txt

exp1:
	rm -rf ../exp1/*
	trace=0 ; while [[ $$trace -le 2 ]] ; do \
		run=1; m=7 ; while [[ $$m -le 20 ]] ; do \
				if [ $$trace == 0 ]; then \
					echo "gcc run:"$$run $$m ; \
				    ./sim bimodal $$m gcc_trace.txt >> ../exp1/gcc.txt ;\
				    ((run = run+1)); \
				fi; \
				if [ $$trace == 1 ]; then \
					echo "jpeg run:"$$run $$m ; \
					./sim bimodal $$m jpeg_trace.txt >> ../exp1/jpeg.txt ;\
					((run = run+1)); \
				fi; \
				if [ $$trace == 2 ]; then \
					echo "perl run:"$$run $$m ; \
					./sim bimodal $$m perl_trace.txt >> ../exp1/perl.txt ;\
					((run = run+1)); \
				fi; \
	        ((m = m +1 )) ; \
		done ; \
		((trace = trace +1 )) ; \
	done

exp2:
	rm -rf ../exp2/*
	trace=0 ; while [[ $$trace -le 2 ]] ; do \
		run=1; m=7 ; while [[ $$m -le 20 ]] ; do \
		    n=0 ; while [[ $$n -le $$m ]] ; do \
				if [ $$trace == 0 ]; then \
					echo "gcc run:"$$run $$m $$n ; \
				    ./sim gshare $$m $$n gcc_trace.txt >> ../exp2/gcc.m.$$m.txt ;\
				    ((run = run+1)); \
				fi; \
				if [ $$trace == 1 ]; then \
					echo "jpeg run:"$$run $$m $$n ; \
					./sim gshare $$m $$n jpeg_trace.txt >> ../exp2/jpeg.m.$$m.txt ;\
					((run = run+1)); \
				fi; \
				if [ $$trace == 2 ]; then \
					echo "perl run:"$$run $$m $$n ; \
					./sim gshare $$m $$n perl_trace.txt >> ../exp2/perl.m.$$m.txt ;\
					((run = run+1)); \
				fi; \
				((n = n +1)) ; \
		    done ; \
	        ((m = m +1 )) ; \
		done ; \
		((trace = trace +1 )) ; \
	done
