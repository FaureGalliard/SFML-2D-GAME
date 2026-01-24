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

    // Verificar si debe cambiar a modo Hunting
    sf::Vector2f heroPos = world.getHeroPosition();
    sf::Vector2f enemyPos = enemy.getPosition();
    sf::Vector2f diff = heroPos - enemyPos;
    float distanceToHero = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    const float DETECTION_RANGE = 150.0f;

    // Cambiar a Hunting si el héroe está cerca
    if (distanceToHero <= DETECTION_RANGE && ai.currentState != AIState::Hunting) {
        changeState(ai, AIState::Hunting);
    }
    // Volver a Wandering si el héroe se aleja demasiado
    else if (distanceToHero > DETECTION_RANGE * 1.5f && ai.currentState == AIState::Hunting) {
        changeState(ai, AIState::Wandering);
        ai.moveDirection = generateRandomDirection();
    }

    if (ai.stateTimer <= 0.0f && ai.currentState != AIState::Hunting) {
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
        sf::Vector2f nextPos = currentPos + velocity * dt * 5.0f;

        if (!CollisionSystem::canMoveTo(enemy, world, nextPos)) {
            ai.moveDirection = generateRandomDirection();
            nextPos = currentPos + ai.moveDirection * speed * dt * 5.0f;

            if (!CollisionSystem::canMoveTo(enemy, world, nextPos)) {
                changeState(ai, AIState::Idle);
                ai.stateTimer = getRandomStateTime(AIState::Idle) * 0.5f;
                ai.moveDirection = {0.0f, 0.0f};
            }
        }
    }

    if (ai.currentState == AIState::Hunting) {
        const float ATTACK_RANGE = 30.0f;

        if (distanceToHero <= ATTACK_RANGE) {
            // Héroe en rango de ataque
            enemy.attack();
            ai.moveDirection = {0.0f, 0.0f};
        } else {
            // Seguir al héroe
            if (distanceToHero > 0.0f) {
                ai.moveDirection.x = diff.x / distanceToHero;
                ai.moveDirection.y = diff.y / distanceToHero;
            }

            // Verificar colisiones al perseguir
            float speed = 120.0f;
            sf::Vector2f nextPos = enemyPos + ai.moveDirection * speed * dt * 5.0f;

            if (!CollisionSystem::canMoveTo(enemy, world, nextPos)) {
                // Intentar rodear el obstáculo
                sf::Vector2f altDirection1 = {-ai.moveDirection.y, ai.moveDirection.x};
                sf::Vector2f altDirection2 = {ai.moveDirection.y, -ai.moveDirection.x};

                sf::Vector2f altPos1 = enemyPos + altDirection1 * speed * dt * 5.0f;
                sf::Vector2f altPos2 = enemyPos + altDirection2 * speed * dt * 5.0f;

                if (CollisionSystem::canMoveTo(enemy, world, altPos1)) {
                    ai.moveDirection = altDirection1;
                } else if (CollisionSystem::canMoveTo(enemy, world, altPos2)) {
                    ai.moveDirection = altDirection2;
                } else {
                    ai.moveDirection = {0.0f, 0.0f};
                }
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

        case AIState::Hunting:
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

        case AIState::Hunting:
            // En Hunting el timer no se usa para cambiar de estado
            return 999.0f;

        default:
            return 2.0f;
    }

    return dist(gen);
}