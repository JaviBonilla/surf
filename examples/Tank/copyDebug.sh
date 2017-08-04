#!/bin/sh

# Binary name
echo "TankSim" >  ./../../src/binary_name

# Copy icon
cp ./resources/appico.ico ./../../Debug/

# Copy resource file
cp resources_linux.qrc ./../../Debug/resources.qrc
