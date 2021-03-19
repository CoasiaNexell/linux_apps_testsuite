# w to compile in standalone

1. sourcing toolchain environment
#> source /opt/poky/2.5.1/environment-setup-cortexa9hf-neon-poky-linux-gnueabi

2. run autogen.sh
#> ./autogen.sh

3. configure
#> ./configure --host=arm-poky-linux-gnueabi --prefix=$(SDKTARGETSYSROOT)/usr

4. compile
#> make

