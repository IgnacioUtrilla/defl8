#!/bin/bash

: '
  REQUIRED INSTALLATIONS
    1. fswatch: https://github.com/emcrisostomo/fswatch#getting-fswatch

  DESCRIPTION
    PARAMS:
      1. Directory to watch
      2. Directory to the unit tests to compile and run

    This script watch changes in the specified directory and compiles/run the test

  LINKS:
    1. https://github.com/emcrisostomo/fswatch
    2. https://github.com/emcrisostomo/fswatch#getting-fswatch
    3. https://github.com/emcrisostomo/fswatch/issues/212
'

DIR_WATCH=$1;
DIR_TEST=$2;

# Clear screen
# Explanation: \033 == \x1B == 27 == ESC
printf "\033c"

tempFilename="$(dirname $DIR_TEST)/criterion_test";

# Remove temp file before exit
trap "rm -f $tempFilename" INT;

function compileAndRunTest() {
  printf "\033c";
  gcc -o "./$tempFilename" "./$DIR_TEST" -lcriterion;
  ./"$tempFilename";
}

compileAndRunTest;

fswatch -o --event Updated --event OwnerModified "$DIR_WATCH" | while read event; do
  # 5 = OwnerModified, 2 = Updated
  if [ $event -eq 5 ] || [ $event -eq 2 ]; then
    compileAndRunTest;
  fi
done
