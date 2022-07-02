Textures
========

Loading images onto the graphics card, blend mode, filtering mode

--------------------------------------------

Textures are images that live on the graphics card. As such, they are somewhat harder to manipulate than images living
in RAM. In return, we're able to performantly render and scale textures.

Static Textures
^^^^^^^^^^^^^^^

All textures inherit from :code:`ts::Texture`:

.. doxygenclass:: ts::Texture

This class is pure virtual (abstract), meaning it cannot be instantiated. Most commonly, we will instead be dealing
with one of this classes sub-classes: :code:`ts::StaticTexture`:

.. doxygenclass:: ts::StaticTexture
    :members:

To create a texture, we need a :code:`ts::Window`. This is because the window supplies the rendering context, which
is basically the GPU-side state we will load the texture into.

With our window ready, we can create a `ts::StaticTexture` in two ways:

.. code-block:: cpp
    :caption: Creating / Loading a Texture

    auto window = ts::Window(// ...
    auto texture = ts::Texture(&window);

    // create texture of specific size, all pixels will be set to one color
    texture.create(500, 500, RGBA(1, 1, 1, 1));

    // load a texture from an image file
    texture.load("~/absolute/path/to/image.png")

Textures support loading .png, .bmp, .jpg, .jpeg files.

---------------------------

Creating A Sprite
^^^^^^^^^^^^^^^^^

Once we have the texture in memory, we need to map it to a shape before it can be rendered. As textures are rectangular,
:code:`ts::RectangleShape` is usually the best option, though all shapes supports being textured.

.. code-block:: cpp
    :caption: Creating a Shape for the Texture

    auto window = ts::Window(// ...
    auto texture = ts::Texture(&window);
    texture.load(// ...

    auto sprite = ts::RectangleShape(Vector2f(50, 50), texture.get_size());
    sprite.set_texture(&texture);

    // render
    while (window.is_open)
    {
        ts::start_frame(&window);
        window.render(&sprite);
        ts::end_frame(&window);
    }

A texture rectangle is called a `sprite`. In the code above, we created the sprite to have the same size as the texture,
this does not have to be case, however.

We can only display part of a texture by settings the sprites `texture rectangle`, which is a geometric rectangle
in local texture coordinates (where :code:`{0, 0}` is the top-left pixel, :code:`{1, 1}` the bottom-right pixel).

.. doxygenfunction:: ts::Shape::set_texture_rectangle

This remaps all texture coordinates of the shapes vertices. By dividing a texture into multiple rectangular areas,
we can create animations by simply iterating through them at a specified speed. A texture that is intended to be divided
like this is often called a `sprite sheet`.

--------------------------------------------

Filtering-Mode
^^^^^^^^^^^^^^

If the sprite is the same size as the texture, the texture will show up as-is, that is, one pixel on the screen
corresponds to one pixel of the texture data. If the shape is smaller or bigger than the texture, this is not the case.

Instead, the texture is **filtered** by the graphics card. Telescope allows users to choose, which filtering algorithm
should be used for any specific texture:

.. doxygenfunction:: ts::Texture::set_filtering_mode

This function takes a value of the :code:`ts::TextureFilteringMode` enum:

.. doxygenenum:: ts::TextureFilteringMode

For more information on the differences between these modes, see `here <https://en.wikipedia.org/wiki/Image_scaling#Nearest-neighbor_interpolation>`_.

--------------------------------------------

Blend-Mode
^^^^^^^^^^

If one texture (sprite) is rendered on top of another, their pixel data will interact according to their **blend mode**.
Recall that each pixel has a color in RGBA format. The following blend modes are offered by telescope:

.. doxygenenum:: ts::TextureBlendMode

--------------------------------------------

Dynamic Textures
^^^^^^^^^^^^^^^^

Unlike :code:`ts::StaticTexture`, :code:`ts::DynamicTexture` allows users to modify the pixel values,
even after the texture is already GPU-side.

(this feature is not yet implemented)

--------------------------------------------

Render-Textures
^^^^^^^^^^^^^^^

Render-textures are a special kind of texture that allows rendering of objects, much like a windows surface. This is
useful for assembling images programmatically.

.. doxygenclass:: ts::RenderTexture
    :members:

We see that, unlike :code:`ts::StaticTexture` and :code:`ts::DynamicTexture`, render textures cannot be loaded from images
on the disk. We can only create an empty render texture of specified size. Other than this, :code:`ts::RenderTexture`
behaves exactly like the other textures.

--------------------------------------------

Texture Color
^^^^^^^^^^^^^

Telescope allows for two types of color multiplier: a RGBA value each sprites fragment will be multiplied with.
Firstly, all textures have their own color multiplier:

.. doxygenfunction:: ts::Texture::set_color
.. doxygenfunction:: ts::Texture::get_color

These apply to anytime the texture is used.

Secondly, each texture will be multiplied with the color of the vertices of the
shape it is rendered with. This color only applies per-shape, two different shapes with the same texture may have different
vertex colors.

If two vertices of the same shape have a different color, their colors will be interpolated automatically.

------------------------------------------------

ts::Texture
^^^^^^^^^^^

For completeness, a full list of :code:`ts::Texture`s functions is provided here.

.. doxygenclass:: ts::Texture
    :members:


