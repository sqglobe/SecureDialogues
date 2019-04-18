project(documents)

FIND_PROGRAM(ASCIIDOC_EXECUTABLE
  asciidoctor-pdf
)

FIND_PROGRAM(PLANTUML_EXECUTABLE
  plantuml
)


if(NOT PLANTUML_EXECUTABLE)
    message(WARNING "Plantuml is not found. Documentation build skipped")
elseif(NOT ASCIIDOC_EXECUTABLE)
    message(WARNING "asciidoctor-pdf is not found. Documentation build skipped")
else()
    add_subdirectory( user-guide )
    add_subdirectory( architecture )
endif()


