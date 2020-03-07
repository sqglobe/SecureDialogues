project(main)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(BerkeleyDB REQUIRED)
find_package(Boost  REQUIRED system)
find_package(persistent-storage REQUIRED)

set(MAIN_SOURCE
    main.cpp
    passask.cpp
)



set(CMAKE_AUTORCC ON)

add_executable(SecureDialogues ${MAIN_SOURCE} project_icons.qrc secure-dialogues.rc)
target_link_libraries(SecureDialogues gui messages oauth-agents
     encryption  email-agent curl-wrapper utils
     Qt5::Core Qt5::Gui  Qt5::Widgets xg ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARY} pthread persistent-storage  cryptopp-static plugins-support)

if(MSVC)
  target_compile_options(SecureDialogues PRIVATE /W4 /WX)
else()
  target_compile_options(SecureDialogues PRIVATE -Wall -Wextra -pedantic)
endif()

if (WIN32)
    target_link_libraries(SecureDialogues intl )
endif (WIN32)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET SecureDialogues PROPERTY WIN32_EXECUTABLE true)
endif()
target_compile_definitions(SecureDialogues PRIVATE NO_MEMBER_FUNCTION_PARTIAL_SPECIALIZATION)
add_internationals(SecureDialogues ru_RU.UTF-8 "locale" "${LOCALE_FILES_DIR}" securedialogues 1.1)
