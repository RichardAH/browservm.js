# Disclaimer
BrowserVM's emulation engine is based on QEMU.

The following points clarify the QEMU licenses.
QEMU as a whole is released under the GNU General Public License, version 2.
Parts of QEMU have specific licenses which are compatible with the GNU General Public License, version 2. 
Hence each source file contains its own licensing information.
Source files with no licensing information are released under the GNU General Public License, version 2 or (at your option) any later version.
As of July 2013, contributions under version 2 of the GNU General Public License (and no later version) are only accepted for the following files or directories: bsd-user/, linux-user/, hw/misc/vfio.c, hw/xen/xen_pt*.
The Tiny Code Generator (TCG) is released under the BSD license (see license headers in files).
QEMU is a trademark of Fabrice Bellard.

# Browservm.js
BrowserVM is an efficient X86-64 full-system emulator running in browsers. It emulates commonly-used hardware such as
1. Console
2. Hard disk (Experimental)
3. Ethernet (Experimental)
4. 3D Graphics (Not yet available)

You can check our the demo page on https://elliott-wen.github.io/browservm.demo/

Use python in your browser today!

## How fast is Browservm.js?
Browservm **in the interpretation mode** is slow. A modern CPU has a single-core performance 300 times faster than Browservm.
Still, we argue that Browservm can enable many exciting applications, such as online Debugger or Docker in browsers.

Browservm **in the JIT mode** can be decently fast. 
Our recent Dhrystone test shows that BrowserVM (internal experimental build) is solely 2.5 times slower than QEMU. It is roughly 36 times slower than a native CPU.
Still, JIT mode is highly experimental and not yet publicly available. But we enable JIT on our demo page so that you can play around with it. 
JIT mode requires many experimental features of WebAssembly, especially tail-call optimization. 
These features may not be available in your browser. In this case, BrowserVM will fall back to interpretation.
To run a benchmark on Browservm, you can try the following command on our demo page.
```
time dhrystone 20000000
```

## How to compile Browservm
Browservm is implemented in C and compiled to WebAssembly with Emscripten. 
To get yourself started, install Emscripten.
```
# Get the emsdk repo
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory
cd emsdk

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
```

Next, get the source code of Browservm and run the build script.
```
git clone https://github.com/elliott-wen/browservm.js
cd browservm.js
make -j6
```

Finally, run a webserver to host the Browservm.
```
python3 wasm.py
```

Now, you can go to http://localhost:8080/ to play around with Browservm.

## How to mount a persistent harddisk.
We provide an Indexeddb-backed persistent virtual hard disk. 
For a first time user, init the hard disk as follows. (Redo this command may destroy your data).
```
mkfs.ext4 /dev/vdb
```
After that, you can mount the hard disk as follows.
```
mkdir /data
mount /dev/vdb /data
```
We intend to automate this process by init scripts. Maybe you can offer help?

## Build your own Linux kernel and system image.
BrowserVM is a general x86-64 full system emulator, so we do not impose any requirements on the operating systems and software.

To build your Linux kernel, just download a Linux kernel and tweak the config. 
The only config you need to enable is the virtio-related drivers (e.g., virito-input, virtio-console, and virito-block)
```
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.8.18.tar.xz
tar xvf linux-5.8.18.tar.xz
cd linux-5.8.18.tar.xz
make menuconfig
make
cp arch/x86/boot/bzImage /path-to-browservm/vmlinux.bin
```

To build your system image, we recommend using Buildroot
```
git clone https://github.com/buildroot/buildroot.git buildroot
cd buildroot
make menuconfig
make
cp output/images/rootfs.ext2 /path-to-browservm/rootfs.ext2
```
**Todo to support a image larger than 8MB, you need to tweak the main.c file, maybe a help from you?. **

# Acknowledgement and License
BrowserVM makes use of many codes from QEMU (https://www.qemu.org/) and TinyEMU (https://bellard.org/jslinux/).
The code in this repository is released under the GNU AFFERO GENERAL PUBLIC LICENSE, version 3. A copy can be found in the LICENSE file. Copyright (c) SwiftLab, 2018-2020.
