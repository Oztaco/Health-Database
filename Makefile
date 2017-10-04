health: health.o health_util.o
	gcc -g -o health health.o health_util.o
health.o:	health.c health.h health_util.h
	gcc -g -c health.c
health_util.o:	health_util.c health.h health_util.h
	gcc -g -c health_util.c
clean:
	rm -f *.o health
run: health
	./health < testreset.txt	
