project(curl-wrapper)

find_package( CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package (Threads REQUIRED)

add_definitions(-DCURL_STATICLIB)

set(WIN_REQUEST_LIB "")

if(WIN32)
    add_definitions(-DCURL_STATICLIB)
    message("Used for windows build")
    set(WIN_REQUEST_LIB  wldap32 ws2_32 -static)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
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

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC  ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${WIN_REQUEST_LIB})
