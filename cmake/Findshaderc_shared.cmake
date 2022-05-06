#[=======================================================================[.rst:

Findshaderc_shared
-----------

This module is intended to detect the shaderc_shared library.
If shaderc_shared is not installed globally, the user will have to set
`SHADER_LIB_DIR` to point to shaderc/lib. If this variable is not yet
set, a warning will be printed educating users on how to set it.

Imported Targets
^^^^^^^^^^^^^^^^

``shaderc_shared``

Result Variables
^^^^^^^^^^^^^^^^

``SHADERC_SHARE_INCLUDE_DIRS``
    directory pointing to shaderc/include

#]=======================================================================]

if(NOT DEFINED shaderc_shared)

    if (NOT DEFINED SHADERC_LIB_DIR)
        message(WARNING "Unable to detect shaderc_shared library. Make sure it is installed correctly. You can manually specify the path using:\n\t -DSHADERC_LIB_DIR=/path/to/shaderc/lib\n during cmake configuration.")
    endif()

    find_library(shaderc_shared REQUIRED
        NAMES "${CMAKE_SHARED_LIBRARY_PREFIX}shaderc_shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
        PATHS ${SHADERC_LIB_DIR}
    )
endif()

get_filename_component(SHADERC_DIR ${shaderc_shared} DIRECTORY)
set(SHADERC_INCLUDE_DIRS "${SHADERC_DIR}/../include")