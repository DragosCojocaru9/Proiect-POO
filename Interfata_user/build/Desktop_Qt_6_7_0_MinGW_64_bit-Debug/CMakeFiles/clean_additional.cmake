# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DocumenteBatalionApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DocumenteBatalionApp_autogen.dir\\ParseCache.txt"
  "DocumenteBatalionApp_autogen"
  )
endif()
