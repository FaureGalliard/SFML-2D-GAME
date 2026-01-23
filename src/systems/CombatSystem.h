#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <vector>

#include "entities/Entity.h"

class Entity;
class Hero;
class Enemy;
class World;

struct AttackInfo {
    sf::FloatRect hitbox;  // Hitbox del ataque en coordenadas del mundo
    int damage;
    bool active;
    float duration;        // Duración del ataque en segundos
    float elapsed;         // Tiempo transcurrido
};

class CombatSystem {
public:
    CombatSystem() = default;

    static void update(Hero& hero, std::vector<Enemy*>& enemies, float dt);

    static AttackInfo createHeroAttack(const Hero& hero);

    static bool checkAttackHit(const AttackInfo& attack, const Enemy& enemy);

    static void applyDamage(Enemy& enemy, int damage);

    static int getAttackDamage(EntityState attackType);

    static sf::FloatRect getAttackHitbox(const Hero& hero, EntityState attackType);

private:
    // Verificar si un estado es un ataque
    static bool isAttackState(EntityState state);
};