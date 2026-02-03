# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/_deps/picotool-src"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/_deps/picotool-build"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/_deps"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/tmp"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/src/picotoolBuild-stamp"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/src"
  "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sreer/Junior_Design/Microcontroller_Basics/c_files/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
