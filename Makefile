# Makefile
#lists programs to be compiled
all:	ex1

#compiles exercise 1 
ex1:	ex1.c
	gcc -o ex1 ex1.c
#----------------------------------------------------------------------------
clean:
	rm -f ex1 # remove the executable ex1 file
	rm -f *.o  # remove ALL object files in the directory

