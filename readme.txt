SJSU DEVK root FOLDER
=======================================================
Table of Contents
* Quick Start
* Editing Makefile

* Quick Start *
	To write your program:
		Open:	L5_Application/main.cpp
	To Build your executable:
		Goto:	_Build
		Run:	Build.sh
	To Load your executable to your board:
		Run: Hyperload (using mono, from root SJSU DEVK folder)
		Open (in Hyperload):	_Build/Executables/{program name here}.hex
		Select (in Hyperload):	Port
		Select (in Hyperload):	Speed (default 38400)
		Click (in Hyperload):	Send reset
	To view output of board:
		Run: Hercules (using wine)
		[Alternatives]
		Run: screen {device location} {BAUD Rate} 
		OR
		<insert more here>