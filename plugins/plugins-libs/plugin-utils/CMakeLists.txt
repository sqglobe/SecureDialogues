add_library(plugins-utils STATIC
    recievedmessagesiteratorimpl.h
    recievedmessagesiteratorimpl.cpp
    serialization-helpers.h
    )
target_link_libraries(plugins-utils  PUBLIC plugin-api )
target_link_libraries(plugins-utils PRIVATE utils-headers)
target_include_directories(plugins-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(plugins-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}../")
set_property(TARGET plugins-utils PROPERTY POSITION_INDEPENDENT_CODE ON)
