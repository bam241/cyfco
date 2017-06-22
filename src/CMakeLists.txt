### DO NOT DELETE THIS COMMENT: INSERT_ARCHETYPES_HERE ###


USE_CYCLUS("cyfco" "reactor")
USE_CYCLUS("cyfco" "mixer")
USE_CYCLUS("cyfco" "enrichment")
USE_CYCLUS("cyfco" "group_inst")

INSTALL_CYCLUS_MODULE("cyfco" "" "NONE")

SET(TestSource ${cyfco_TEST_CC} PARENT_SCOPE)

# install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
