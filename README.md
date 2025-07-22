
![Logo](/imgs/logo.png)


# CPHYS

CPHYS is a lightweight physics simulation program made in C using [CSFML library](https://github.com/SFML/CSFML). Designed to model and visualize collisions between physical bodies in a two-dimensional space, it provides users with a simplified yet realistic environment in which they can study the fundamentals of classical mechanics, specifically the behavior of objects during collisions.

At its core, CPHYS simulates interactions between rigid bodies using classical Newtonian physics principles. Users can define the initial conditions for multiple bodies, such as their position, velocity and mass. The program then calculates their trajectories and interactions over time, resolving collisions using well-established algorithms based on conservation laws.


## Demo

![rball](/imgs/rubberball.gif)
![mad](/imgs/madness.gif)


## Features

- Up to 20 simulated bodies.
- Each body can be defined by user or randomized. 
- Simulation is set up using configuration files making the process of setting up very fast.
- Customizable environment (coefficient of restitution, gravity).
- User controled visualisation (pause, restart).
- A couple of pre defined example simulation scenarios,


## Optimizations

The physics engine was carefully optimized, to achieve high performance, mainly by using branchelss programming and by choice of physics formulas.

CPHYS is also using high efficency mt19937 algorithm for generating random numbers (for setting up simulations, of course c: )


## Installation

You have 2 choices:

- ### Pre Complied Release Package
Download pre compiled package for your system from "Realeses" tab of this repo, then unpack to folder of your choice... CPHYS is ready to go! (since binaries are not signed, if you are on windows machine, the system will warn you of not trusted software. Just go to advanced settings and ignore it)

- ### Since its cross platform and open source - Build it yourself!
If you are on linux, make sure to build or install [CSFML](https://github.com/SFML/CSFML) first, then download this repo and run provided makefile.

```bash
  cd CPhys
  make all
  ./bin/Cphys
```

On every other system you will have to manually build or install [CSFML](https://github.com/SFML/CSFML) and then using it as dependency for this project, you'll have to build it using build system of your choice. If you are unsure what to do, just download pre compiled package (not available for Mac).
## Usage/Examples

### Keybinds
- **Q**, **ESC** - Exit Program
- **SPACEBAR** - Pause/Unpause simulation
- **R** - Restart simulation

### Configuration Files
CPHYS's starting state is set up using configuration files. There are a couple of ready to go predefined simulation files for different scenarios stored in "examples" folder. If you want to use certain file, copy it to main directory and (re)name it "config.cinit" and then just start the application, or optionally you can run the program from cli and specify name (path) of the file you want to use.

I.E for linux:
```bash
./bin/Cphys examples/RubberBall.cinit
```

### Writting own scenarios
If you want to write your own simulation scenario, just check how the example files look like. This system is really simple c:


## License

[MIT](https://choosealicense.com/licenses/mit/)


## Acknowledgements

 - [CSFML](https://github.com/SFML/CSFML) - Graphics library
 - [Sultanik's mt19937 implementation](https://github.com/ESultanik/mtwister) - implementation of mt19937 RNG algorithm used in this project

## Known Issues
- RNG ranges are not working properly
- Slight numerical instablility