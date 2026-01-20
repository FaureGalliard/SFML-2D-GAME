#include "ObjectAtlas.h"
#include "WorldObject.h"
#include <unordered_map>
#include <cassert>

static const std::unordered_map<WorldObjectType, ObjectVisual> visuals = {

    { WorldObjectType::Flower, {
        1, 1,
        {
                {31, 1},//azul
                {32, 1},
                {33, 1},
                {31, 2},//rojo
                {32, 2},
                {33, 2},
                {31, 3},//blanco
                {32, 3},
                {33, 3},
                {34, 3}

        }
    }},

    { WorldObjectType::GrassTuft, {
        1, 1,
        {
                {27, 1}
        }
    }},

    { WorldObjectType::Rock, {
        1, 1,
        {
                {31, 4},
                {32, 4},
                {33,4},
                {34,4}
        }
    }},

    { WorldObjectType::Bush, {
        1, 1,
        {
                {51, 4}

        }
    }},

    { WorldObjectType::Tree1x3, {
        1, 3,
        {
                {52, 3}
        }
    }},

    { WorldObjectType::Tree2x3, {
        2, 3,
        {
                {51, 6}

        }
    }}
};

const ObjectVisual& getObjectVisual(WorldObjectType type) {
    auto it = visuals.find(type);
    assert(it != visuals.end() && "ObjectType not found in atlas!");
    return it->second;
}