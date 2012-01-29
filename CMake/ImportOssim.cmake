
MESSAGE(STATUS "Importing Ossim...")
# Option for internal/external GDAL
OPTION(OTB_USE_EXTERNAL_OSSIM "Use an outside build of Ossim." OFF)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_OSSIM)

IF(OTB_USE_EXTERNAL_OSSIM)

  IF (NOT OSSIM_FOUND)
    FIND_PATH(OSSIM_INCLUDE_DIR NAMES ossim/init/ossimInit.h PATHS $ENV{OSSIM_INCLUDE_DIR} /usr/local)
    MARK_AS_ADVANCED(OSSIM_INCLUDE_DIR)
    FIND_LIBRARY(OSSIM_LIBRARY NAMES ossim PATHS /usr/local/lib/ossim)
    MARK_AS_ADVANCED(OSSIM_LIBRARY)
  ENDIF(NOT OSSIM_FOUND)

  IF (NOT OSSIM_INCLUDE_DIR)
  MESSAGE(FATAL_ERROR
           "Cannot find OSSIM include directory. Please set OSSIM_INCLUDE_DIR.")
  ENDIF (NOT OSSIM_INCLUDE_DIR)

  INCLUDE_DIRECTORIES(${OSSIM_INCLUDE_DIR} ${OSSIM_INCLUDE_DIR}/ossim)

  IF (NOT OSSIM_LIBRARY)
    MESSAGE(FATAL_ERROR
             "Cannot find OSSIM library. Please set OSSIM_LIBRARY.")
  ENDIF (NOT OSSIM_LIBRARY)
  MESSAGE(STATUS "  OSSIM includes : ${OSSIM_INCLUDE_DIR}")
  MESSAGE(STATUS "  OSSIM library  : ${OSSIM_LIBRARY}")
ELSE(OTB_USE_EXTERNAL_OSSIM)
  SET(OSSIM_LIBRARY otbossim)
ENDIF(OTB_USE_EXTERNAL_OSSIM)

