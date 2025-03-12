
# requirements
you need to install meson and make sure your gcc/g++ support cpp 23

# First time setup

setup buildir

$ meson setup build

compile all code

$ meson compile -C build

compile and run tests 

$ meson test -C build 


