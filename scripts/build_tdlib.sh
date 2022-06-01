cd jnibuild
cmake -DCMAKE_BUILD_TYPE=Release -DTD_ENABLE_JNI=ON -DCMAKE_INSTALL_PREFIX:PATH=../example/java/td .. -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl/ -DJAVA_HOME=/usr/local/opt/openjdk/libexec/openjdk.jdk/Contents/Home/
cmake --build . --target install