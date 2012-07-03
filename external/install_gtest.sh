mkdir -p $1/local/include
mkdir -p $1/local/lib
cp -R $2/include/* $1/local/include
cp -R $3/*.a $1/local/lib
