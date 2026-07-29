[English](./README.md) | [简体中文](./README_CN.md)

### System SDK Download

- Yocto-Linux 6.12.3_1.0.0
  https://www.nxp.com/design/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX?

### Modify sources/meta-imx/meta-bsp/recipes-bsp/u-boot/u-boot-imx_2024.04.bb

```shell
UBOOT_SRC ?= "git://github.com/debix-tech/uboot-nxp-debix;protocol=https"
SRCBRANCH = "lf_v2024.04-yocto_L6.12.3-debix_model_ab_4gbddr"
SRCREV = " ... commit id ... "
```

`SRCREV` can be obtained through the commit on git hub or through the `git log` command:

```shell
debix@polyhex:~/workstation/Github/uboot-nxp-debix$ git checkout lf_v2024.04-yocto_L6.12.3-debix_model_ab_4gbddr
Already on 'yocto_L6.12.3-debix_model_ab_2gbddr'
Your branch is up to date with 'origin/lf_v2024.04-yocto_L6.12.3-debix_model_ab_4gbddr'.

```

`666557f2d6dafab3725a6bcc6c744678b4326dc2` is the commit id

### Build uboot

```shell
DISTRO=imx-desktop-xwayland MACHINE=imx8mpevk source imx-setup-desktop.sh -b debix-desktop
bitbake -c compile -f -v u-boot-imx
bitbake -c deploy -f -v u-boot-imx
bitbake -c compile -f -v imx-boot
bitbake -c deploy -f -v imx-boot
```



uboot bin file: `debix-desktop/tmp/deploy/images/imx8mpevk/imx-boot-imx8mpevk-sd.bin-flash_evk`

### Use ubuntu dd command write to device

```shell
sudo dd if=imx-boot-imx8mpevk-sd.bin-flash_evk of=/dev/sdx bs=1k seek=32 conv=fsync
```
