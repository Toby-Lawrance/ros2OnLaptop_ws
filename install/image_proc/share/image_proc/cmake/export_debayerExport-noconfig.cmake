#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "image_proc::debayer" for configuration ""
set_property(TARGET image_proc::debayer APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(image_proc::debayer PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libdebayer.so"
  IMPORTED_SONAME_NOCONFIG "libdebayer.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS image_proc::debayer )
list(APPEND _IMPORT_CHECK_FILES_FOR_image_proc::debayer "${_IMPORT_PREFIX}/lib/libdebayer.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
