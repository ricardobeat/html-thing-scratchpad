
LOCAL_LIBS=$(shell find libs -name lib -type d | xargs -Ix echo -Lx)
LOCAL_INCLUDES=$(shell find libs -name include -type d | xargs -Ix echo -Ix)

INCLUDES=\
	$(LOCAL_INCLUDES) \
	-I/usr/local/include

LIBS=\
	$(LOCAL_LIBS) \
	-framework OpenGL \
	-lsfml-graphics \
	-lsfml-system \
	-lsfml-window \
	-levent \
	-lgumbo \
	-lyogacore

build: thing.cpp
	g++ -o thing -std=c++11 $(INCLUDES) $(LIBS) thing.cpp

sdl: sdl-thing.cpp
	g++ -o sdl-thing -std=c++11 $(LIBS) $(INCLUDES) -lSDL2 sdl-thing.cpp

glut: gluttest.cpp
	g++ -o gluttest -std=c++11 $(INCLUDES) $(LIBS) gluttest.cpp

watch:
	make build || true
	fswatch -l0.5 *.cpp | xargs -n1 sh -c "make run"

run: build
	./thing teste.html

testyoga: testyoga.c
	gcc -I/usr/local/include -framework OpenGL -lyogacore -lnanovg -lglfw testyoga.c -o testyoga

fltk: testfltk.c
	g++ -std=c++11 -I/usr/local/include -framework OpenGL -lfltk testfltk.c -o testfltk