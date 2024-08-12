### System SDK Download
- Ubuntu 22.04 :    
     https://github.com/nxp-imx/meta-nxp-desktop/tree/lf-6.1.22-2.0.0-mickledore

- Yocto-Linux 6.1.22_2.2.0    
     repo init -u https://github.com/nxp-imx/imx-manifest -b imx-linux-mickledore -m imx-6.1.22-2.0.0.xml
   
### Modify sources/meta-imx/meta-bsp/recipes-bsp/u-boot/u-boot-imx-common_2023.04.inc
    UBOOT_SRC ?= "git://github.com/debix-tech/uboot.git;protocol=https"
    SRCREV = "last commit id"

### Build yocto
    DISTRO=imx-desktop-xwayland MACHINE=imx8mpevk source imx-setup-desktop.sh -b debix-desktop
    bitbake -c compile -f -v u-boot-imx
    bitbake -c deploy -f -v u-boot-imx
    bitbake -c compile -f -v imx-boot
    bitbake -c deploy -f -v imx-boot

uboot bin file: debix-desktop/tmp/deploy/images/imx8mpevk/imx-boot-imx8mpevk-sd.bin-flash_evk

### Use ubuntu dd command write to device
    sudo dd if=imx-boot-imx8mpevk-sd.bin-flash_evk of=/dev/sdx bs=1k seek=32 conv=fsync
   
