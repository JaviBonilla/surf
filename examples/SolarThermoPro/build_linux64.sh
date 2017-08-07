#!/bin/sh

# Call general script
# Argument 1 - Build folder
# Argument 2 - Resources file
# Argument 3 - Linux icon (referenced in the desktop file)
# Argument 4 - Executable name
# Argument 5 - AppImage desktop file

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")

# Absolute path this script is in, thus /home/user/bin
PATHS=$(dirname "$SCRIPT")

arg1="SolarThermoPro"
arg2="$PATHS/resources_linux.qrc"
arg3="$PATHS/resources/SolarThermoPro.png"
arg4="SolarThermoPro"
arg5="$PATHS/resources/SolarThermoPro.desktop"

$PATHS/../../build/build_surf_linux64.sh "$arg1" "$arg2" "$arg3" "$arg4" "$arg5"
