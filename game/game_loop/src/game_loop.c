#include "game_loop.h"

#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_bullet_hit_event_queue.h"
#include "entity_info.h"
#include "game_math.h"
#include "player_rules.h"
#include "wave.h"
#include "game_api.h"
#include "player_asteroid_hit_event.h"

void GAME_LOOP_ProcessAsteroidBulletCollision(const AsteroidArray *asteroidArray, const BulletArray *bullets, const AsteroidBulletHitEventSink *sink);
void GAME_LOOP_ProcessPlayerAsteroidCollision(const AsteroidArray *asteroidArray, Player *player, const PlayerAsteroidHitEventSink *sink);

void GAME_LOOP_ProcessAsteroidBulletHitEvent(const GameContext *gameContext) {
    const AsteroidBulletHitEventSink bulletHitAsteroidEventSink = ASTEROID_BULLET_HIT_EVENT_QUEUE_GetSink(gameContext->bulletHitEventQueue);
    GAME_LOOP_ProcessAsteroidBulletCollision(gameContext->asteroidArray, gameContext->bulletArray, &bulletHitAsteroidEventSink);
    WAVE_ExplodeAsteroidsFromBullet(gameContext->asteroidArray, gameContext->wave, gameContext->bulletHitEventQueue);
    PLAYER_UpdateBulletHits(gameContext->bulletHitEventQueue, gameContext->player, gameContext->bulletArray);
    ASTEROID_BULLET_HIT_EVENT_QUEUE_Purge(gameContext->bulletHitEventQueue);
}

void GAME_LOOP_ProcessAsteroidBulletCollision(const AsteroidArray *asteroidArray, const BulletArray *bullets, const AsteroidBulletHitEventSink *sink) {
    for (int i = 0; i < bullets->count; ++i) {
        const Bullet *bullet = BULLETS_GetBullet(bullets, i);
        for (int j = 0; j < asteroidArray->nbAsteroid; ++j) {

            if (asteroidArray->asteroid[j].info.state == STATE_DEAD) continue;
            const Asteroid *asteroid = &asteroidArray->asteroid[j];
            if (ASTEROID_IsBulletColliding(asteroid, bullet, asteroidArray->verticePool)) {
                const Vector2 bulletSpeed = BULLET_GetSpeed(bullet);
                const float hitAngle = AngleFromComponent(bulletSpeed);
                const AsteroidBulletHitEvent event = {
                    j,
                    i,
                    hitAngle,
                    asteroid->score,
                    BULLET_GetPosition(bullet)
                };
                sink->emit(sink->ctx, event);
            }
        }
    }
}

void GAME_LOOP_ProcessPlayerAsteroidHitEvent(const GameContext *gameContext) {
    const PlayerAsteroidHitEventSink sink = PLAYER_ASTEROID_HIT_EVENT_QUEUE_GetSink(gameContext->playerHitEventQueue);
    GAME_LOOP_ProcessPlayerAsteroidCollision(gameContext->asteroidArray, gameContext->player, &sink);

    if (PLAYER_ASTEROID_HIT_EVENT_QUEUE_GetCount(gameContext->playerHitEventQueue) > 0) {
        PLAYER_Die(gameContext->player);
    }
    WAVE_ExplodeAsteroidsFromPlayer(gameContext->asteroidArray, gameContext->wave, gameContext->playerHitEventQueue);

    PLAYER_ASTEROID_HIT_EVENT_QUEUE_Purge(gameContext->playerHitEventQueue);
}

void GAME_LOOP_ProcessPlayerAsteroidCollision(const AsteroidArray *asteroidArray, Player *player, const PlayerAsteroidHitEventSink *sink) {
    for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
        const Asteroid *asteroid = &asteroidArray->asteroid[i];
        Vector2 playerSpeed = PLAYER_GetSpeed(player);
        playerSpeed.y = -playerSpeed.y;
        const float hitAngle = AngleFromComponent(playerSpeed);
        if (ASTEROID_IsPlayerColliding(asteroid, player, asteroidArray->verticePool)) {
            const PlayerAsteroidHitEvent event = {
                .player = player,
                .asteroidId = i,
                .hitPosition = PLAYER_GetPosition(player),
                .hitAngle = hitAngle
            };
            sink->emit(sink->ctx, event);
        }
    }
}