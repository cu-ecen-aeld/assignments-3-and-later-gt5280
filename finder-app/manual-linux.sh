#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.1.10
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi
if [ ! -d "${OUTDIR}" ]
mkdir -p ${OUTDIR}
else
cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; 
then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}

	make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- mrproper
	make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- defconfig
	make ARCH=arm64 -j4 CROSS_COMPILE=aarch64-none-linux-gnu- all
	make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- modules
	make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- dtbs	
	
    # TODO: Add your kernel build steps here
fi 

echo "Adding the Image in outdir"

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs
fi

mkdir rootfs
cd rootfs
mkdir bin dev etc home lib proc sbin sys tmp usr va var
mkdir bin usr/bin usr/sbin usr/lib
mkdir -p var/log
sudo chown -R root:root *

# TODO: Create necessary base directories

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
git clone git://busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
	
	make distclean
	make defconfig
	
    # TODO:  Configure busybox
else
    cd busybox

sudo make ARCH=arm64 CROSS_COMPILE=/usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu- install CONFIG_PREFIX=/home/nero2121/myproject/assignment-1-gt5280/outdir/rootfs

cd ${OUTDIR}/rootfs

# TODO: Make and insatll busybox

echo "Library dependencies"
${CROSS_COMPILE}readelf -a bin/busybox | grep "program interpreter"
${CROSS_COMPILE}readelf -a bin/busybox | grep "Shared library"

sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib/ld-linux-aarch64.so.1 lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/ld-2.31.so lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libresolv.so.2 lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libresolv-2.31.so lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libc.so.6 lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libc-2.31.so lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libm.so.6 lib
sudo cp -a /usr/local/arm-cross-compiler/install/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc/lib64/libm-2.31.so lib


# TODO: Add library dependencies to rootfs

#sudo mknod -m 666 dev/null c 1 3
#sudo mknod -m 600 dev/console c 5 1

# TODO: Make device nodes

#cd ${OUTDIR}/linux-stable

#sudo make -j4 ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- INSTALL_MOD_PATH=/home/nero2121/myproject/assignment-1-gt5280/outdir/linux-stable modules_install

# TODO: Clean and build the writer utility

# TODO: Copy the finder related scripts and executables to the /home directory
# on the target rootfs

 
# TODO: Chown the root directory

#cd ${OUTDIR}/rootfs
#sudo chown -R root:root *
#find . | cpio -H newc -ov --owner root:root > ../initramfs.cpio
#cd ..
#zip initramfs.cpio 


# TODO: Create initramfs.cpio.gz
#QEMU_AUDIO_DRV=none qemu-system-arm -m 256 -nographic -M versatilepb -kernel zImage -append "console=ttyAMA0 rdinit=bin/sh" -dtb versatile-pb.dtb-initrd initramfs.cpio.gz

