project(oauth-agents-top)

find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED )


add_subdirectory( src )

