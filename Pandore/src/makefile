all : appUser appAdmin

LIB=../lib/
RELEASE=../pandoreRelease/
OPTIONS = -std=c++11

appUser : main.cpp
	g++ $(OPTIONS) $(LIB)* main.cpp -o $(RELEASE)Pandore

appAdmin : mainKey.cpp
	g++ $(OPTIONS) $(LIB)* mainKey.cpp -o $(RELEASE)PandoreAdmin

clean :
	cd $(RELEASE); rm -f Pandore*
