
SET(qif_SOURCES
  qif-context.c
  qif-defaults.c
  qif-file.c
  qif-objects.c
  qif-parse.c
)

# Add dependency on config.h
SET_SOURCE_FILES_PROPERTIES (${qif_SOURCES} PROPERTIES OBJECT_DEPENDS ${CONFIG_H})

SET(qif_noinst_HEADERS
  qif-file.h
  qif-defaults.h
  qif-import-p.h
  qif-import.h
  qif-objects.h
  qif-objects-p.h
  qif-parse.h
)

ADD_LIBRARY(gncmod-qif ${qif_noinst_HEADERS} ${qif_SOURCES})

TARGET_LINK_LIBRARIES(gncmod-qif gncmod-generic-import gncmod-engine gnc-qof ${GLIB2_LDFLAGS})

TARGET_COMPILE_DEFINITIONS(gncmod-qif PRIVATE -DG_LOG_DOMAIN=\"gnc.import.qif\")

INSTALL(TARGETS gncmod-qif
  LIBRARY DESTINATION lib/gnucash
  ARCHIVE DESTINATION lib/gnucash
  RUNTIME DESTINATION bin)
# No headers to install.
