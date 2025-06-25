[English](./README.md) | [简体中文](./README_CN.md)

### 系统SDK下载

- Ubuntu 22.04 :
  
  https://github.com/nxp-imx/meta-nxp-desktop
  
- Yocto-Linux 5.15.71_2.2.0

   https://www.nxp.com/design/software/embedded-software/i-mx-software/embedded-linux-for-i-mx-applications-processors:IMXLINUX?

  

> 下面是关于如何在yocto工程下，编译uboot

### 修改sources/meta-imx/meta-bsp/recipes-bsp/u-boot/u-boot-imx_2021.04.bb

```shell
UBOOT_SRC ?= "git://github.com/debix-tech/uboot-nxp-debix;protocol=https"
SRCBRANCH = "lf_v2022.04-debix_model_a"
SRCREV = " ... commit id ... "
```

`SRCREV`可以通过git hub上面的commit获取或者通过`git log`命令获取:

```shell
ljm@polyhex:~/workstation/Github/uboot-nxp-debix$ git checkout lf_v2022.04-debix_model_a
Branch 'lf_v2022.04-debix_model_a' set up to track remote branch 'lf_v2022.04-debix_model_a' from 'origin'.
Switched to a new branch 'lf_v2022.04-debix_model_a'


ljm@polyhex:~/workstation/Github/uboot-nxp-debix$ git log
commit cf7f1d8c1d3b5b5c2db2b809e1ff1ed79e73ea66 (HEAD -> lf_v2022.04-debix_model_a, origin/lf_v2022.04-debix_model_a)
Author: John_gao <9278978@qq.com>
Date:   Tue Jul 4 11:44:58 2023 +0000

    Modify README.md


```

其中`cf7f1d8c1d3b5b5c2db2b809e1ff1ed79e73ea66`就是commit id



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

