all: *.cpp
	mkdir -p build
	g++ -O2 -o build/liveSplit-hotkeys *.cpp -lyaml-cpp

run: all
	sudo ./build/liveSplit-hotkeys

debug: *.cpp
	mkdir -p build
	g++ -g -o build/liveSplit-hotkeys-debug *.cpp -lyaml-cpp

static: *.cpp
	mkdir -p build
	g++ -O2 -o build/liveSplit-hotkeys *.cpp -lyaml-cpp -static

clean:
	rm -R build