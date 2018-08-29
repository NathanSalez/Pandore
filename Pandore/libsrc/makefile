all : Exception.o String.o Combination.o CombinationList.o libFile.o libUtils.o

LIB=../lib/
OPTIONS = -std=c++11

Exception.o : Exception.cpp
	g++ -c $(OPTIONS) Exception.cpp -o $(LIB)Exception.o

String.o : String.cpp
	g++ -c $(OPTIONS) String.cpp -o $(LIB)String.o

Combination.o : Combination.cpp
	g++ -c $(OPTIONS) Combination.cpp -o $(LIB)Combination.o

CombinationList.o : CombinationList.cpp
	g++ -c $(OPTIONS) CombinationList.cpp -o $(LIB)CombinationList.o

libFile.o : libFile.cpp
	g++ -c $(OPTIONS) libFile.cpp -o $(LIB)libFile.o

libUtils.o : libUtils.cpp
	g++ -c $(OPTIONS) libUtils.cpp -o $(LIB)libUtils.o

clean :
	rm -f $(LIB)/*
