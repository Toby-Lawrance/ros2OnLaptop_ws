#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "image_proc::rectify" for configuration ""
set_property(TARGET image_proc::rectify APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(image_proc::rectify PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/librectify.so"
  IMPORTED_SONAME_NOCONFIG "librectify.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS image_proc::rectify )
list(APPEND _IMPORT_CHECK_FILES_FOR_image_proc::rectify "${_IMPORT_PREFIX}/lib/librectify.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
