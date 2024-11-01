# PFL
PR00F Foundation Library

Collection of some useful routines (file system, math, bit manipulation, etc.) used across my projects. Compiles into static lib.

This project is unit-tested, however the tests are in the [repository of PGE](https://github.com/proof88/PGE/blob/master/PGE/UnitTests/PFLTest.h).

The Visual Studio project file is included.<br/>
However, if you want to see example of integration in other projects, you may be interested in the Visual Studio solution including other projects as well in [PGE-misc](https://github.com/proof88/PGE-misc) repo.

Note that it is highly recommended that you use [Boost lib](https://www.boost.org/) instead of my library.  
Boost has been existing for long time, and nowadays it is very easy to configure your project with it.  
You don't even need to checkout and build the entire Boost lib with your project: [it is nicely distributed into components, and components can be referenced in your CMakefile](https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake).
