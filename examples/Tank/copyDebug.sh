#!/bin/sh

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")

# Absolute path this script is in, thus /home/user/bin
PATHS=$(dirname "$SCRIPT")

# Binary name
echo "TankSim" >  "$PATHS/../../src/binary_name"

# Copy icon
cp "$PATHS/resources/appico.ico" "$PATHS/../../Debug/"

# Copy resource file
cp "$PATHS/resources_linux.qrc" "$PATHS/../../Debug/resources.qrc"

# Substitute in resource file the tag #CURRENT_DIR# for the path where the resource file is
sed -i "s|#CURRENT_DIR#|${PATHS}|g" "$PATHS/../../Debug/resources.qrc"
