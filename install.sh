#!/bin/bash

echo "Starting the installation..."

echo "Updating package list..."
sudo apt-get update

echo "Installing libsoundio-dev..."
sudo apt-get -y install libsoundio-dev

echo "Installing ros-humble-xacro..."
sudo apt install -y ros-humble-xacro

echo "Installing ros-humble-joint-state-publisher..."
sudo apt install -y ros-humble-joint-state-publisher

# Download libk4a1.4 package
echo "Downloading libk4a1.4 package..."
wget -q https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4/libk4a1.4_1.4.2_amd64.deb -O libk4a1.4_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Failed to download libk4a1.4 package. Exiting..."
  exit 1
fi

# Download libk4a1.4-dev package
echo "Downloading libk4a1.4-dev package..."
wget -q https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4-dev/libk4a1.4-dev_1.4.2_amd64.deb -O libk4a1.4-dev_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Failed to download libk4a1.4-dev package. Exiting..."
  exit 1
fi

# Install libk4a1.4
echo "Installing libk4a1.4..."
sudo dpkg -i libk4a1.4_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "dpkg installation failed. Resolving dependencies..."
  sudo apt-get install -f
fi

# Install libk4a1.4-dev
echo "Installing libk4a1.4-dev..."
sudo dpkg -i libk4a1.4-dev_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "dpkg installation failed. Resolving dependencies..."
  sudo apt-get install -f
fi

# Clean up .deb files
echo "Cleaning up .deb files..."
rm -f libk4a1.4_1.4.2_amd64.deb libk4a1.4-dev_1.4.2_amd64.deb

# 使わなくて済む方法があれば教えてください！
git submodule update --init --recursive

source ~/.bashrc

echo "Installation completed."
