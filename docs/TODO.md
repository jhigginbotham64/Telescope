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
     
## CMake

Please reads the inline documentation at the start of `Telescope/CMakeLists.txt`, I restructured your cmake environment and added some new options and variables. Hopefully it just works but read it anyway so you can interact with cmake through your IDE properly

Also, fix
    
+ `Telescope/CMakeLists.txt` line 156

before releasing.

## Tests

I set up the mains in `Telescope/test` for you, you can run them using the IDE or the CTest commands, see `test/template_test.cpp`, it has a guide in the comments.

A typical CTest output looks like this:

```
/home/clem/Applications/clion-2020.1.2/bin/cmake/linux/bin/ctest --extra-verbose
Testing started at 18:55 ...
UpdateCTestConfiguration  from :/home/clem/Workspace/Telescope/cmake-build-debug/DartConfiguration.tcl
Parse Config file:/home/clem/Workspace/Telescope/cmake-build-debug/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/clem/Workspace/Telescope/cmake-build-debug/DartConfiguration.tcl
Parse Config file:/home/clem/Workspace/Telescope/cmake-build-debug/DartConfiguration.tcl
Test project /home/clem/Workspace/Telescope/cmake-build-debug
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end

1: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_common
1: Test timeout computed to be: 1500
1: CLAMP: PASSED
1: TS_NDCRect: PASSED
1: TS_NDCX: PASSED
1: TS_NDCY: PASSED
1: TS_NTCRect: PASSED
1: TS_NTCU: PASSED
1: TS_NTCV: PASSED
1: 7 out of 7 tests passed.

2: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_bullet
2: Test timeout computed to be: 1500
2: TS_BtAddRigidBox: PASSED
2: TS_BtAddStaticBox: PASSED
2: TS_BtAddTriggerBox: PASSED
2: TS_BtGetLinearVelocity: PASSED
2: TS_BtGetNextCollision: PASSED
2: TS_BtGetPosition: PASSED
2: TS_BtInit: PASSED
2: TS_BtQuit: PASSED
2: TS_BtRemovePhysicsObject: PASSED
2: TS_BtSetCollisionMargin: PASSED
2: TS_BtSetGravity: PASSED
2: TS_BtSetLinearVelocity: PASSED
2: TS_BtStepSimulation: PASSED
2: 13 out of 13 tests passed.

3: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_ts
3: Test timeout computed to be: 1500
3: TS_Init: PASSED
3: TS_PhysicsObject: ctor & dtor: PASSED
3: TS_PlaySound: PASSED
3: TS_Quit: PASSED
3: 4 out of 4 tests passed.

4: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_vertex
4: Test timeout computed to be: 1500
4: TS_Vertex: ctor: PASSED
4: TS_Vertex: getAttributeDescriptions: PASSED
4: TS_Vertex: getBindingDescription: PASSED
4: TS_Vertex: getTransform: PASSED
4: 4 out of 4 tests passed.

5: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_vma
5: Test timeout computed to be: 1500
5: TS_VmaCreateBuffer: PASSED
5: TS_VmaCreateImage: PASSED
5: TS_VmaDestroyAllocator: PASSED
5: TS_VmaDestroyBuffers: PASSED
5: 4 out of 4 tests passed.

6: Test command: /home/clem/Workspace/Telescope/cmake-build-debug/test_vulkan
6: Test timeout computed to be: 1500
6: 0 out of 0 tests passed.

100% tests passed, 0 tests failed out of 6

Total Test time (real) =   0.17 sec
Process finished with exit code 0
```

Because they are all neatly split into mains, the output is really easy to parse. It shows you exactly what test failed and even which assertion, if you name the assertion using the defaulted argument of `Test::test`.

Take a look at `Telescope/test/test.hpp`, its basically exactly like Julias `@test` just with lambdas instead of `begin .. end` blocks. I saw that you used the Julia unit tests in starlight so I figure I'd reproduce the syntax so it's easier for you.

To get good-practice coverage, every function should have a test. I don't think that's really possible with vulkan which is why I left the `test/vulkan.cpp` empty. Also as long as Telescope is in primary development id'd be a good idea for you to fill out the tests during debugging. Any of the tests can be run as their own executable, you don't have to run them through CTest.

I wasn't able to understand what most of your functions are supposed to do, so I'm sorry for not being able to write the tests. I would've had to reverse engineer pretty much all of `src/src.cpp`.

## src.cpp

`src/src.cpp` is your old `telescope.cpp`. I pulled the freshest version from the master branch, hopefully that way you won't have to do much merging.

Please compile your code using the `-Wall` argument, it triggers a bunch of warnings which a master branch shouldn't do. 

Also, I think it would be better software design if all the global variables in `src.cpp` where inside a struct, the struct is basically an instance of your render environment. Maybe something like `telescope::State`. Then, just have a single global variable:

```cpp
static telescope::State _state
```

That holds all the current variables as `static` members.

That way, the code is way more extendable. You can have multiple states in the future, for example for multiple windows (I remember seeing that as an issue in starlight.jl).

I tried to mimic your style, even though I strongly believe in my own. I think it would help future collaborators if you stick to at least the following of my rules:

1. all non-private member functions need to be declared publicly in a header
   
I already did this for you, because I actually needed to. Doxygen only parses headers and I don't want to add 1000 lines of documentation to your old `telescope.cpp`

2. all non-private member functions, their arguments, and global variables need to be named a combination of words in the english dictionary. For example:

```cpp
// not congruent with my style guide:
void resizeWindowDim(size_t x, size_t y);

// instead:
void resizeWindowDimension(size_t width, size_t height);
```

This makes code so much easier to read for a third party. I know C++ isn't python or Julia, but still. 

3. only use signed integers when their value is **expected** to be negative

For example, your window size takes two `int`s. What happens if someone calls `TS_VkCreateWindow("title", -10, -200)`? I don't actually know, but if those were `size_t`s it would throw a compiler error instead of triggering undefined behavior. The same applies to a bunch of your variables, it seems like you use `int` as the default, when `size_t` should be the default and `int` the exception.

## Closing Statement

Hopefully you're happy with the documentation outline, you still have to fill some things in but I at least took all the setup of your shoulders. I still think documentation is the most important part of a library intended for developers. Even if its the most amazing library ever, either people give up after 1h of not getting things to work or they flood your issues on github, neither of which is very good lol.









