#include "ObjectAtlas.h"
#include "WorldObject.h"
#include <unordered_map>
#include <cassert>

static const std::unordered_map<WorldObjectType, ObjectVisual> visuals = {
    { WorldObjectType::Flower, ObjectVisual(
        1, 1,
        {
            {31, 1}, {32, 1}, {33, 1},  // azul
            {31, 2}, {32, 2}, {33, 2},  // rojo
            {31, 3}, {32, 3}, {33, 3}, {34, 3}  // blanco
        },
        false
    )},

    { WorldObjectType::GrassTuft, ObjectVisual(
        1, 1,
        {{27, 1}},
        false
    )},

    { WorldObjectType::Rock, ObjectVisual(
        1, 1,
        {{31, 4}, {32, 4}, {33, 4}, {34, 4}},
        true,  // con colisión
        sf::FloatRect(-1.0f, -15.0f, 5.0f, 5.0f)  // hitbox ajustada
    )},

    { WorldObjectType::Bush, ObjectVisual(
        1, 1,
        {{51, 4}},
        true,
        sf::FloatRect(-3.0f, -14.0f, 6.0f, 6.0f)
    )},

    { WorldObjectType::Tree1x3, ObjectVisual(
        1, 3,
        {{52, 3}},
        true,
        sf::FloatRect(-6.0f, -8.0f, 12.0f, 15.0f)  // solo el tronco
    )},

    { WorldObjectType::Tree2x3, ObjectVisual(
        2, 3,
        {{51, 6}},
        true,
        sf::FloatRect(-3.0f, -26.0f, 6.0f, 8.0f)  // tronco más ancho
    )}
};

const ObjectVisual& getObjectVisual(WorldObjectType type) {
    auto it = visuals.find(type);
    assert(it != visuals.end() && "ObjectType not found in atlas!");
    return it->second;
}