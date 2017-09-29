# SJSU-DEV-Linux

[![SJSU-Dev-Linux Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/kammce/SJSU-DEV-Linux)
[![GNU General Public License v2.0 Badge](https://img.shields.io/badge/license-GNUv2.0-blue.svg)](https://github.com/kammce/SJSU-DEV-Linux)
[![Documentation Status](https://readthedocs.org/projects/sjsu-dev-linux/badge/?version=latest)](http://sjsu-dev-linux.readthedocs.io/en/latest/?badge=latest)
[![Build Status](https://travis-ci.org/kammce/SJSU-DEV-Linux.svg?branch=master)](https://travis-ci.org/kammce/SJSU-DEV-Linux)

## NOTICE!

**THIS BRANCH WILL BE DEPRECATED AND NO LONGER SUPPORTED AS OF DECEMBER 25TH 2017, PLEASE MOVE TO THE LINK BELOW:**

	https://github.com/kammce/SJSU-Dev

### Why this change?

Here are the features of SJSU-Dev:

* Includes mechanisms for unit testing with `Catch` and `CGreen`.
* The framework is more flexible and allows firmware packages to be copied or moved to other locations and still be able to be compiled and allow for the tools  reference tools from
* The file hierarchy has been simplified and flattened.
* Compilation, flashing and testing is all done through a make file interface
	* make build - to build application
	* make flash - flash SJ-One board, builds application if it is not built
	* make telemetry - launch telemetry serial monitor and real time firmware variable inspector
	* make test - (Only exists in dedicated test folder within firmware package) build and runs tests
	* make cgreen - (Only exists in dedicated test folder within firmware package) build and runs cgreen tests
* Most importantly, it supports the Mac OSX, Windows and Linux operating systems which is why the repo (and by extension the name) has been changed.


## Operating System Supported

Built for **Debian** based system.

## Tutorial

See **[documentation](http://sjsu-dev-linux.readthedocs.io/en/latest/?badge=latest)** for a full tutorial.

## Contrib
* [Preet Kang](http://www.socialledge.com/sjsu/index.php?title=Main_Page): Original creator of the SJDev development framework.
* [Khalil Estell](http://kammce.io): Creator of the Linux port of SJDev and maintainer of this repo.
* [Kai Wetlesen](https://github.com/kaiwetlesen): Contributed Mac OS X port and development environment improvements.

### Special Credits
* **Mikko Bayabo**: Windows surface destructive testing
* **WSL testing**: Sameer Azer, Aaron Moffit, Ryan Lucus

## Change Log

### Update 09.28.2017

* Added support for Windows 10 WLS and Mac OS X (beta)
* Setup script no longer pollutes global space (except for gdbgui)
* Setup script now generates an environment script to set environment variables
* Massive change to file hierarchy
	* Firmware folders can be moved to any other location as long as it has a link to the development
	* Default firmware folders come with a symbolic link to `env.sh` and `makefile`.
	* There is no longer a defaults folder, application folders were moved to firmware folder.

### Update 09.11.2017

* Added **SJOne_lpc1758_rev4.pdf**


<!--

apt-get install python-sphinx
pip install sphinx

sudo pip install breathe
sudo apt-get install doxygen

https://github.com/Velron/doxygen-bootstrapped

-->
