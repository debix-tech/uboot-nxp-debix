[English](./README.md) | [简体中文](./README_CN.md)

### 系统SDK下载

- Yocto-Linux 6.12.49_2.2.0
  https://www.nxp.com/design/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX?
  
  

> 下面是关于如何在yocto工程下，编译uboot

### 修改sources/meta-imx/meta-imx-bsp/recipes-bsp/u-boot/u-boot-imx-common_2025.04.inc

```shell
UBOOT_SRC ?= "git://github.com/debix-tech/uboot-nxp-debix;protocol=https"
SRCBRANCH = "lf_v2025.04-yocto-L6.12.49-2.2.0-debix-emb-13-a1"
SRCREV = " ... commit id ... "
```

`SRCREV`可以通过git hub上面的commit获取或者通过`git log`命令获取:

```shell
debix@polyhex:$ git checkout sources/meta-imx/meta-imx-bsp/recipes-bsp/u-boot/u-boot-imx-common_2025.04.inc
debix@polyhex:$ git branch
  lf_v2024.04-debix_model_ab_2gbddr
* lf_v2025.04-yocto-L6.12.49-2.2.0-debix-emb-13-a1
  yocto_L6.12.3-debix_model_ab_2gbddr

# show commit id
debix@polyhex:$ git log


```
### 编译uboot

```shell
#build imx93 uboot
DISTRO=fsl-imx-xwayland MACHINE=imx93-11x11-lpddr4x-evk source imx-setup-release.sh -b debix-imx93

#build imx91 uboot
DISTRO=fsl-imx-xwayland MACHINE=imx91-11x11-lpddr4x-evk source imx-setup-release.sh -b debix-imx91

bitbake -c compile -f -v u-boot-imx
bitbake -c deploy -f -v u-boot-imx
bitbake -c compile -f -v imx-boot
bitbake -c deploy -f -v imx-boot
```


生成的uboot bin file路径: 
`imx93/tmp/deploy/images/imx93-11x11-lpddr4x-evk/imx-boot-imx93-11x11-lpddr4x-evk-sd.bin-flash_singleboot`
`imx93/tmp/deploy/images/imx91-11x11-lpddr4x-evk/imx-boot-imx91-11x11-lpddr4x-evk-sd.bin-flash_singleboot`

### 使用`dd`命令去烧录

```shell
sudo dd if=imx-boot-imx93-11x11-lpddr4x-evk-sd.bin-flash_singleboot of=/dev/sdx bs=1k seek=32 conv=fsync
```

