thewatchman: src/logger.h src/tools.h src/logger.cpp src/thewatchman.cpp
	rm -rf bin
	cd lib/curl && make && sudo make install && cd ../..
	mkdir -p bin
	g++ -Wall -std=c++11 -lcurl -o bin/thewatchman src/*.h src/*.cpp
install:
	cp -riv bin/* /usr/bin

run:
	thewatchman
