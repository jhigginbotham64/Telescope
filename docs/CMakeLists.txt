# reference used: https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# Find all the public headers
#get_target_property(TELESCOPE_INCLUDE_DIR telescope INTERFACE_INCLUDE_DIRECTORIES)
set(TELESCOPE_INCLUDE_DIR "./src")
file(GLOB_RECURSE TELESCOPE_INCLUDE_DIR ${TELESCOPE_INCLUDE_DIR}/*)

# configure doxygen
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src)
set(DOXYGEN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/docs/.doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/docs/.doxygen/xml/index.xml)
set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
set(DOXYFILE_OUT "${PROJECT_SOURCE_DIR}/docs/Doxyfile")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# add custom target to build doxygen
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
		#DEPENDS ${TELESCOPE_INCLUDE_DIR}
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
		MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        	COMMENT "Generating docs"
		VERBATIM)

add_custom_target(pre_build_docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# configure sphinx
set(SPHINX_SOURCE ${PROJECT_SOURCE_DIR}/docs)
set(SPHINX_BUILD ${PROJECT_SOURCE_DIR}/docs/out)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# add custom target to build sphinx from doxygen
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
	COMMAND
		${SPHINX_EXECUTABLE} -Ea -b html
		-Dbreathe_projects.telescope=${DOXYGEN_OUTPUT_DIR}/xml
		${SPHINX_SOURCE} ${SPHINX_BUILD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
	COMMENT "Generating documentation with Sphinx")

add_custom_target(build_docs ALL DEPENDS ${SPHINX_INDEX_FILE})

# install
install(DIRECTORY ${SPHINX_BUILD} DESTINATION ${CMAKE_INSTALL_DOCDIR})
