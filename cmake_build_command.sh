#! /bin/bash

# Run this command in your build directory re-build the simulation of needed
# The second argument passed to cmake should obviously be updated to match
# the location of your source directory containing your CMakeLists.txt

 cmake -DGeant4_DIR=$G4COMP ../ -DOnnxRuntime_LIBRARY=../onnxruntime-linux-x64-1.17.1/lib/libonnxruntime.so.1.17.1

