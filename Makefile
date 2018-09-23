all: test

clean:
	rm -f example.hpp example.pb.h example.pb.cc example.proto *.out libcluon_example protobuf_example cluon-msc cluon-complete.cpp timing.* lcd lce pbd pbe chart.pdf log

cluon-complete.cpp:
	ln -sf cluon-complete.hpp cluon-complete.cpp

cluon-msc: cluon-complete.cpp
	g++ -O2 -DHAVE_CLUON_MSC -std=c++14 -pthread -o cluon-msc cluon-complete.cpp

example.hpp: cluon-msc example.odvd
	./cluon-msc --cpp --out=example.hpp example.odvd

example.pb.cc: cluon-msc example.odvd
	./cluon-msc --proto --out=example.proto example.odvd
	protoc --cpp_out=. example.proto

libcluon_example: libcluon_example.cpp cluon-complete.hpp example.hpp
	g++ -O2 -std=c++14 -pthread -static -o libcluon_example -I. libcluon_example.cpp
	strip -s libcluon_example

protobuf_example: protobuf_example.cpp example.pb.cc
	g++ -O2 -std=c++14 -pthread -static -o protobuf_example protobuf_example.cpp example.pb.cc -l protobuf
	strip -s protobuf_example

test: libcluon_example protobuf_example
	@./libcluon_example 2>&1 > /dev/null
	@./protobuf_example 2>&1 > /dev/null
	@cmp -l libcluon.out protobuf.out || echo "Output is not identical."
	@for i in `seq 1 100`; do nice -n -20 ./libcluon_example | grep "took" >> timing.libcluon; done
	@for i in `seq 1 100`; do nice -n -20 ./protobuf_example | grep "took" >> timing.protobuf; done
	@echo -n "libcluon encoding:" && cat timing.libcluon |grep "Encod"|cut -f3 -d" "|num median
	@echo -n "libcluon decoding:" && cat timing.libcluon |grep "Decod"|cut -f3 -d" "|num median
	@echo -n "Protobuf encoding:" && cat timing.protobuf |grep "Encod"|cut -f3 -d" "|num median
	@echo -n "Protobuf decoding:" && cat timing.protobuf |grep "Decod"|cut -f3 -d" "|num median

test100: libcluon_example protobuf_example
	@./test100.sh

