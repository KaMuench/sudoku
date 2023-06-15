# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Aufgabe_X_autogen"
  "CMakeFiles\\Aufgabe_X_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Aufgabe_X_autogen.dir\\ParseCache.txt"
  )
endif()
