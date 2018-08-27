all : Exception.o String.o Combination.o File.o CombinationList.o 

LIB=../lib/
OPTIONS = -std=c++11

Exception.o : Exception.cpp
	g++ -c $(OPTIONS) Exception.cpp -o $(LIB)Exception.o

String.o : String.cpp
	g++ -c $(OPTIONS) String.cpp -o $(LIB)String.o

Combination.o : Combination.cpp
	g++ -c $(OPTIONS) Combination.cpp -o $(LIB)Combination.o

File.o : File.cpp
	g++ -c $(OPTIONS) File.cpp -o $(LIB)File.o

CombinationList.o : CombinationList.cpp
	g++ -c $(OPTIONS) CombinationList.cpp -o $(LIB)CombinationList.o

clean :
	rm -f $(LIB)/*
