# TODO: delete this
echo ---------------------------------------------------------------------
cd build
make telescope
cd ..
/home/clem/Applications/julia-1.8.0-beta3/bin/julia  -e "include(\"telescope.jl\"); Main.ts.test.run()"