#!/bin/sh

# Binary name
echo "HeatTransferBasics" >  ./../../src/binary_name

# App icon
#cp ./resources/appico.ico ./../../Debug/

# Resource file
cp resources_linux.qrc ./../../Debug/resources.qrc
