project(curl-wrapper)

find_package( CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

add_definitions(-DCURL_STATICLIB)

set(WIN_REQUEST_LIB "")

if(WIN32)
    message("Used for windows build")
    set(WIN_REQUEST_LIB  wldap32 ws2_32 -static)
endif()
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

set(HEADERS
  uribuilder.h
  httprequest.h
  curlexceptions.h
)

set(SOURCES
  uribuilder.cpp
  httprequest.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC  curl  ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${WIN_REQUEST_LIB})
