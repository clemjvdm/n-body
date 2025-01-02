simulation: engine/nbody.c engine/nbody.h engine/vector.c engine/vector.h graphics/graphic_utils.c graphics/graphic_utils.h
	gcc -o nbody main.c engine/nbody.c graphics/graphic_utils.c engine/vector.c -lSDL2 -lm

