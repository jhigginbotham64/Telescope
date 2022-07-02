#!/bin/bash
#
# clean out folder, rebuild docs without cmake
#
# usage: cd Telescope/docs; bash rebuild_docs.sh
#
#rm -r ./out
doxygen
sphinx-build -Ea -j 4 -b html -Dbreathe_projects.telescope=.doxygen/xml . ./out
