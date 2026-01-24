#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <map>

class Enemy;
class World;

enum class AIState {
    Idle,       // Enemigo quieto
    Wandering,   // Enemigo moviéndose
    Hunting
};

struct AIComponent {
    AIState currentState = AIState::Idle;
    sf::Vector2f moveDirection = {0.0f, 0.0f};
    float stateTimer = 0.0f;
    float nextStateChange = 2.0f;
};

class AISystem {
public:
    AISystem() = default;

    static void update(std::vector<Enemy*>& enemies, const World& world, float dt);

    static void updateEnemy(Enemy& enemy, AIComponent& ai, const World& world, float dt);

    static sf::Vector2f generateRandomDirection();

    static void changeState(AIComponent& ai, AIState newState);

    static float getRandomStateTime(AIState state);
};