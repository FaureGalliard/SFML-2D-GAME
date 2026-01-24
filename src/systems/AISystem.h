#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

class Enemy;
class World;

enum class AIState {
    Idle,       // Enemigo quieto
    Wandering   // Enemigo moviéndose
};

struct AIComponent {
    AIState currentState = AIState::Idle;
    sf::Vector2f moveDirection = {0.0f, 0.0f};
    float stateTimer = 0.0f;          // Tiempo restante en el estado actual
    float nextStateChange = 2.0f;     // Cuándo cambiar de estado
};

class AISystem {
public:
    AISystem() = default;
    
    // Actualizar la IA de todos los enemigos
    static void update(std::vector<Enemy*>& enemies, const World& world, float dt);
    
    // Actualizar la IA de un enemigo individual
    static void updateEnemy(Enemy& enemy, AIComponent& ai, const World& world, float dt);
    
    // Generar una nueva dirección aleatoria para moverse
    static sf::Vector2f generateRandomDirection();
    
    // Cambiar el estado de la IA
    static void changeState(AIComponent& ai, AIState newState);
    
    // Obtener tiempo aleatorio para un estado
    static float getRandomStateTime(AIState state);
};