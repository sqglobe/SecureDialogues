project(main)

find_package(Qt5Widgets REQUIRED)

set(MAIN_SOURCE
    main.cpp
    passask.cpp
)



set(CMAKE_AUTORCC ON)

add_executable(SecureDialogues ${MAIN_SOURCE} project_icons.qrc secure-dialogues.rc)
target_link_libraries(SecureDialogues gui messages )

if(MSVC)
  target_compile_options(SecureDialogues PRIVATE /W4 /WX)
else()
  target_compile_options(SecureDialogues PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET SecureDialogues PROPERTY WIN32_EXECUTABLE true)
endif()
