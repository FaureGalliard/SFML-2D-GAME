#include "CombatSystem.h"
#include "entities/Hero.h"
#include "entities/Enemy.h"
#include "core/Config.h"
#include <cmath>
#include <map>
#include <set>

struct AttackTracker {
    std::set<const Enemy*> hitEnemies;  // Enemigos ya golpeados
    EntityState lastState = EntityState::Idle;
};

static std::map<const Hero*, AttackTracker> attackTrackers;

bool CombatSystem::isAttackState(EntityState state) {
    return state == EntityState::Attack ||
           state == EntityState::Axe ||
           state == EntityState::Hammering;
}

int CombatSystem::getAttackDamage(EntityState attackType) {
    switch (attackType) {
        case EntityState::Attack:
            return 20;
        case EntityState::Axe:
            return 20;
        case EntityState::Hammering:
            return 15;
        default:
            return 0;
    }
}

sf::FloatRect CombatSystem::getAttackHitbox(const Hero& hero, EntityState attackType) {
    sf::Vector2f heroPos = hero.getPosition();
    bool facingLeft = hero.isFacingLeft();

    float attackWidth = 20.0f;
    float attackHeight = 20.0f;
    float attackRange = 5.0f;  // Distancia del ataque desde el centro del hero

    switch (attackType) {
        case EntityState::Attack:
            attackWidth = 25.0f;
            attackHeight = 20.0f;
            attackRange = 0.0f;
            break;

        case EntityState::Axe:
            attackWidth = 24.0f;
            attackHeight = 20.0f;
            attackRange = 0.0f;
            break;

        case EntityState::Hammering:
            attackWidth = 50.0f;
            attackHeight = 20.0f;
            attackRange = 20.0f;
            break;
        default:
            break;
    }

    float hitboxX, hitboxY;

    if (facingLeft) {
        hitboxX = heroPos.x - attackWidth + attackRange;
    } else {
        hitboxX = heroPos.x - attackRange;
    }

    hitboxY = heroPos.y - attackHeight * 2 - 10.0f;

    return sf::FloatRect(hitboxX, hitboxY, attackWidth, attackHeight);
}

AttackInfo CombatSystem::createHeroAttack(const Hero& hero) {
    AttackInfo attack;
    attack.active = false;
    attack.damage = 0;
    attack.duration = 0.0f;
    attack.elapsed = 0.0f;

    EntityState state = hero.getState();

    if (!isAttackState(state)) {
        return attack;
    }

    const Animation* anim = hero.getCurrentAnimation();
    if (!anim) {
        return attack;
    }

    attack.active = true;
    attack.damage = getAttackDamage(state);
    attack.hitbox = getAttackHitbox(hero, state);
    attack.duration = 0.3f;
    attack.elapsed = 0.0f;

    return attack;
}

bool CombatSystem::checkAttackHit(const AttackInfo& attack, const Enemy& enemy) {
    if (!attack.active) {
        return false;
    }

    if (!enemy.isAlive()) {
        return false;
    }

    sf::FloatRect enemyBounds = enemy.getBounds();
    return attack.hitbox.intersects(enemyBounds);
}

void CombatSystem::applyDamage(Enemy& enemy, int damage) {
    if (!enemy.isAlive()) {
        return;
    }

    int currentHealth = enemy.getHealth();
    currentHealth -= damage;

    if (currentHealth <= 0) {
        currentHealth = 0;
        enemy.setHealth(0);
        enemy.triggerAction(EntityState::Death);
    } else {
        enemy.setHealth(currentHealth);
        enemy.triggerAction(EntityState::Hurt);
    }
}

void CombatSystem::update(Hero& hero, std::vector<Enemy*>& enemies, float dt) {
    EntityState heroState = hero.getState();

    AttackTracker& tracker = attackTrackers[&hero];

    if (tracker.lastState != heroState) {
        tracker.hitEnemies.clear();
        tracker.lastState = heroState;
    }

    if (!isAttackState(heroState)) {
        return;
    }

    AttackInfo attack = createHeroAttack(hero);
    if (!attack.active) {
        return;
    }

    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        if (!enemy->isAlive()) continue;

        if (tracker.hitEnemies.find(enemy) != tracker.hitEnemies.end()) {
            continue;
        }

        if (checkAttackHit(attack, *enemy)) {
            applyDamage(*enemy, attack.damage);
            tracker.hitEnemies.insert(enemy);
        }
    }
}