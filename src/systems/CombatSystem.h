#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <vector>

#include "entities/Entity.h"

class Entity;
class Hero;
class Enemy;
class World;

struct AttackInfo {
    sf::FloatRect hitbox;
    int damage;
    bool active;
    float duration;
    float elapsed;
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
    static bool isAttackState(EntityState state);
};