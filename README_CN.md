[English](./README.md) | [简体中文](./README_CN.md)

### 系统SDK下载

- Yocto-Linux 6.12.3_1.0.0
  https://www.nxp.com/design/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX?
  
  

> 下面是关于如何在yocto工程下，编译uboot

### 修改sources/meta-imx/meta-bsp/recipes-bsp/u-boot/u-boot-imx_2024.04.bb

```shell
UBOOT_SRC ?= "git://github.com/debix-tech/uboot-nxp-debix;protocol=https"
SRCBRANCH = "yocto_L6.12.3-debix_model_ab_2gbddr"
SRCREV = " ... commit id ... "
```

`SRCREV`可以通过git hub上面的commit获取或者通过`git log`命令获取:

```shell
debix@polyhex:~/workstation/Github/uboot-nxp-debix$ git checkout yocto_L6.12.3-debix_model_ab_2gbddr
Already on 'yocto_L6.12.3-debix_model_ab_2gbddr'
Your branch is up to date with 'origin/yocto_L6.12.3-debix_model_ab_2gbddr'.

```

其中`32a28bda1e7f995703d828cf4891f86c905b4232`就是commit id



### 编译uboot

```shell
DISTRO=imx-desktop-xwayland MACHINE=imx8mpevk source imx-setup-desktop.sh -b debix-desktop
bitbake -c compile -f -v u-boot-imx
bitbake -c deploy -f -v u-boot-imx
bitbake -c compile -f -v imx-boot
bitbake -c deploy -f -v imx-boot
```


生成的uboot bin file路径: `debix-desktop/tmp/deploy/images/imx8mpevk/imx-boot-imx8mpevk-sd.bin-flash_evk`

### 使用`dd`命令去烧录

```shell
sudo dd if=imx-boot-imx8mpevk-sd.bin-flash_evk of=/dev/sdx bs=1k seek=32 conv=fsync
```

