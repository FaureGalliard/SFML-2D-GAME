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
        src/world/generation/Noise.h
        src/objects/WorldObject.h
        src/render/Tileset.cpp
        src/render/Tileset.h
        src/render/WorldRenderer.cpp
        src/render/WorldRenderer.h
        src/world/generation/Noise.cpp
        src/systems/InputSystem.cpp
        src/systems/InputSystem.h
        src/render/EntityRenderer.cpp
        src/render/EntityRenderer.h
        src/entities/Hero.cpp
        src/entities/Hero.h

        src/core/Config.h
        src/objects/SpawnRules.cpp
        src/objects/SpawnRules.h
        src/systems/MovementSystem.cpp
        src/systems/MovementSystem.h
        src/systems/CombatSystem.cpp
        src/systems/CombatSystem.h
        src/systems/AISystem.cpp
        src/systems/AISystem.h
        src/render/Renderer.cpp
        src/render/Renderer.h
        src/world/generation/TerrainGenerator.cpp
        src/world/generation/TerrainGenerator.h
        src/world/generation/ObjectSpawner.cpp
        src/world/generation/ObjectSpawner.h
        src/objects/ObjectAtlas.cpp
        src/objects/ObjectAtlas.h
        src/objects/ObjectVisual.h
        src/render/Camera.cpp
        src/render/Camera.h
        src/render/Animation.cpp
        src/render/Animation.h
        src/entities/Entity.cpp
        src/entities/Entity.h
        src/world/generation/BiomeNoise.cpp
        src/world/generation/BiomeNoise.h
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