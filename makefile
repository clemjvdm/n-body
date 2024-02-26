simulation: nbody
	gcc -o nbody main.c graphics/graphic_utils.c engine/nbody.c engine/vector.c -lSDL2 -lm
