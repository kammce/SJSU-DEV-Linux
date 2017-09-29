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

<p align="center">
<img src="http://design.ubuntu.com/wp-content/uploads/ubuntu-logo32.png" height="100px"/>
&nbsp;&nbsp;
<img src="http://cdn.osxdaily.com/wp-content/uploads/2010/10/giant-apple-logo-bw.png" height="100px" />
&nbsp;&nbsp;
<img src="https://cdn.worldvectorlogo.com/logos/microsoft-windows-22.svg" height="100px" />
</p>

Built for **Debian** based systems, **Mac OSX** (beta), and **Windows 10** using the latest Windows Linux Subsystem (WLS) Insider Builds. The instructions to install on Windows BASH are the same as in Linux.

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

### Update 09.11.2017

* Added **SJOne_lpc1758_rev4.pdf**


<!--

apt-get install python-sphinx
pip install sphinx

sudo pip install breathe
sudo apt-get install doxygen

https://github.com/Velron/doxygen-bootstrapped

-->
