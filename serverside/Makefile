thewatchman: src/logger.h src/tools.h src/logger.cpp src/thewatchman.cpp
	rm -rf bin
	mkdir -p bin
	g++ -Wall -std=c++11 -o bin/thewatchman src/*.h src/*.cpp
	# g++ -std=c++11 -Wall -Werror -Ilib/curl/include -o bin/thewatchman src/*.h src/*.cpp -Llib/curl -lcurl -lrtmp -lssh2
install:
	cp -riv bin/* /usr/bin

run:
	thewatchman
