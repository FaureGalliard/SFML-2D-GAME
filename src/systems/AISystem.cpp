#include "AISystem.h"
#include "entities/Enemy.h"
#include "world/World.h"
#include <random>
#include <cmath>

// Generador de números aleatorios
static std::random_device rd;
static std::mt19937 gen(rd());

void AISystem::update(std::vector<Enemy*>& enemies, const World& world, float dt) {
    // Mapa estático para mantener el componente de IA de cada enemigo
    static std::map<Enemy*, AIComponent> aiComponents;
    
    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        if (!enemy->isAlive()) continue;
        
        // Si el enemigo está herido o muriendo, no procesar IA
        if (enemy->getState() == EntityState::Hurt || 
            enemy->getState() == EntityState::Death) {
            continue;
        }
        
        // Obtener o crear el componente de IA para este enemigo
        AIComponent& ai = aiComponents[enemy];
        
        updateEnemy(*enemy, ai, world, dt);
    }
    
    // Limpiar componentes de IA de enemigos que ya no existen
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
    // Actualizar el timer del estado
    ai.stateTimer -= dt;
    
    // Si el timer se acabó, cambiar de estado
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
    
    // Aplicar el movimiento según el estado
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
    
    // Normalizar el vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    } else {
        // Si por casualidad es (0,0), dar una dirección por defecto
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
            // Estar quieto entre 1 y 3 segundos
            dist = std::uniform_real_distribution<float>(1.0f, 3.0f);
            break;
            
        case AIState::Wandering:
            // Moverse entre 2 y 4 segundos
            dist = std::uniform_real_distribution<float>(2.0f, 4.0f);
            break;
            
        default:
            return 2.0f;
    }
    
    return dist(gen);
}