#!/bin/bash

# runs the julia-side tests. This also inherently tests the C++ library
# Usage: /bin/bash -i run_tests.sh

julia -e "include(\"../julia/telescope.jl\"); Main.ts.runtests();"