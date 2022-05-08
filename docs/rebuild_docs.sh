#!/bin/bash
#
# clean out folder, rebuild docs
#
# usage: cd Telescope/docs; bash rebuild_docs.sh
#
rm -r ./out
cmake --build ../cmake-build-debug --target pre_build_docs
cmake --build ../cmake-build-debug --target build_docs
