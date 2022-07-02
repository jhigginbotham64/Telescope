Intermission: Telescope in Julia
================================

What is the difference between this Telescope and :code:`Telescope.jl` ?

-----------------------------------------

Telescope is heavily used in `Starlight.jl <https://github.com/jhigginbotham64/Starlight.jl>`_, but may be
a solid base for any game engine written all by itself. All of telescopes functionality is
exported to Julia through `Telescope.jl <https://github.com/jhigginbotham64/Telescope.jl>`_. We can make this package
available by simply calling :code:`import Pkg; Pkg.add("Telescope")` from within Julia. This makes telescope easy access
in both C++ and Julia.

The obvious question, then, is:

a) What is the difference between Julia-telescope and C++-telescope?
b) Does this documentation also apply to :code:`Telescope.jl`?

Short answer:

a) very little
b) yes!

:code:`Telescope.jl` was designed and formatted specifically to imitate C++ syntax as closely as possible, under the
inherent limitations of Julia, of course.

Consider the following code snippet, written using C++ telescope:

.. code-block:: cpp
    :caption: A basic Telescope Main in C++

    #include <telescope.hpp>

    int main()
    {
        if (!ts::initialize())
            exit(1);

        auto window = ts::Window();
        window.create("title", 800, 600, ts::DEFAULT);

        while (window.is_open())
        {
            ts::start_frame(window);

            if (ts::InputHandler::was_pressed(ts::SPACE))
                std::cout << "space pressed." << std::endl;

            ts::end_frame(window);
        }

        return 0
    }

Here is this exact same program, written entirely in Julia with :code:`Telescope.jl`:

.. code-block:: julia
    :caption: the exact same Main, but in Julia

    import Telescope

    if !ts.initialize()
        exit(1)
    end

    window = ts.Window()
    ts.create!(window, "title", 800, 600, ts.DEFAULT)

    while ts.is_open(window)

        ts.start_frame!(window)

        if ts.InputHandler.was_pressed(ts.SPACE)
            println("space pressed.")
        end

        ts.end_frame!(window)
    end

    return 0

We see that both code excerpts are extremely similar. The only notable differences include:

- namespacing an object: :code:`ts::<Object>` is instead written in Julia as :code:`ts.<Object>`
- any member function call in C++:

        + :code:`<instance>.<member_function>(<arg>)`
    is instead written in Julia as:

        + :code:`ts.<member_function>(<instance>, <arg>)`
    because Julia does not offer C++-style member functions natively.
- a :code:`!` is appended to names of functions that mutate their arguments, this adheres to Julia syntax convention
- indices that would be 0-based in C++ are instead 1-based in Julia

Other than this, there is very little difference in notation between C++ telescope and Julia telescope. The Julia package
has the exact same classes and functions, named exactly the same, exhibiting exactly the same behavior, except for the
four differences mentioned above.
In fact, the Julia package directly calls the C++ functions through an internal C-interface, making them fully equivalent.

Because of this, all documentation and tutorials offered here also apply to :code:`Telescope.jl`. Most code can
be translated to Julia with very little effort, making knowledge about C++ Telescope fully transferable.