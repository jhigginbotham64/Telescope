General Usage: Rendering
========================

Most of the time, we will interface with Telescope through its public C shared library, which only contains a limited number of functions. Usually, a telescope :code:`main` will look like this:

.. code-block:: cpp
   :caption: Typical :code:`main` of a C++ application using Telescope

   #include <telescope.h>

   // TODO: I have no idea how to use telescope, please edit this
   int main()
   {
       TS_Init("<window title>", 500, 500);

       while (something)
       {
           TS_VkBeginDrawPass
           TS_VkCmdClearColorImage
           // draw sprites here using TS_VkCmdDrawRect / TS_VkCmdDrawSprite
           TS_VkEndDrawPass
       }

       TS_Quit();
   }

Let's talk through each of the functions used here, one by one:

Initialization & Shutdown
*************************

To initialize the telescope state, we first need to call :code:`TS_Init`. This function takes three arguments:

.. doxygenfunction:: TS_Init

Afterwards, at the end of our application, we call :code:`TS_Quit` to safely quit the state and deallocate all physics and render objects

.. doxygenfunction:: TS_Quit

Drawing: Render Cycle
*********************

Each frame, Telescope will go through what is called a *draw pass*. <explain what that is here I have no idea>.

This always begins with :code:`TS_VkBeginDrawPass`, and ends with :code:`TS_VkEndDrawPass`

.. doxygenfunction::TS_VkBeginDrawPass
.. doxygenfunctino::TS_VkEndDrawPass

---------------------

We can clear the entire screen with a solid color, which we need to do at the start of each frame. Otherwise, things rendered last frame will still be displayed. To clear the screen, we use:

.. doxygenfunction:: TS_VkCmdClearColorImage

---------------------

Of course, we don't just want to draw a solid color. <explain how to create sprites and textures here>.

Then, we can render them using:

.. doxygenfunction:: TS_VkCmdDrawRect
.. doxygenfunction:: TS_VkCmdDrawSprite



