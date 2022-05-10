#!/bin/bash
#
# clean out folder, rebuild docs
#
# usage: cd Telescope/docs; bash rebuild_docs.sh <build_dir>
#
# where <build_dir> is the output dir specified at cmake configuration
#
rm -r ./out
mkdir $1
cmake --build "../$1" --target pre_build_docs
cmake --build "../$1" --target build_docs
