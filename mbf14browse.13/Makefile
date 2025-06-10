mbf14browse: mbf14browse.o fastlist.o mbf16c.o time_ms.o SFMT.o rnd.o
	g++ -O3 -DSFMT_MEXP=19937 mbf14browse.o fastlist.o mbf16c.o time_ms.o SFMT.o rnd.o -o mbf14browse
mbf14browse.o:
	g++ -c -O3 -DSFMT_MEXP=19937 mbf14browse.cpp
fastlist.o: 
	g++ -c -O3 -DSFMT_MEXP=19937 fastlist.cpp
mbf16c.o: 
	g++ -c -O3 -DSFMT_MEXP=19937 mbf16c.cpp
time_ms.o:
	g++ -c -O3 -DSFMT_MEXP=19937 time_ms.cpp
SFMT.o:
	gcc -c -O3 -DSFMT_MEXP=19937 SFMT.c
rnd.o:
	g++ -c -DSFMT_MEXP=19937 rnd.cpp
clean:
	rm -rf *.o mbf14browse
