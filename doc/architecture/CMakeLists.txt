project(architecture)


set(
    UMLS_DIR
    umls
)

set (
  DOCS
  architecture.adoc
)

set (
    ARCH_DIAGRAMS
    umls/general-presentation.plantuml
    umls/messages-module-presentation.plantuml
    umls/message-handling.plantuml
    umls/dialog-composition.plantuml
    umls/dialog-contact-selected.plantuml
    umls/dialog-messages.planuml
    umls/dialog-key-not-valid.plantuml
    )

add_custom_target(
  create-arch-diagrams
  ALL
  COMMAND ${PLANTUML_EXECUTABLE}  ${ARCH_DIAGRAMS} -o '${DOCUMENTS_OUTPUT_DIRECTORY}/${UMLS_DIR}'
  SOURCES ${ARCH_DIAGRAMS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
  create-architecture
  ALL
  COMMAND ${ASCIIDOC_EXECUTABLE} -a generateddir='${DOCUMENTS_OUTPUT_DIRECTORY}/${UMLS_DIR}' -a staticdir='${CMAKE_CURRENT_SOURCE_DIR}/images' -D '${CMAKE_CURRENT_BINARY_DIR}' ${DOCS}
  SOURCES ${DOCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS  create-arch-diagrams
)

install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/architecture.pdf DESTINATION documentation)


