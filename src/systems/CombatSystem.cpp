#include "CombatSystem.h"
#include "entities/Hero.h"
#include "entities/Enemy.h"
#include "core/Config.h"
#include <cmath>

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
            return 40;
        default:
            return 0;
    }
}

sf::FloatRect CombatSystem::getAttackHitbox(const Hero& hero, EntityState attackType) {
    sf::Vector2f heroPos = hero.getPosition();
    bool facingLeft = hero.isFacingLeft();

    // Hitbox base del ataque (relativo a la posición del hero)
    float attackWidth = 20.0f;
    float attackHeight = 20.0f;
    float attackRange = 5.0f;  // Distancia del ataque desde el centro del hero

    // Ajustar según el tipo de ataque
    switch (attackType) {
        case EntityState::Attack:
            attackWidth = 20.0f;
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

    // Calcular la posición del hitbox según la dirección
    float hitboxX, hitboxY;

    if (facingLeft) {
        hitboxX = heroPos.x  - attackWidth+ attackRange;
    } else {
        hitboxX = heroPos.x- attackRange;
    }

    // Centrar verticalmente con un pequeño offset hacia arriba
    hitboxY = heroPos.y- attackHeight*2 - 10.0f;

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

    // El ataque solo está activo durante ciertos frames de la animación
    const Animation* anim = hero.getCurrentAnimation();
    if (!anim) {
        return attack;
    }

    // Determinar si el ataque está activo según el progreso de la animación
    // Los ataques son activos en la mitad de la animación aproximadamente
    attack.active = true;
    attack.damage = getAttackDamage(state);
    attack.hitbox = getAttackHitbox(hero, state);
    attack.duration = 0.3f;  // Ventana de tiempo para golpear
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
    // Verificar si el hero está atacando
    EntityState heroState = hero.getState();

    if (!isAttackState(heroState)) {
        return;
    }

    // Obtener la animación actual
    const Animation* anim = hero.getCurrentAnimation();
    if (!anim) {
        return;
    }

    // Para evitar múltiples golpes por animación
    static std::map<const Hero*, bool> attackProcessed;

    // Resetear cuando la animación empieza
    if (anim->getCurrentFrame().left == 0) {
        attackProcessed[&hero] = false;
    }

    if (attackProcessed[&hero]) {
        return;
    }

    // Crear el ataque
    AttackInfo attack = createHeroAttack(hero);
    if (!attack.active) {
        return;
    }

    bool hitSomething = false;

    // Verificar colisión con enemigos
    for (Enemy* enemy : enemies) {
        if (!enemy) continue;          // seguridad
        if (!enemy->isAlive()) continue;

        if (checkAttackHit(attack, *enemy)) {
            applyDamage(*enemy, attack.damage);
            hitSomething = true;
        }
    }

    // Marcar ataque como procesado
    if (hitSomething) {
        attackProcessed[&hero] = true;
    }
}
