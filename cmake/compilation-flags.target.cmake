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
