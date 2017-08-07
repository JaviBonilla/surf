#!/bin/sh

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")

# Absolute path this script is in, thus /home/user/bin
PATHS=$(dirname "$SCRIPT")

# User defined directories
SURF_SRC="$PATHS/../src"

# Example directory
PATHB=$PATHS/../bin/linux64/$1/

# Create example and build directory
mkdir -p $PATHB/obj

# Absolute path to the resource file
RESOURCE=$(readlink -f "$2")

# Absolute path the resource file is in
PATHR=$(dirname "$RESOURCE")

# Copy resources file
cp $2 "$PATHB/obj/resources.qrc"

# Substitute in resource file the tag #CURRENT_DIR# for the path where the resource file is
sed -i "s|#CURRENT_DIR#|${PATHR}|g" "$PATHB/obj/resources.qrc"

# Set binary name (without extension)
name=$(echo $4 | cut -f 1 -d '.')
echo $name >  "$SURF_SRC/binary_name"

# Build path
cd "$PATHB/obj/"

# QMake
qmake "$SURF_SRC/surf.pro" "CONFIG+=staticlib"  -spec linux-g++

# Make
make

# Create directory for AppImage
mkdir -p "$PATHB/obj/AppImage/"
mkdir -p "$PATHB/obj/AppImage/AppDir/"

# Creat tree structure in AppDir
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/"
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/bin"
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/share"
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/share/icons"
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/share/icons/hicolor"
mkdir -p "$PATHB/obj/AppImage/AppDir/usr/share/applications"

# Copy binary
cp "$PATHB/obj/$4" "$PATHB/obj/AppImage/AppDir/usr/bin/$4"

# Copy AppImage desktop file 
cp "$5" "$PATHB/obj/AppImage/AppDir//usr/share/applications/"

# Copy AppImage icon
cp "$3" "$PATHB/obj/AppImage/AppDir/usr/share/icons/hicolor/"

# Build AppImage path
cd "$PATHB/obj/AppImage"

# Create bundle
fname=${5##*/}
"$PATHS/tools/linuxdeployqt-continuous-x86_64.AppImage" "$PATHB/obj/AppImage/AppDir/usr/share/applications/$fname" -appimage

# Copy AppImage binary file
cp "$PATHB/obj/AppImage/"*".AppImage" "$PATHB"
