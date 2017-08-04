#!/bin/sh

# Binary name
echo "SolarThermoPro" >  ./../../src/binary_name

# App icon
cp ./resources/SolarThermoPro.ico ./../../Debug/appico.ico

# Resources file
cp resources_linux.qrc ./../../Debug/resources.qrc
