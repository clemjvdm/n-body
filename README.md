## N-body Simulation

An exploration of n-body simulations and related algorithms. I started this repository a little under a year ago (as of `02/01/2025`), and I feel I implemented some interesting algorithms and even came up with some fun solutions. However I never documented it, but now I am looking to apply for masters I have a good incentive to do so :grin:. 

So the I will start documenting, cleaning up and maybe even extending the code. Please briefly read the documentation section if you are considering my application somewhere. A list of things I am aiming to do and their progress can be found in the To-do section.

### Running the code
First ensure you have SDL2 (Simple DirectMedia Layer) installed (alongside gcc, make and git). After this you should be able to download and compile the code with the following commands:
```
git clone git@github.com:clemjvdm/n-body.git
cd n-body
make
```
Finally you should be able to run it with `./nbody`.

### Documentation

I've documented some interesting parts of the code through blog style markdown files, have a look 😄:
    - [Collision Detection](./docs/collision_detection.md)

### To-Do

1. Documentation
   1. ~~fix makefile and document how to run code~~
   2. ~~document general code structure~~
   3. ~~document different collision detection algorithms~~
   4. explain integer based arithmetic midpoint circle algorithm, and how it can be derived
   5. discuss time complexity and efficiency of all parts of code
   6. overview of implemented features
2. Clean up code
   1. ~~better inline documentation~~
   2. refactor some complex structures
3. Features
   1. QoL
      1. adjustable window size
      2. better/adjustable system initialization
   2. Optimizations
      1. implement Barnes-Hut algorithm
      2. allow for different collision detection algorithms to be used
   3. Other
      1. formally proven haskell functions core algorithms
