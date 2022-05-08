# if already available, don't override
if(NOT DEFINED shaderc_shared)

    # user can specify SHADERC_LIB_DIR to manually specify path
    if (NOT DEFINED SHADERC_LIB_DIR)
        message(WARNING "Unable to detect shaderc_shared library. Make sure it is installed correctly. You can manually specify the path using:\n\t -DSHADERC_LIB_DIR=/path/to/shaderc/lib\n during cmake configuration.")
    endif()

    find_library(shaderc_shared REQUIRED
        NAMES "${CMAKE_SHARED_LIBRARY_PREFIX}shaderc_shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
        PATHS ${SHADERC_LIB_DIR}
    )
endif()

# deduce include path and export
get_filename_component(SHADERC_DIR ${shaderc_shared} DIRECTORY)
set(SHADERC_INCLUDE_DIRS "${SHADERC_DIR}/../include")