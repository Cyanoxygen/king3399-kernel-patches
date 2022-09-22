# Rongpin board specific driver for mainline Linux kernel

Status
======

- [x] Out-of-tree kernel modules which brings up LED and the fan
- [x] Wi-Fi
- [ ] Bluetooth
- [x] Sound (Sound card can be detected but can not play sound)

Installation
======

1. Clone this tree

```
git clone https://github.com/Cyanoxygen/king3399-kernel-patches
```

2. Fetch Linux kernel source

```
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.19.9.tar.xz
tar Jxf linux-5.19.9.tar.xz
```

3. Add files into the mainline tree

```
pushd king3399-kernel-patches
find . -mindepth 2 -not -path '*/\.*' -type f -exec install -Dv {} ../linux-5.19.9/{} \;
popd
```
4. Apply patches

```
pushd linux-5.19.9
find ./patches -type f -name '*.patch' -print0 | xargs -0 -n1 patch -p1 -i
popd
```

5. Build kernel

```
cd linux-5.19.9
make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is king3399_defconfig
make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is -j$(nproc)
```

6. Install kernel modules

Note: depends on your target directory, you may want to add `sudo` to the start of the command.

```
make ARCH=arm64 INSTALL_MOD_PATH=/whatever/you/want/to/install modules_install
```

7. Copy kernel image file

The built kernel is at `arch/arm64/boot/Image`.  
You can run `make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is Image.gz` to build gzip compressed kernel.

The compiled Flattened Device Tree is at `arch/arm64/boot/dts/rockchip/rk3399-king3399.dtb`.  
You may have to copy the kernel and this dtb to a partition readable to U-Boot, e.g. the `boot` partition appearing in Rockchip download tool.

8. Booting the new kernel

The stock U-Boot can boot this kernel directly.

You need to generate a U-Boot script manually:

```sh
cat > boot.cmd << EOF
load mmc 0:7 0x02080000 Image
load mmc 0:7 0x01f00000 dtb.dtb
fdt addr 0x01f00000
setenv bootargs console=ttyS2,115200 root=/dev/mmcblk1p8 rootwait
booti 0x02080000 - 0x01f00000
EOF

mkimage -A arm -T script -C none -d boot.cmd boot.scr

cp arch/arm64/boot/Image ${BOOT_PART}/Image
cp arch/arm64/boot/dts/rockchip/rk3399-king3399.dtb ${BOOT_PART}/dtb.dtb
cp boot.scr ${BOOT_PART}/boot.scr
```

As for booting with USB media, use the following script (note the difference between this and the one above):

```
load usb 0:X 0x02080000 Image
load usb 0:X 0x01f00000 dtb.dtb
fdt addr 0x01f00000
setenv bootargs console=ttyS2,115200 root=/dev/sda rootwait
booti 0x02080000 - 0x01f00000
```

Where X is the boot partition of your USB drive.

Notes
======

### Booting from USB storage with stock U-Boot

The stock U-Boot can boot from USB storage, but it is configured to boot from eMMC first.

To boot from USB storage, Serial UART access is required. Please connect your serial UART to the board first. The default baud rate is 115200, 8N1.

1. Power on the board, hit Ctrl-C repeatedly until you see the `=>` prompt.
2. Run `usb start`.
3. U-Boot is now scanning for USB devices. You can see the number of USB storage devices detected in the output.
4. U-Boot should detected your USB storage device, if not, plug it in a different port, and run `usb reset`.
5. When U-Boot detects your USB storage, run `bootcmd_usb0`. The board will boot into the new kernel.
6. Make sure your `boot.scr` is booting from USB, not eMMC.

Mainline U-Boot is work in progress. Almost got the board bricked.
