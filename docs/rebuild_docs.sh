#!/bin/bash
#
# clean out folder, rebuild docs
#
# usage: cd Telescope/docs; bash rebuild_docs.sh
#
rm -r ./out
cmake --build ../build --target pre_build_docs
cmake --build ../build --target build_docs
