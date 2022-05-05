FROM debian:stretch

# Install dependencies
RUN apt-get update && apt-get install -y \
        cmake \
        gcc \
        git \
        libsdl2-2.0-0 \
        libsdl2-image-2.0-0 \
        libsdl2-dev \
        libsdl2-image-dev

RUN mkdir /src

# Compile Io libraries
RUN cd /src && git clone --recursive https://github.com/IoLanguage/io.git
RUN mkdir /src/io/build
RUN cd /src/io/build && cmake -DCMAKE_BUILD_TYPE=release -DWITHOUT_EERIE=1 .. && \
        make && make install
# Move the static version of libiovmall to /usr/local/lib
RUN cp /src/io/build/_build/lib/libiovmall_static.a /usr/local/lib/libiovmall.a

# Compile our code with static linking to Io and its dependencies.
COPY ./ /src/scribbler-build

CMD ["sh", "/src/scribbler-build/build.sh"]

