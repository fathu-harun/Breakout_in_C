#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WavPack::WavPack" for configuration ""
set_property(TARGET WavPack::WavPack APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(WavPack::WavPack PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "ASM;C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libwavpack.a"
  )

list(APPEND _cmake_import_check_targets WavPack::WavPack )
list(APPEND _cmake_import_check_files_for_WavPack::WavPack "${_IMPORT_PREFIX}/lib64/libwavpack.a" )

# Import target "WavPack::wavpackapp" for configuration ""
set_property(TARGET WavPack::wavpackapp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(WavPack::wavpackapp PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/wavpack"
  )

list(APPEND _cmake_import_check_targets WavPack::wavpackapp )
list(APPEND _cmake_import_check_files_for_WavPack::wavpackapp "${_IMPORT_PREFIX}/bin/wavpack" )

# Import target "WavPack::wvunpack" for configuration ""
set_property(TARGET WavPack::wvunpack APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(WavPack::wvunpack PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/wvunpack"
  )

list(APPEND _cmake_import_check_targets WavPack::wvunpack )
list(APPEND _cmake_import_check_files_for_WavPack::wvunpack "${_IMPORT_PREFIX}/bin/wvunpack" )

# Import target "WavPack::wvgain" for configuration ""
set_property(TARGET WavPack::wvgain APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(WavPack::wvgain PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/wvgain"
  )

list(APPEND _cmake_import_check_targets WavPack::wvgain )
list(APPEND _cmake_import_check_files_for_WavPack::wvgain "${_IMPORT_PREFIX}/bin/wvgain" )

# Import target "WavPack::wvtag" for configuration ""
set_property(TARGET WavPack::wvtag APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(WavPack::wvtag PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/wvtag"
  )

list(APPEND _cmake_import_check_targets WavPack::wvtag )
list(APPEND _cmake_import_check_files_for_WavPack::wvtag "${_IMPORT_PREFIX}/bin/wvtag" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
