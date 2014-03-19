SJSU-DEV-Linux
==============

Tools
-----------
 - **Hyperload.exe**: Used to upload programs to SJSU One Board.
	* Requires: Windows OS or Mono C# runtime environment (cross platform)
 - **Hercules.exe**: Used as a serial terminal to interact with SJSU One Board
    * Requires: Windows OS or wine (Linux) or darwin (Mac OS X)
 - **Gtkterm**: Serial monitor for *nix systems.
 - **Minicom**: Commandline text Serial monitor for *nix based systems.
 - **Putty**: Serial monitor for *Nix based systems and Windows.
 - **Mono**: C# runtime environment used to run C# applications
    * To Download: http://www.go-mono.com/mono-downloads/download.html

Setup
------------
 1. Install Mono runtime environment.
 2. Install one or more Serial monitoring software. 
 3. Done! Now you have everything setup for development!

Directory Layout
----------------------
 - **Projects:** holds all of the projects. 
    - The path to the main.cpp file to edit for each project is located at `Projects/*insert project name*/L5_Application/main.cpp`
 - **gcc-arm-none-eabi-*:** Binaries needed to compile the SJSU One sources code to ARM compatiable hex files. *Path to this folder is added when building using build.sh*
 - **ref_and_datasheets:** References and Datasheets about the SJSU One board, source code and documentation.

Getting Started
----------------------
 1. Write your program to the: 
        open Projects/*insert project name*/L5_Application/main.cpp

 2. Build your executable:
		Goto: Projects/*insert project name*/L5_Application/_Build
		Run : build.sh
 3. Load your executable to your board:
		Run: mono Hyperload.exe
        #In Hyperload:
		Open: Projects/*insert project name*/L5_Application/_Build/Executables/{program name here}.hex
		Select:	Appropriate Port
		Select:	Speed (default 38400 - 11520)
		Click:	Send reset
 4. To view output of board:
        Use: any one of the Serial monitor programs
        Recommended: Hercules & Gtkterm

Version
-------
0.1

**Hope this helps!**

K.A.M.M.C.E. SJSU DEV brought to you by
[Khalil Estell](http://kammce.io)
    