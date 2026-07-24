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

### Option 1: Build from Source

Requirements:
  - g++ (GCC) with C++17 support
  - make

Clone and build:

  git clone https://github.com/vladik50/simpleI.git
  cd simpleI
  make

Run without installing:

  ./simplei
  ./simplei --geo

Install system-wide (optional):

  sudo make install

This copies simplei to /usr/local/bin. You can then run it from anywhere:

  simplei

To uninstall:

  sudo make uninstall

### Option 2: Compile Manually

  g++ -std=c++17 -O2 -o simplei simplei.cpp
  ./simplei

## Usage

Basic:

  $ simplei
  
  Local IP:    192.168.1.42
  External IP: 203.0.113.15

With geo info:

  $ simplei --geo
  
  Local IP:    192.168.1.42
  External IP: 203.0.113.15
  Provider:    AS12345 Example ISP
  Country:     RU
  Region:      Moscow
  City:        Moscow

## License

GNU GPL v3.0 or later. See LICENSE for full text.
