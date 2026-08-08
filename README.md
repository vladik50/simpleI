# simpleI — Simple IP Info

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A lightweight terminal tool that shows your local and external IP addresses.  
No dependencies, no curl, one command.

## Features

- Shows local IP address
- Shows external IP address
- Optional --geo flag for provider, country, region, and city info
- Zero dependencies: works on any Linux distribution

## Getting Started

Choose the method that fits your system.

### Option 1: Install a Pre-built Package

The easiest way. Download the package for your distribution from the [Releases](https://github.com/vladik50/simpleI/releases) page and install it.

**Debian / Ubuntu / Linux Mint:**
sudo dpkg -i simplei_1.0-1_amd64.deb

**Fedora / RHEL:**
sudo dnf install simplei-1.0-1.fcXX.x86_64.rpm

**Arch Linux / Manjaro:**
sudo pacman -U simplei-1.0-1-x86_64.pkg.tar.zst

After installation, run:
simplei

### Option 2: Build from Source

**Requirements**
- g++ (GCC) with C++17 support
- make

**Clone and build**
git clone https://github.com/vladik50/simpleI.git
cd simpleI
make

**Run without installing**
./simplei
./simplei --geo

**Install system-wide (optional)**
sudo make install

To uninstall:
sudo make uninstall

**Compile manually**
g++ -std=c++17 -O2 -o simplei simplei.cpp
./simplei

## Usage

Launch the tool:
simplei

**Basic:**
$ simplei

Local IP:    192.168.1.42
External IP: 203.0.113.15

**With geo info:**
$ simplei --geo

Local IP:    192.168.1.42
External IP: 203.0.113.15
Provider:    AS12345 Example ISP
Country:     RU
Region:      Moscow
City:        Moscow

## License

GNU GPL v3.0 or later. See LICENSE for full text.
