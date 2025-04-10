source /opt/fsl-imx-xwayland/6.6-scarthgap/environment-setup-armv8a-poky-linux
export ARCH=arm64
make distclean
make imx8mp_evk_defconfig
make -j32
