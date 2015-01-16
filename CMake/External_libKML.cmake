message(STATUS "Setup libKML ...")

set(proj LIBKML)

set(DEFAULT_USE_SYSTEM_LIBKML  OFF)

option(USE_SYSTEM_LIBKML "  Use a system build of libKML." ${DEFAULT_USE_SYSTEM_LIBKML})
mark_as_advanced(USE_SYSTEM_LIBKML)

if(USE_SYSTEM_LIBKML)
  message(STATUS "  Using libKML system version")
else()
  set(${proj}_DEPENDENCIES)
  set(LIBKML_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(LIBKML_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_EXPAT)
    set(LIBKML_SB_EXPAT_CONFIG)
  else()
    set(LIBKML_SB_EXPAT_CONFIG 
      -DEXPAT_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DEXPAT_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libexpat.so
      )
    list(APPEND ${proj}_DEPENDENCIES EXPAT)
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "https://libkml.googlecode.com/files/libkml-1.2.0.tar.gz"
    URL_MD5 25d534437f971bf55a945608e39fea79
    BINARY_DIR ${LIBKML_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      ${LIBKML_SB_EXPAT_CONFIG}
    #CMAKE_COMMAND
    #  # use 'env' because CTest launcher doesn't perform shell interpretation
    #  env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    #  ${CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory 
      ${CMAKE_SOURCE_DIR}/patches/${proj}
      ${LIBKML_SB_SRC}
    )
  
  message(STATUS "  Using libKML SuperBuild version")

endif()