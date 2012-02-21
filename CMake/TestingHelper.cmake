SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)
SET(EXAMPLEDATA ${OTB_DATA_ROOT}/Examples)

IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(NOTOL 0.0)
SET(EPSILON_1  0.1)
SET(EPSILON_2  0.01)
SET(EPSILON_3  0.001)
SET(EPSILON_4  0.0001)
SET(EPSILON_5  0.00001)
SET(EPSILON_6  0.000001)
SET(EPSILON_7  0.0000001)
SET(EPSILON_8  0.00000001)
SET(EPSILON_9  0.000000001)
SET(EPSILON_10 0.0000000001)
SET(EPSILON_11 0.00000000001)
SET(EPSILON_12 0.000000000001)
