#!/bin/bash

#
# Collect all mo files for software packing on Linux platform
#
# $Id$
#

# Define configurable variables
InputFolder=../resource/locale
OutputFolder=locale
AppName=PWUpdater
executeBinaryParentPath=../bin

# Remove previous output
rm -rf ${OutputFolder}

# Create a temp file to list all the mo files in the locale folder
for i in $(ls ${InputFolder}/*.mo | awk '{ print $1 }');
do
    lang=$(basename $i)
    lang_dir=${lang%%.mo}
    mkdir -p ${OutputFolder}/${lang_dir}
    cp ${InputFolder}/${lang_dir}.mo ${OutputFolder}/${lang_dir}/${AppName}.mo
done

# Copy output data to execute binary path for test purpose
for i in $(ls ${executeBinaryParentPath} | awk '{ print $1 }');
do
    cp -a ${OutputFolder} ${executeBinaryParentPath}/$i/locale
done

