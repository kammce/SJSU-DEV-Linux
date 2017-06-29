Getting Started
=================

Prerequisites
---------------
RoverCore was built to run on Ubuntu and on Embedded Linux Platforms. Your best option is to run the most recent LTS (L.ong T.erm S.ervice) Ubuntu on your machine or install it on a virtual machine.

.. note::
	The main features of RoverCore will work on Windows and Mac OSX but the install.sh will not work. You will need to manually install all of the necessary components in the installer script.

Installation
-------------

**Step 1**
	Clone the repository

	.. code-block:: bash

		git clone https://github.com/kammce/RoverCore-S.git

**Step 2**
	Change directory into **RoverCore-S**

	.. code-block:: bash

		cd RoverCore-s

**Step 3**
	Install  RoverCore-S using `install.sh` script. The script will install the latest `node.js@6.x.x`, `npm`, node dependencies and development tools according to *package.json*.

	.. code-block:: bash

		./install.sh

	.. warning::

		Do not run this script using **SUDO**. The script will ask you for **sudo** permissions once it runs.

	.. note::
		It is currently planned to add an --production flag to keep from installing the development tools.

Running RoverCore-S
:::::::::::::::::::::
To run RoverCore-S use the following command::

	[sudo] node RoverCore

.. note::
	Use `sudo` if you are using I2C or Bluetooth or anything else that requires root permissions.

To get more information about the command line arguments run::

	node RoverCore -h

RoverCore-S manual output::

	NAME
		RoverCore - Start RoverCore

	SYNOPSIS
		node RoverCore [-h]
		node RoverCore [-t http://address:port] [-s]

	OPTIONS
		-h
			this parameter returns manual information

		-t, --target    http://address:port
			This parameter sets the address of the Primus.js server that
			RoverCore will communicate with.
			Defaults to http://localhost:9000.

		-s, --simulate
			This parameter will replace every module with empty version
			in the modules folder with a Protolobe module. The Protolobe
			will have the name and idle charateristics of the module it
			is replacing. This is useful for testing communication
			between interface and modules. Data sent to protolobe will
			be echoed back to the server and sent to stdout (console).

		-i, --isolate "module" | "module1,moduel2,..."
			Isolate a particular lobe. For a single module, you need
			only put in the name. List of lobes must be comma
			seperated list without spaces.

		--no-color
			Disable log coloring in RoverCore.

		-v, -vv, -vvv
			Verbose output.
			-v will show debug level 1 messages.
			-vv will show debug level 1 and 2 messages.
			-vvv will show debug levels 1, 2 and 3 messages.

Connecting to RoverCore-S using RoverCore-MC
::::::::::::::::::::::::::::::::::::::::::::::
.. note::
	If you do not have RoverCore-MC setup on your machine, follow this guide for setting up

#. Open RoverCore-MC. The page should default to the "Test" interface.
#. Click the Server combo box on the lower right side of the interface and select **localhost / 127.0.0.1:9000**
#. The Server Combobox and Test Navigation bar should turn green, indicating that you are connected.

At this point, you can send JSON to Protolobe using the interface.

Protolobe loop back the data and send it to STDOUT as well as to the mission control test interface.

At this point, you have the stack setup.
The next thing you want to do is start creating your own Lobes.