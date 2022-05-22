tests:
    all public functions and member functions shall be covered

lambdas:
    only use auto if necessary
    always specify trailing return type

only use uint64_t, int32_t, int8_t, etc. not char, long, unsigned 

if objects are plural, variable names should have plural s

if a block of code is executed exactly twice or more times, it should be replaced with a function of the same behavior, potentially a lambda to avoid name pollution.

order of .hpp declaration shall be the same as in the .cpp

all files using templates should implement all (templated and non-template) in .inl