Textures
========

Loading images onto the graphics card, blend mode, filtering mode

--------------------------------------------

Textures are images that live on the graphics card. As such, they are somewhat harder to manipulate than images living
in RAM. In return, we're able to performantly render, scale and multiply textures.

Static Textures
^^^^^^^^^^^^^^^

All textures have to inherit from :code:`ts::Texture`:

.. doxygenclass:: ts::Texture

This class is pure virtual (abstract), meaning it cannot be instantiated. Most commonly, we will instead be dealing
with one of this classes daughter: :code:`ts::StaticTexture`:

.. doxygenclass:: ts::StaticTexture
    :members:

To create a texture, we need a :code:`ts::Window`. This window creates the rendering context, which is basically the
telescopes GPU-side state. Next, we can create a `ts::StaticTexture` in two ways:

.. code-block:: cpp
    :caption: Creating / Loading a Texture

    auto window = ts::Window(// ...
    auto texture = ts::Texture(&window);

    // create texture of specific size, all pixels will be set to one color
    texture.create(500, 500, RGBA(1, 1, 1, 1));

    // load a texture from an image file
    texture.load("~/absolute/path/to/image.png")

Textures support loading .png, .bmp, .jpg, .jpeg files.

Creating A Sprite
^^^^^^^^^^^^^^^^^

Once we have the texture loaded, we need to create a shape to map it to, so it can be rendered:

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

Here, we created a sprite that has the same size as the texture. After :code:`set_texture`, whenever we render
:code:`sprite`, the texture will show up on the screen.

We can change what part of the texture the sprite will display with:

.. doxygenfunction:: ts::Shape::set_texture_rectangle

This remaps all texture coordinates of the shape. By dividing a texture into multiple rectangular areas, we can create
animations by simply iterating through them at a specified speed. A texture that is intended to be divided like this
is of called a **sprite sheet**.

--------------------------------------------

Filtering-Mode
^^^^^^^^^^^^^^

If the sprite is the same size as the texture, the texture will show-up as-is, that is, one pixel on the screen
corresponds to one pixel of the texture data. What if the shape is smaller or bigger than the texture?

In cases like this, the texture is **filtered** by the graphics card. Telescope allows users to choose, which algorithm
should be used for this by calling:

.. doxygenfunction:: ts::Texture::set_filtering_mode

This function takes a value of the :code:`ts::TextureFilteringMode` enum:

.. doxygenenum:: ts::TextureFilteringMode

For more information on what these modes mean, c.f. `here <https://en.wikipedia.org/wiki/Image_scaling#Nearest-neighbor_interpolation>`_.

--------------------------------------------

Blend-Mode
^^^^^^^^^^

If one texture (sprite) is rendered on top of another, their pixel data will interact according to their **blend mode**.
Recall that each pixel has a color in RGBA format. The following blend modes are offered by telescope:

.. doxygenenum:: ts::TextureBlendMode

--------------------------------------------

Dynamic Textures
^^^^^^^^^^^^^^^^

Unlike :code:`ts::StaticTexture`, :code:`ts::DynamicTexture` allows users to modify the pixel values after the texture
is already GPU-side.

(this feature is not yet implemented)

--------------------------------------------

Render-Textures
^^^^^^^^^^^^^^^

Render-textures are a special kind of texture that allows rendering of objects, much like a window. This is useful for
assembling images programmatically.

.. doxygenclass:: ts::RenderTexture
    :members:

We see that, unlike :code:`ts::StaticTexture` and :code:`ts::DynamicTexture`, render textures cannot be loaded from images
on the disk. Instead, we can only create an empty render texture of specified size. Other than this, they can be handled
just like other textures.

--------------------------------------------

Texture Color
^^^^^^^^^^^^^

Telescope allows for two types of color multiplier, which is a RGBA value each texture pixel will be multiplied with.
Firstly, all textures have their own color multiplier:

.. doxygenfunction:: ts::Texture::set_color
.. doxygenfunction:: ts::Texture::get_color

These apply to any usage of the shape. Secondly, each texture will be multiplied with the color of the vertices of the
shape it is rendered with. This color only applies per-shape, two different shapes with the same texture may have different
vertex colors.

If two vertices of the same shape have a different color, their colors will be interpolated automatically on the graphics
card.


