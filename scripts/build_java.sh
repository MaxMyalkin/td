cd example/java
rm -rf build
mkdir "build"
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DTd_DIR=/Users/myalkin/Desktop/Projects/td/example/java/td/lib/cmake/Td  -DJAVA_HOME=/usr/local/opt/openjdk/libexec/openjdk.jdk/Contents/Home/ -DCMAKE_INSTALL_PREFIX:PATH=.. ..
cmake --build . --target install