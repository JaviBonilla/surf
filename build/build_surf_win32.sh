#!/bin/sh

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")

# Absolute path this script is in, thus /home/user/bin
PATHS=$(dirname "$SCRIPT")
echo $PATHS

# Surf source path
SURF_SRC="$PATHS/../src"

# ----------------------------------------------------------------------------- #
# User defined directory for MXE                                                 #
# Or add mxe/usr/bin to path                                                      #
# ----------------------------------------------------------------------------- #
MXE_SRC="$PATHS/../../ThirdParty/src/mxe/"
# ----------------------------------------------------------------------------- #

# Include MXE to PATH
export PATH="$MXE_SRC/usr/bin":$PATH

# Example directory
PATHB=$PATHS/../bin/win32/$1/

# Create example and build directory
mkdir -p "$PATHB/obj"

# Copy resources file
cp $2 "$PATHB/obj/resources.qrc"

# Absolute path to the resource file
RESOURCE=$(readlink -f "$2")

# Absolute path the resource file is in
PATHR=$(dirname "$RESOURCE")

# Substitute in resource file the tag #CURRENT_DIR# for the path where the resource file is
sed -i "s|#CURRENT_DIR#|${PATHR}|g" "$PATHB/obj/resources.qrc"

# Copy icon file
cp $3 "$SURF_SRC/appico.ico"

# Set binary name (without extension)
name=$(echo $4 | cut -f 1 -d '.')
echo $name >  "$SURF_SRC/binary_name"

# Build path
cd "$PATHB/obj/"

# QMake
"i686-w64-mingw32.static-qmake-qt5" "$SURF_SRC/surf.pro" "CONFIG+=staticlib" -spec win32-g++

# Make
make

# Copy binary
cp $PATHB/obj/release/$4 $PATHB/$4
