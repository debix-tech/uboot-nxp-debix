#!/bin/bash

if [ $# != 1 ] ; then
	echo "select imx91 or imx93 ..."
	echo "eg. $0 <imx93 or imx91> "
elif [ "$1" = "imx91" ] ;then
	echo "start building imx91 ... "
source /opt/fsl-imx-xwayland/6.12-walnascar/environment-setup-armv8a-poky-linux
export ARCH=arm64

make distclean
make imx91_11x11_evk_defconfig
make -j32

elif [ "$1" = "imx93" ] ;then
	echo "start building imx93 ... "
source /opt/fsl-imx-xwayland/6.12-walnascar/environment-setup-armv8a-poky-linux
export ARCH=arm64

make distclean
make imx93_11x11_evk_defconfig
make -j32

fi
