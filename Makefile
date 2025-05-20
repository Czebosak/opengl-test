LIBRARIES=-lGLEW -lGL -lglfw

debug: clean build/main

release: clean build/main

build:
	mkdir build

build/obj: build
	mkdir build/obj

DEBUG=
OBJECT_FILES=build/obj/index_buffer
build/main: build build/obj/main.o build/obj/renderer.o build/obj/vertex_buffer.o build/obj/index_buffer.o build/obj/shader.o build/obj/vertex_array.o
	g++ build/obj/main.o build/obj/renderer.o build/obj/vertex_buffer.o build/obj/index_buffer.o build/obj/shader.o build/obj/vertex_array.o -o build/main $(LIBRARIES) $(DEBUG)

build/obj/main.o: build/obj
	g++ -c src/main.cpp -o build/obj/main.o $(DEBUG)

build/obj/renderer.o: build/obj
	g++ -c src/renderer.cpp -o build/obj/renderer.o $(DEBUG)

build/obj/vertex_buffer.o: build/obj
	g++ -c src/vertex_buffer.cpp -o build/obj/vertex_buffer.o $(DEBUG)

build/obj/index_buffer.o: build/obj
	g++ -c src/index_buffer.cpp -o build/obj/index_buffer.o $(DEBUG)

build/obj/shader.o: build/obj
	g++ -c src/shader.cpp -o build/obj/shader.o $(DEBUG)

build/obj/vertex_array.o: build/obj
	g++ -c src/vertex_array.cpp -o build/obj/vertex_array.o -std=c++23

clean:
	rm -rf build
