#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "image_proc::resize" for configuration ""
set_property(TARGET image_proc::resize APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(image_proc::resize PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libresize.so"
  IMPORTED_SONAME_NOCONFIG "libresize.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS image_proc::resize )
list(APPEND _IMPORT_CHECK_FILES_FOR_image_proc::resize "${_IMPORT_PREFIX}/lib/libresize.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
