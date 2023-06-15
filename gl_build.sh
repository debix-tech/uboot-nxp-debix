source /opt/fsl-imx-xwayland/6.1-langdale/environment-setup-armv8a-poky-linux
export ARCH=arm64
make distclean
make imx93_11x11_evk_defconfig
make -j32
