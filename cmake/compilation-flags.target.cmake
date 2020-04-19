option(ON_SANITIZE "Enable sanitize (Y/N)" OFF)

add_library(CompilationFlags INTERFACE)

target_compile_options(CompilationFlags INTERFACE
  $<$<CXX_COMPILER_ID:GNU>: -pipe -Wall -Wextra -pedantic>
)

target_compile_options(CompilationFlags INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>
)

target_compile_options(CompilationFlags INTERFACE
  $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Release>>: -O3 >
)

target_compile_definitions(CompilationFlags INTERFACE
$<$<CONFIG:Release>:QT_NO_DEPRECATED_WARNINGS>
)

if(ON_SANITIZE)
    target_compile_options(CompilationFlags INTERFACE
      $<$<CXX_COMPILER_ID:GNU>: -fsanitize=leak -fsanitize=address -fsanitize=undefined >
    )
    target_link_libraries(CompilationFlags INTERFACE
       $<$<CXX_COMPILER_ID:GNU>: asan ubsan >
    )
endif()
