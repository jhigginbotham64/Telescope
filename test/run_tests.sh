# Usage: /bin/bash -i run_tests.sh

echo ---------------------------------------------------------------------
cd ../julia
julia -e "include(\"telescope.jl\"); Main.ts.test.run()"