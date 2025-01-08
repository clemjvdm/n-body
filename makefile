simulation: engine/simulation.c engine/simulation.h engine/vector.c engine/vector.h engine/system.c engine/system.h engine/AABB.h engine/AABB.c graphics/graphic_utils.c graphics/graphic_utils.h
	gcc -o nbody main.c engine/simulation.c engine/system.c engine/AABB.c graphics/graphic_utils.c engine/vector.c -lSDL2 -lm -g
