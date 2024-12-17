#!/bin/bash

echo "Starting the installation process..."

echo "Step 1: Updating the package list..."
sudo apt-get update

echo "Step 2: Installing libsoundio-dev package..."
sudo apt-get -y install libsoundio-dev

echo "Step 3: Installing ROS Humble Xacro..."
sudo apt install -y ros-humble-xacro

echo "Step 4: Installing ROS Humble Joint State Publisher..."
sudo apt install -y ros-humble-joint-state-publisher

echo "Step 5: Installing libusb-1.0-0-dev..."
sudo apt-get install -y libusb-1.0-0-dev

# Download and install libk4a1.4 package
echo "Step 6: Downloading and installing libk4a1.4 package..."
wget -q https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4/libk4a1.4_1.4.2_amd64.deb -O libk4a1.4_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Error: Failed to download libk4a1.4 package. Exiting..."
  exit 1
fi

sudo dpkg -i libk4a1.4_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Resolving dependencies for libk4a1.4..."
  sudo apt-get install -f -y
fi

# Download and install libk4a1.4-dev package
echo "Step 7: Downloading and installing libk4a1.4-dev package..."
wget -q https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4-dev/libk4a1.4-dev_1.4.2_amd64.deb -O libk4a1.4-dev_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Error: Failed to download libk4a1.4-dev package. Exiting..."
  exit 1
fi

sudo dpkg -i libk4a1.4-dev_1.4.2_amd64.deb
if [ $? -ne 0 ]; then
  echo "Resolving dependencies for libk4a1.4-dev..."
  sudo apt-get install -f -y
fi

# Clean up .deb files
echo "Step 8: Cleaning up temporary files..."
rm -f libk4a1.4_1.4.2_amd64.deb libk4a1.4-dev_1.4.2_amd64.deb

# Initialize and update submodules
echo "Step 9: Initializing and updating git submodules..."
git submodule update --init --recursive

# Copy udev rules file
UDEV_RULES_FILE="/home/$USER/colcon_ws/src/azure_kinect_sensor_sdk/scripts/99-k4a.rules"
DESTINATION_DIR="/etc/udev/rules.d/"
echo "Step 10: Copying udev rules file to $DESTINATION_DIR..."
if [ -f "$UDEV_RULES_FILE" ]; then
  sudo cp -f "$UDEV_RULES_FILE" "$DESTINATION_DIR"
  echo "udev rules file copied successfully."
else
  echo "Error: udev rules file not found at $UDEV_RULES_FILE. Exiting..."
  exit 1
fi

# Update udev rules and trigger
echo "Step 11: Reloading udev rules..."
sudo /usr/lib/systemd/systemd-udevd --daemon
sudo udevadm control --reload-rules
sudo udevadm trigger

# Reload environment variables
echo "Step 12: Reloading bash configuration..."
source ~/.bashrc

echo "Installation completed successfully."
