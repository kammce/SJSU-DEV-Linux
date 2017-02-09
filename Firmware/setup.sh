#!/bin/bash

echo "Starting SJSU-DEV-Linux Environment Setup Script"

echo "Installing gcc-arm-embedded"
add-apt-repository -y ppa:team-gcc-arm-embedded/ppa
apt-get update
apt-get -y install gcc-arm-embedded

echo "Installing gtkterm"
sudo apt-get -y install gtkterm

echo "Adding current user to 'dialout' group"
sudo adduser $USER dialout

echo "Installing Mono-Complete"
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb http://download.mono-project.com/repo/debian wheezy main" | sudo tee /etc/apt/sources.list.d/mono-xamarin.list
sudo apt-get update
sudo apt-get -y install mono-complete

echo "Setup complete!"
echo
echo "PLEASE LOGOUT AND BACK IN to load code into your SJOne Board."