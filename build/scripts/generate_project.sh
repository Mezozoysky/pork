#! /usr/bin/env bash

BUILD_SCRIPTS_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_TOP_DIR="$(dirname "$BUILD_SCRIPTS_DIR")"
PROJECT_TOP_DIR="$(dirname "$BUILD_TOP_DIR")"
CMAKE_SCRIPTS_DIR="$PROJECT_TOP_DIR/cmake/scripts"

cmake -DPROJECT_TOP_DIR:PATH="$PROJECT_TOP_DIR"\
      -DBUILD_TOP_DIR:PATH="$BUILD_TOP_DIR"\
      -P"$CMAKE_SCRIPTS_DIR/generate_project.cmake"


