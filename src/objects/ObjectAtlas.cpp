#include "ObjectAtlas.h"
#include <cassert>
#include <unordered_map>
static const std::unordered_map<WorldObjectType, ObjectVisual> visuals = {

    { WorldObjectType::Flower,    {31, 1,  1, 1} },
    { WorldObjectType::GrassTuft, {27, 1,  1, 1} },
    { WorldObjectType::Rock,      {31, 4,  1, 1} },
    { WorldObjectType::Bush,      {51, 4,  1, 1} },

    { WorldObjectType::Tree1x3,   {52, 3,  1, 3} },
    { WorldObjectType::Tree2x3,   {51, 6,  2, 3} }
};

const ObjectVisual& getObjectVisual(WorldObjectType type) {
    auto it = visuals.find(type);
    assert(it != visuals.end());
    return it->second;
}