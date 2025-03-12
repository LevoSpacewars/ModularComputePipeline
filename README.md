
# requirements
1. meson 1.6.0
2. gcc/g++ support cpp 23
3. doxygen 1.13.2

# First time setup

setup buildir

$ meson setup build

compile all code

$ meson compile -C build

compile and run tests 

$ meson test -C build 

# Building docs

$ doxygen docs/Doxyfile


https://levospacewars.github.io/ModularComputePipeline/files.html
