### System SDK Download
- Yocto-Linux 6.6.23_2.0.0    
     https://www.nxp.com/design/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX?   
   
### Modify sources/meta-imx/meta-bsp/recipes-bsp/u-boot/u-boot-imx_2024.04.bb
    UBOOT_SRC ?= "git://github.com/debix-tech/uboot.git;protocol=https"
    SRCBRANCH = "lf_v2024.04_Model_AB_2GBDDR"
    SRCREV = " ... commit id ... "

### Build yocto
    DISTRO=imx-desktop-xwayland MACHINE=imx8mpevk source imx-setup-desktop.sh -b debix-desktop
    bitbake -c compile -f -v u-boot-imx
    bitbake -c deploy -f -v u-boot-imx
    bitbake -c compile -f -v imx-boot
    bitbake -c deploy -f -v imx-boot

uboot bin file: debix-desktop/tmp/deploy/images/imx8mpevk/imx-boot-imx8mpevk-sd.bin-flash_evk

### Use ubuntu dd command write to device
    sudo dd if=imx-boot-imx8mpevk-sd.bin-flash_evk of=/dev/sdx bs=1k seek=32 conv=fsync
   
