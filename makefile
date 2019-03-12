CC=gcc

all: 	gol.c
	$(CC) -fopenmp gol.c


runglid1100:	a.out glid1.txt
		./a.out glid1.txt 100 output.txt

runglid11000:	a.out glid1.txt
		./a.out glid1.txt 1000 output.txt

runglid2100:	a.out glid2.txt
		./a.out glid2.txt 100 output.txt

runglid21000: 	a.out glid2.txt
		./a.out glid2.txt 1000 output.txt

clean:	a.out output.txt
	rm a.out output.txt

