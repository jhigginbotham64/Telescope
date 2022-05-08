# TODO List

This file should not be merged into a publicly available repo. All todos listed here should be resolved first

## Building the Docs

To build the docs, make sure Sphinx 4.0+ is installed, earlier sphinx versions had a bug that makes it unable to work in Telescopes environment.

To build the docs on unix, just run the bash script `docs/rebuild_docs.sh`. Idk windows powershell but its only a few lines, it should be easy to port. Basically, to rebuild the docs:

1. Update any `.rst` file in `/docs`
2. build the CMake target `pre_build_docs`, this build the doxygen intermediate. The output is in `/docs/.doxygen`
3. delete the content of the `/docs/out` folder
4. build the CMake target `build_docs`. Afterwards, the resulting website is in `/docs/out`

Idk why we have to delete `docs/out` to do a clean rebuild, if any file still exist sphinx will think it doesn't have to recompile. In `/docs/CMakeLists.txt` line 39 you can see me calling sphinx with:

```bash
${SPHINX_EXECUTABLE} -Ea -b html
```

where `SPHINX_EXECUTABLE` is deduced to `sphinx-build` at configuration. 

`-Ea` is supposed to always rebuild, but it doesn't work for some reason. I think it has to do with how the cmake cache works.

Sphinx will throw a bunch of ` WARNING: Duplicate C++ declaration` warnings. These are intended, I figured it would be nice to have all internal functions for each header both in the internal index (`graphics_interface.rst`, `physics_interface.rst`, `other.rst`) *and* in the public guide. Feel free to delete them from the internal index.

## Documentations

The following gaps need to be filled in by you, it's mostly me not understanding a function so it should take like one sentence per item to fix

+ `include/vulkan_interface.hpp` line 119
+ `include/vulkan_interface.hpp` line 169

+ `include/common.hpp` line 73

+ `docs/index.rst` 
 - may need an introduction paragraph, idk what how you would like me to describe it so I left it up to you

+ `docs/installation.rst` 
 - this works on my machine but please test it to make sure it's applicable to all environments

+ `docs/common_render.rst` line 11
+ `docs/common_render.rst`
 - idk the vulkan rendering pipeline so please check if this is correct. Also it could use more elaborate text, docs/common_* and docs/installation are intended for the public so they should be described in detail. All the other *.rst in `docs/` are internal implementation details, I don't think they need to be described any more than I did through doxygen

+ `docs/common_physics.rst` line 7
+ `docs/common_physics.rst`
    - check everything else for correctness and style
     
## Set CMake to Release
    
+ `Telescope/CMakeLists.txt` line 156

## Tests

I set up the mains in `Telescope/test` for you, you can run them using the IDE or the CTest commands, see `test/template_test.cpp`, it has a guide in the comments.

Take a look at `Telescope/test/test.hpp`, its basically exactly like Julias `@test` just with lambdas instead of `begin .. end` blocks. I saw that you used the Julia unit tests in starlight so I figure I'd reproduce the syntax so it's easier for you.

To get good-practice coverage, every function should have a test. I don't think that's really possible with vulkan which is why I left the `test/vulkan.cpp` empty. Also as long as Telescope is in primary development id'd be a good idea for you to fill out the tests during debugging. Any of the tests can be run as their own executable, you don't have to run them through CTest.

I wasn't able to understand what most of your functions are supposed to do, so I'm sorry for not being able to write the tests. I would've had to reverse engineer pretty much all of `src/src.cpp`.




