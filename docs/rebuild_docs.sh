#!/bin/bash

#
# clean out folder, rebuild docs
#
# usage: cd Telescope/docs; bash rebuild_docs.sh
#
rm -r ./out
/home/clem/Applications/clion-2020.1.2/bin/cmake/linux/bin/cmake --build /home/clem/Workspace/Telescope/cmake-build-debug --target pre_build_docs
/home/clem/Applications/clion-2020.1.2/bin/cmake/linux/bin/cmake --build /home/clem/Workspace/Telescope/cmake-build-debug --target build_docs
