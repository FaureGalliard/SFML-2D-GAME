#pragma once
#include "ObjectVisual.h"

enum class WorldObjectType : uint8_t;

const ObjectVisual& getObjectVisual(WorldObjectType type);