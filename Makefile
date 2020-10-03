TARGETS=MHPrim PQKruskal PQPrim UFKruskal  

all: ${TARGETS} 

MHPrim: MHPrim.cpp
	g++ -g -ggdb -o $@ $^   
PQKruskal: PQKruskal.cpp
	g++ -g -ggdb -o $@ $^   
PQPrim: PQPrim.cpp
	g++ -g -ggdb -o $@ $^   
UFKruskal: UFKruskal.cpp
	g++ -g -ggdb -o $@ $^   

clean:
	rm -f *.o ${TARGETS} 
