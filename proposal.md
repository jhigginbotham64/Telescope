# Proposal for Refactoring of Telescope

### Vulkan / SDL Render
+ Move global state into a `Window` class. This class will be encapsulated, making initialization / shutdown a function operating on objects, rather than a global state. This should allow for multiple windows
+ Implement a shader interface, such that users are able to replace the shaders in the render pipeline
+ separate render-related things from the SDL system / physics modules
+ reorganize the system architecture to follow a set render cycle:
  ```cpp
  auto window = TS_Window(/* ... */)
          
  // user initialization here
  
  while (window.is_open()
  {
    /* start of frame */
  
    // clear screen
    // handle user input
    
    // user simulation here
  
    // handle physics (with arbitrary precision)
  
    // render all user queued objects
    // wait for vsync
    // display result in the window
  
    /* end of frame */
  }
  
  // shutdown
  ```
+ **optional**: allow for moving textures between CPU ram and GPU ram. This allows for programmatically modifying textures without shaders
+ **optional**: allow for benchmark diagnostics: frame usage, current memory load, etc.

### Physics / Bullet
+ separate physics related things from the SDL system / render modules

### general code
+ translate all functionally in `src.cpp` to C++20 style, including object oriented design & modern memory management
+ this does not change the exported C interface, it can be C++ behind the scenes but exported as pure C
+ split each functionality into separate files
+ allow telescope to be used as a library in a pure C++-project
+ translate code internal usage (excluding the public interface) to my styleguide
+ provide an exception system

### Tests
+ implement tests, achieving 100% coverage for all public, user-aimed functions 
+ provide a "hello world" `main`, users can use to start out with when creating new projects using telescope
+ **optional**: implement benchmarks for performance-intensive functions

### Telescope.jl

The julia interface will be updated to reflect any potentially new functions, old interface names will be preserved such that no modification to Starlight.jl is necessary.

### SDL Non-Render

#### User Input
Create a global `InputHandler`, which implements the following:

+ enum `Key`, one keycode for each key
+ `is_key_down(::Key)`
    - is the key identified by argument currently pressed this frame
+ `has_state_changed(::Key)`
    - is the keys state (pressed or not pressed) this frame different from last frame
+ `was_key_pressed(::Key)`
    - `is_key_down(::Key) and has_state_changed(::Key)`
+ `was_key_released(::Key)
    - `not is_key_down(::Key) and has_state_changed(::Key)`
+ `get_hold_duration(::Key)`
    - get the duration of how long a key has been down without changing state
    
This will improve the message system, as, instead for having to listen to messages, the global InputHandler can simply be accessed in a thread-safe way, improving user dev experience
+ Implement a Julia-side Interface for the new InputHandler

#### Audio
+ provide a global interface for loading / unloading audio
+ make a distinction between samples and music
+ dynamically start / stop samples that are already playing
+ **optional**: allow for signal processing interface of audio (to allow for fade-in / fade-out for example)

### CONTRIBUTING.md

Write `CONTRIBUTING.md`, includes:

+ Code of Conduct
+ general Code styleguide (language-independent)
+ C++ Style Guide
+ Documentation Style Guide
+ explanation of QA pipeline (proposal -> fork repo -> (open PR <-> review) -> tests)
+ contact information

### Update Documentation

+ update the readthedocs documentation for any potentially newly implement features or changes in function behavior
+ add new docstrings for any potentially newly implement features
+ add a fully functional hello_world users can use to start out with
+ explain the newly implemented audio / shader interfaces

## Conditions

### Copyright

All copyright for code created during this project is held fully by you and can be licensed in any way you choose, or not at all

### Credits

The `Authors` section of `README.md` will be modified to list me as a co-author

### Payment

On successfully merging my fork, 1500USD will be billed. This includes the already implemented work on logging and error handling, no additional payment is required for past work. Any potential work resulting from bugs discovered during review, or issue occurring because of any faults in my implementation or the docs pipeline, will be fixed free of charge, even after the merge. 

The payment *excludes* transactions fees:
```
amount payed by you: US$1500

in euro: €1.420
PayPal Fees: ~5% = €71

amount received by me: €1349
```