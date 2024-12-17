LIBRARIES=-lGLEW -lGL -lglfw

debug: clean build/main

release: clean build/main

build:
	mkdir build

build/obj: build
	mkdir build/obj

DEBUG=
OBJECT_FILES=build/obj/index_buffer
build/main: build build/obj/main.o build/obj/renderer.o build/obj/vertex_buffer.o build/obj/index_buffer.o build/obj/shader_loader.o
	g++ build/obj/main.o build/obj/renderer.o build/obj/vertex_buffer.o build/obj/index_buffer.o build/obj/shader_loader.o -o build/main $(LIBRARIES) $(DEBUG)

build/obj/main.o: build/obj
	g++ -c src/main.cpp -o build/obj/main.o $(DEBUG)

build/obj/renderer.o: build/obj
	g++ -c src/renderer.cpp -o build/obj/renderer.o $(DEBUG)

build/obj/vertex_buffer.o: build/obj
	g++ -c src/vertex_buffer.cpp -o build/obj/vertex_buffer.o $(DEBUG)

build/obj/index_buffer.o: build/obj
	g++ -c src/index_buffer.cpp -o build/obj/index_buffer.o $(DEBUG)

build/obj/shader_loader.o: build/obj
	g++ -c src/shader_loader.cpp -o build/obj/shader_loader.o $(DEBUG)

clean:
	rm -rf build
