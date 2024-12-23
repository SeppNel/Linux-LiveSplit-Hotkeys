all: *.cpp
	mkdir -p build
	g++ -O2 -o build/linux-live-split-hotkeys *.cpp -lyaml-cpp

run: all
	sudo ./build/linux-live-split-hotkeys

clean:
	rm -R build