## Planning

Here I will write notes on what and how I might contribute to this project.

## Timeline

1. Wrap up the core documentation of the code (1-4 in README.md todo's)
2. Better collision detection
    1. Elastic collisions
    2. Investigate
        1. Lazy collision updates (only check for collisions with points which have previously collided in movement recursion)
        2. Spatial partitioning (Spatial grid, R-tree)
        3. Sweep and prune (AABB)
    3. Implement ideal collision detection methods
    4. Proofs for interesting/critical functions
3. Implement barnes hut
4. GPU optimization
