# SJSU-DEV-Linux #

## Dependencies ##
 - **Mono**: C# runtime environment used to run C# applications
    * To Download: http://www.go-mono.com/mono-downloads/download.html
 - **Wine**: Wine (originally an acronym for "Wine Is Not an Emulator") is a compatibility layer capable of running Windows applications on several POSIX-compliant operating systems, such as Linux, Mac OSX, & BSD.
 - **make**: GNU make utility to maintain groups of programs.

# Software to Load SJONE Board #
 - **Hyperload.exe**: Used to upload programs to SJSU One Board.
	* Requires: Windows OS or Mono C# runtime environment (cross platform)

## Serial Monitors ##
The SJONE and many other micro-controllers communicate with host machines via serial. Serial can take many forms such as UART, RS485 or USB. The SJONE board communicates serially through USB. In order to view the Serial output of the device you can use the following tools:
 - **Hercules.exe**: Used as a serial terminal to interact with SJSU One Board
    * Requires: Windows OS or wine (Linux) or darwin (Mac OS X). NOT RECOMMENDED.
 - **Arduino IDE**: The Arduino IDE has a serial monitor program. You can select your /dev/ttyUSB0 device and use their monitor to communicate with the device.
 - **Putty**: A cross platform software tool for SSH and telnet but can be used as a serial monitor.
 - **Screen**: Command line Serial monitor for *Nix based systems.

## Setup ##
 1. Install Mono runtime environment.
    Follow instructions here: [Mono Installer Page](http://www.mono-project.com/docs/getting-started/install/linux/#usage).

 2. (Optional) Install Wine runtime environment. [Wine Installer Page](https://wiki.winehq.org/Ubuntu)
 3. Install [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded/+download) For linux
 	Add the **<path to directory>/gcc-arm-none-eabi-5_4-2016q2/bin** to your path
 	You can add it to your PATH automatically in your shell by editing your ~/.bashrc file using the line below
 	```
 	PATH=$PATH:<path to directory>/gcc-arm-none-eabi-5_4-2016q2/bin
 	```
 	DO NOT INSTALL USING APT-GET!!!
 4. Install one or more Serial monitoring software.
 5. Done! Now you have everything setup for development!


## Directory Layout ##

 - **Projects:** holds all of the projects.
    - The path to the main.cpp file to edit for each project is located at `Projects/*insert project name*/L5_Application/main.cpp`
 - **gcc-arm-none-eabi-*:** Binaries needed to compile the SJSU One sources code to ARM compatiable hex files. *Path to this folder is added when building using build.sh*
 - **ref_and_datasheets:** References and Datasheets about the SJSU One board, source code and documentation.

## Getting Started ##

 1. Write your program to the:
        open Projects/*insert project name*/L5_Application/main.cpp

 2. Build your executable:
		Goto: Projects/*insert project name*/L5_Application/_Build
		Run : make all
 3. Load your executable to your board:
		Run: mono Hyperload.exe
        #In Hyperload:
		Open: Projects/*insert project name*/L5_Application/_Build/Executables/{program name here}.hex
		Select:	Appropriate Port
		Select:	Speed (default 38400 - 11520)
		Click:	Send reset
 4. To view output of board:
        Use: any one of the Serial monitor programs

## Version ##
1.0

**Enjoy!**

SJSU DEV Linux brought to you by [Khalil Estell](http://kammce.io)
