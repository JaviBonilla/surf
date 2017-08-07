#!/bin/sh

# Call general script
# Argument 1 - Build folder
# Argument 2 - Resource file
# Argument 3 - Windows icon
# Argument 4 - Executable name

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")

# Absolute path this script is in, thus /home/user/bin
PATHS=$(dirname "$SCRIPT")

# Arguments
arg1="SolarThermoPro"
arg2="$PATHS/resources_windows.qrc"
arg3="$PATHS/resources/SolarThermoPro.ico"
arg4="SolarThermoPro.exe"

$PATHS/../../build/build_surf_win32.sh "$arg1" "$arg2" "$arg3" "$arg4"
