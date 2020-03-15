add_library(plugins-utils STATIC
    recievedmessagesiteratorimpl.h
    recievedmessagesiteratorimpl.cpp
    base64.cpp
    base64.h
    )
target_link_libraries(plugins-utils  PUBLIC plugin-api)
target_include_directories(plugins-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(plugins-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}../")
target_compile_options(plugins-utils   PRIVATE -fPIC)
