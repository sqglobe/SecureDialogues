project(curl-wrapper)

find_package( CURL REQUIRED)

set(WIN_REQUEST_LIB "")

if(WIN32)
    message("Used for windows build")
    set(WIN_REQUEST_LIB  wldap32 ws2_32)
endif()

set(HEADERS
  uribuilder.h
  httprequest.h
  curlexceptions.h
  httpcode.h
)

set(SOURCES
  uribuilder.cpp
  httprequest.cpp
  httpcode.cpp
)


add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
link_directories( ${CURL_LIBRARIES} )
target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC  curl ${WIN_REQUEST_LIB})
