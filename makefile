all:	gol.c
	gcc -fopenmp gol.c


run:	a.out	glid1.txt
	./a.out glid2.txt 10 output.txt


clean:	a.out output.txt
	rm a.out output.txt

