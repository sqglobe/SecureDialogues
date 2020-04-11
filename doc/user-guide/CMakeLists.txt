project(user-guide)


set(
    UMLS_DIR
    umls
)

set (
  DOCS
  user-guid.adoc
)

set (
    DIAGRAMS

    )

add_custom_target(
  create-diagrams
  ALL
  COMMAND ${PLANTUML_EXECUTABLE}  ${DIAGRAMS} -o ${DOCUMENTS_OUTPUT_DIRECTORY}/${UMLS_DIR}
  SOURCES ${DIAGRAMS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
  create-user-guid
  ALL
  COMMAND ${ASCIIDOC_EXECUTABLE} -a generateddir=${DOCUMENTS_OUTPUT_DIRECTORY}/${UMLS_DIR} -a staticdir=${CMAKE_CURRENT_SOURCE_DIR}/images   -D ${CMAKE_CURRENT_BINARY_DIR} ${DOCS}
  SOURCES ${DOCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS  create-diagrams
)


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/user-guid.pdf DESTINATION documentation)
