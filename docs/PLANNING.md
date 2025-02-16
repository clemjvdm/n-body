## Planning

Here I will write notes on what and how I might contribute to this project.

## Timeline

1. Wrap up the core documentation of the code (1-4 in README.md todo's)
2. Better collision detection
    1. ~~Elastic collisions~~
    2. Inelastic collisions
    3. Investigate
        1. Lazy collision updates (only check for collisions with points which have previously collided in movement recursion)
        2. Spatial partitioning (Spatial grid, R-tree)
        3. Sweep and prune (AABB)
    4. Implement ideal collision detection methods
    5. Proofs for interesting/critical functions
3. Implement barnes hut
4. GPU optimization

## Resources

https://youtu.be/fdZfddO7YTs?feature=shared
https://realtimecollisiondetection.net/books/rtcd/



## IDEAS FOR COLLISION SYSTEM:
1. collision happens when DIST(a, b) < GRID_SIZE
2. maybe use MPFR for critical parts
3. caching some solutions for equations which will be repeated (parts of TOC which will be the same given radius for example).
4. if it's too expensive for it to actually be robust, exploring where errors can come from and what their probability might be
