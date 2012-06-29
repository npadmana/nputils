mkdir -p $1/local/include
mkdir -p $1/local/lib
cp -R $1/build/src/gtest/include/* $1/local/include
cp -R $1/build/src/gtest-build/*.a $1/local/lib
