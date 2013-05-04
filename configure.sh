#/bin/sh
export CC='ccache gcc -std=c11'
export CXX='ccache g++ -std=c++11'
export CFLAGS='-g -Os'
export CXXFLAGS='-g -Os'
export CPPFLAGS=''
export LDFLAGS=''
./configure $*
