cd /path/to/codes
mkdir build
cd build
cmake ..
cmake --build . --target install -j8
./tabulate /path/to/source.txt
./a.out # To execute the compiled translated C++ source code.