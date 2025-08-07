cmake_minimum_required(VERSION 3.10)
project(SaveValley)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ruta a tus archivos fuente
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Incluir headers
include_directories(include)
# .hpp o h
include_directories("lib/SFML/include")
include_directories("lib")
# .a o .lib
link_directories("lib/SFML/lib")


# Ejecutable
add_executable(SaveValley ${SOURCES} ${HEADERS})

# Link SFML
target_link_libraries(SaveValley
 sfml-graphics
  sfml-window 
  sfml-system
   sfml-audio
   sfml-network)

set(SFML_DLLS
   ${CMAKE_SOURCE_DIR}/lib/SFML/bin/sfml-graphics-2.dll
   ${CMAKE_SOURCE_DIR}/lib/SFML/bin/sfml-window-2.dll
   ${CMAKE_SOURCE_DIR}/lib/SFML/bin/sfml-system-2.dll
   ${CMAKE_SOURCE_DIR}/lib/SFML/bin/sfml-audio-2.dll
   ${CMAKE_SOURCE_DIR}/lib/SFML/bin/sfml-network-2.dll
)
foreach(dll ${SFML_DLLS})
    add_custom_command(TARGET SaveValley POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${dll}
        $<TARGET_FILE_DIR:SaveValley>)
endforeach()

add_custom_command(TARGET SaveValley POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SaveValley>/assets)

add_custom_command(TARGET SaveValley POST_BUILD
    COMMAND SaveValley
    WORKING_DIRECTORY $<TARGET_FILE_DIR:SaveValley>
    COMMENT "Ejecutando SaveValley después de compilar...")