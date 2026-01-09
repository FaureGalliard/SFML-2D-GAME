cmake_minimum_required(VERSION 4.0)
project(ProjectSFML)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp src/*h")

include_directories(
        include
        "C:/Program Files/SFML/include"

)
link_directories(
        "C:/Program Files/SFML/lib"

)
add_executable(ProjectSFML ${SOURCES} ${HEADERS}
        src/core/Game.cpp
        src/core/Game.h
        src/world/World.cpp
        src/world/World.h
        src/world/Chunk.cpp
        src/world/Chunk.h
        src/world/Tile.h
        src/world/Noise.h
        src/world/worldObject.cpp
        src/world/worldObject.h
        src/render/Tileset.cpp
        src/render/Tileset.h
        src/render/WorldRenderer.cpp
        src/render/WorldRenderer.h
)

target_link_libraries(ProjectSFML
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
)

target_include_directories(ProjectSFML PRIVATE
        "include"
        "C:/Program Files/nlohmann"
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include

)

set(SFML_DLLS
        "C:/Program Files/SFML/bin/sfml-graphics-2.dll"
        "C:/Program Files/SFML/bin/sfml-window-2.dll"
        "C:/Program Files/SFML/bin/sfml-system-2.dll"
        "C:/Program Files/SFML/bin/sfml-audio-2.dll"
        "C:/Program Files/SFML/bin/sfml-network-2.dll"
)

foreach(dll ${SFML_DLLS})
    add_custom_command(TARGET ProjectSFML POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${dll}
            $<TARGET_FILE_DIR:ProjectSFML>)

endforeach()

add_custom_command(TARGET ProjectSFML POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ProjectSFML>/assets
)