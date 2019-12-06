#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "image_proc::crop_decimate" for configuration ""
set_property(TARGET image_proc::crop_decimate APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(image_proc::crop_decimate PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libcrop_decimate.so"
  IMPORTED_SONAME_NOCONFIG "libcrop_decimate.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS image_proc::crop_decimate )
list(APPEND _IMPORT_CHECK_FILES_FOR_image_proc::crop_decimate "${_IMPORT_PREFIX}/lib/libcrop_decimate.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
