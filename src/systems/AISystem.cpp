#include "AISystem.h"
#include "entities/Enemy.h"
#include "world/World.h"
#include "CollisionSystem.h"
#include "core/Config.h"
#include <random>
#include <cmath>

static std::random_device rd;
static std::mt19937 gen(rd());

void AISystem::update(std::vector<Enemy*>& enemies, const World& world, float dt) {
    static std::map<Enemy*, AIComponent> aiComponents;

    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        if (!enemy->isAlive()) continue;

        if (enemy->getState() == EntityState::Hurt ||
            enemy->getState() == EntityState::Death) {
            continue;
        }

        AIComponent& ai = aiComponents[enemy];

        updateEnemy(*enemy, ai, world, dt);
    }

    for (auto it = aiComponents.begin(); it != aiComponents.end();) {
        bool found = false;
        for (Enemy* enemy : enemies) {
            if (enemy == it->first) {
                found = true;
                break;
            }
        }

        if (!found) {
            it = aiComponents.erase(it);
        } else {
            ++it;
        }
    }
}

void AISystem::updateEnemy(Enemy& enemy, AIComponent& ai, const World& world, float dt) {
    ai.stateTimer -= dt;

    if (ai.stateTimer <= 0.0f) {
        // Alternar entre Idle y Wandering
        if (ai.currentState == AIState::Idle) {
            changeState(ai, AIState::Wandering);
            ai.moveDirection = generateRandomDirection();
        } else {
            changeState(ai, AIState::Idle);
            ai.moveDirection = {0.0f, 0.0f};
        }
    }

    if (ai.currentState == AIState::Wandering) {
        sf::Vector2f currentPos = enemy.getPosition();
        float speed = 100.0f;
        sf::Vector2f velocity = ai.moveDirection * speed;
        sf::Vector2f nextPos = currentPos + velocity * dt * 5.0f; // Mirar un poco adelante

        if (!CollisionSystem::canMoveTo(enemy, world, nextPos)) {
            ai.moveDirection = generateRandomDirection();

            nextPos = currentPos + ai.moveDirection * speed * dt * 5.0f;

            if (!CollisionSystem::canMoveTo(enemy, world, nextPos)) {
                changeState(ai, AIState::Idle);
                ai.stateTimer = getRandomStateTime(AIState::Idle) * 0.5f; // Tiempo más corto
                ai.moveDirection = {0.0f, 0.0f};
            }
        }
    }

    switch (ai.currentState) {
        case AIState::Idle:
            enemy.moveInDirection({0.0f, 0.0f}, false);
            break;

        case AIState::Wandering:
            enemy.moveInDirection(ai.moveDirection, false);
            break;
    }
}

sf::Vector2f AISystem::generateRandomDirection() {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    sf::Vector2f direction;
    direction.x = dist(gen);
    direction.y = dist(gen);

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    } else {
        direction = {1.0f, 0.0f};
    }

    return direction;
}

void AISystem::changeState(AIComponent& ai, AIState newState) {
    ai.currentState = newState;
    ai.stateTimer = getRandomStateTime(newState);
}

float AISystem::getRandomStateTime(AIState state) {
    std::uniform_real_distribution<float> dist;

    switch (state) {
        case AIState::Idle:
            dist = std::uniform_real_distribution<float>(1.0f, 3.0f);
            break;

        case AIState::Wandering:
            dist = std::uniform_real_distribution<float>(2.0f, 4.0f);
            break;

        default:
            return 2.0f;
    }

    return dist(gen);
}