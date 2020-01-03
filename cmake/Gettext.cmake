find_package(Gettext REQUIRED)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext REQUIRED)
find_program(GETTEXT_MSGINIT_EXECUTABLE msginit REQUIRED)

function(add_internationals target lang lang_root output_dir)

    get_target_property(sources_list ${target} SOURCES)

    set(pot_file "${CMAKE_CURRENT_SOURCE_DIR}/${lang_root}/${target}.pot")

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lang_root}")
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${lang_root}")
    endif()

    if(NOT EXISTS "${pot_file}")
        execute_process(
            COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -C -o "${pot_file}" -D "${CMAKE_CURRENT_SOURCE_DIR}" ${sources_list}
            ERROR_VARIABLE creation_error
            RESULT_VARIABLE creation_res
          )
      if(NOT creation_res EQUAL "0")
           message(FATAL_ERROR "Failed generate POT file ${pot_file}: ${creation_error}")
      endif()
    endif()

    set(source_lang_dir "${CMAKE_CURRENT_SOURCE_DIR}/${lang_root}/${lang}")
    set(dist_lang_dir "${output_dir}/${lang}/LC_MESSAGES")
    set(po_file "${source_lang_dir}/${target}.po")
    set(mo_file "${dist_lang_dir}/${target}.mo")

    if(NOT EXISTS "${source_lang_dir}")
        file(MAKE_DIRECTORY "${source_lang_dir}")
    endif()

    if(NOT EXISTS "${dist_lang_dir}")
        file(MAKE_DIRECTORY "${dist_lang_dir}")
    endif()

    if(NOT EXISTS "${po_file}")
        execute_process(
            COMMAND ${GETTEXT_MSGINIT_EXECUTABLE} --input=${pot_file} --locale=${lang} --output=${po_file} --no-translator
            ERROR_VARIABLE creation_error
            RESULT_VARIABLE creation_res
          )
      if(NOT creation_res EQUAL "0")
           message(FATAL_ERROR "Failed generate PO file ${po_file}: ${creation_error}")
      endif()
    endif()

    add_custom_target(
               "${target}_${lang}_po_files"
                COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -C -j -o "${pot_file}" -D "${CMAKE_CURRENT_SOURCE_DIR}" ${sources_list}

               )

    add_custom_target(
           "${target}-${lang}-locale"
           COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} -Uis "${po_file}" "${pot_file}"
           DEPENDS "${target}_${lang}_po_files"
           WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}
           SOURCES "${po_file}"
       )

    add_custom_target( "${target}_${lang}_mo_file"
              COMMAND ${CMAKE_COMMAND} -E make_directory ${dist_lang_dir}
              COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -D ${CMAKE_CURRENT_SOURCE_DIR} -o "${mo_file}" "${po_file}"
              DEPENDS "${target}-${lang}-locale"
           )


     add_dependencies(${target}  "${target}_${lang}_mo_file")
endfunction()
