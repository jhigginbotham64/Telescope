Installation
============

Dependencies
************

To build Telescope from source, the following dependencies need to be met:

+ `Bullet <https://github.com/bulletphysics/bullet3>`_
  - also available as :code:`libbullet-dev`
+ `Vulkan <https://vulkan.lunarg.com/>`_
  - also available as :code:`libvulkan-dev`
+ `SDL2 <https://www.libsdl.org/download-2.0.php>`_
  - also available as :code:`libsdl2-dev`
  - additionally, :code:`libsdl2-image`, :code:`libsdl2-mixer`, :code:`libsdl2-ttf`, :code:`libsdl2-net` may need to be installed separately
+ `glm <https://github.com/g-truc/glm>`_
  - also available as :code:`libglm-dev`
+ `shaderc <https://github.com/google/shaderc#downloads>`_

Clicking on the links above will lead you to the correct download pages for each dependency. Alternatively, they can be installed through your package manager, potentially under the names supplied above.

------------------------

Building
********

To install Telescope, execute, in any public directory

.. code-block:: console

	git clone https://github.com/jhigginbotham64/Telescope
	cd Telescope
	mkdir build
	cd build
	cmake .. #-DCMAKE_INSTALL_PREFIX=<install location>
	make install

Where :code:`-DCMAKE_INSTALL_PREFIX=<install location>` is an optional argument that determines, what directory the Telescope shared library will be installed into.

After installation, you can interface with Telescope from Julia using [Starlight.jl](https://github.com/jhigginbotham64/Starlight.jl).

------------------------

If you wish to use telescope for your C / C++ application, in your own CMakeLists.txt, add the following lines:

.. code-block:: console

	find_library(telescope REQUIRED
	    NAMES telescope
	    #PATHS <install location>
	)
	target_link_libraries(<your_target> PRIVATE telescope)

Where
+ `<your_target>` is the name of your CMake library or executable
+ `<install location>` is the location specified during CMake configuration [earlier](#installation)

Then you can make Telescope available to your library using

.. code-block:: cpp

	#include <telescope.h>

------------------------

Troubleshooting
===============

telescope.h: No such file or directory
****************************************

When compiling your own C / C++ target that uses telescope, the following compiler error may occur:

.. code-block:: console

	/home/.../main.cpp: fatal error: telescope.h: No such file or directory
	   11 | #include <telescope.h>
	      |          ^~~~~~~~~~~~~

This happens if the telescope install directory was not added to your CMake targets include directories. To address this, in your own CMakeLists.txt, add the following lines:

.. code-block:: console

	find_library(telescope REQUIRED
	    NAMES telescope
	    PATHS <install location>
	)
	target_include_directories(<your_target> PRIVATE <install location>)
	target_link_libraries(<your_target> PRIVATE telescope)

Where
  + :code:`<your_target>` is the name of your CMake executable or library
  + :code:`<install location>` is the directory specified as :code:`CMAKE_INSTALL_PREFIX` during [CMake configuration](#installation)

Now, your compiler should be able to locate :code:`telescope.h` properly.

------------------------

Could not find shaderc_shared
*****************************

During CMake configuration, the following error may occur:

.. code-block:: console

	Unable to detect shaderc_shared library.  Make sure it is installed
	correctly.  You can manually specify the path using:

	   -DSHADERC_LIB_DIR=/path/to/shaderc/lib

	 during cmake configuration.

	CMake Error at cmake/Findshaderc_shared.cmake:30 (find_library):
	  Could not find shaderc_shared using the following names:
	  libshaderc_shared.so

This means Telescope was unable to detect the :code:`shaderc_shared` library, which is part of the shaderc package. If you are sure shaderc is already installed properly, you can manually specify the path to the shared library using the :code:`SHADERC_LIB_DIR` CMake variable during configuration, like so:

.. code-block:: console

	# in Telescope/build
	cmake .. -DSHADERC_LIB_DIR=/path/to/shaderc/lib

Where :code:`/path/to/shaderc/lib` should point to a directory that has the following layout:

.. code-block:: console

	shaderc/
	  bin/
	  include/
	  lib/
	    libshaderc_shared.so
	    (...)
	  share/

Where :code:`libshaderc_shared.so` may have a different prefix and/or suffix depending on your system, for example :code:`shaderc_shared.dll`.



