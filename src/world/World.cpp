#include "World.h"
#include "generation/TerrainGenerator.h"
#include "generation/ObjectSpawner.h"
#include <unordered_set>
#include <cmath>
#include <random>

static int floorDiv(int a, int b) {
    int r = a / b;
    if ((a ^ b) < 0 && a % b) --r;
    return r;
}

World::World(uint32_t seed)
    : terrainGen(std::make_unique<TerrainGenerator>(seed)),
      objSpawner(std::make_unique<ObjectSpawner>(seed + 1)) {
}

World::~World() = default;

void World::update(int playerTileX, int playerTileY) {
    int centerCx = floorDiv(playerTileX, CHUNK_SIZE);
    int centerCy = floorDiv(playerTileY, CHUNK_SIZE);

    std::unordered_set<ChunkCoord, ChunkCoordHash> needed;

    for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
        for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
            needed.insert({centerCx + dx, centerCy + dy});
        }
    }

    for (const auto& coord : needed) {
        if (chunks.find(coord) == chunks.end()) {
            loadChunk(coord.x, coord.y);
        }
    }

    unloadFarChunks(centerCx, centerCy);

    for (Enemy& enemy : enemies) {
        enemy.update(1.0f / 60.0f); // TODO: usar delta time real
    }
}

void World::loadChunk(int cx, int cy) {
    auto chunk = std::make_unique<Chunk>(cx, cy, *this);

    terrainGen->generateTerrain(*chunk, cx, cy);
    objSpawner->spawnObjects(*chunk, cx, cy);
    chunk->finalizeAutoTiling();

    chunks[{cx, cy}] = std::move(chunk);

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            auto it = chunks.find({cx + dx, cy + dy});
            if (it != chunks.end()) {
                it->second->finalizeAutoTiling();
                it->second->invalidateObjectMesh();
            }
        }
    }

    spawnEnemiesInChunk(cx, cy);
}

void World::spawnEnemiesInChunk(int cx, int cy) {

    std::mt19937 rng(WORLD_SEED^ (cx * 73856093 ^ cy * 19349663));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    if (dist(rng) < 0.3f) {
        std::uniform_int_distribution<int> countDist(1, 3);
        int enemyCount = countDist(rng);

        for (int i = 0; i < enemyCount; ++i) {
            std::uniform_int_distribution<int> tileDist(2, CHUNK_SIZE - 3);
            int tileX = tileDist(rng);
            int tileY = tileDist(rng);

            float worldX = (cx * CHUNK_SIZE + tileX) * TILE_SIZE;
            float worldY = (cy * CHUNK_SIZE + tileY) * TILE_SIZE;

            enemies.emplace_back(worldX, worldY);
        }
    }
}

void World::unloadFarChunks(int centerCx, int centerCy) {
    for (auto it = chunks.begin(); it != chunks.end();) {
        int dx = std::abs(it->first.x - centerCx);
        int dy = std::abs(it->first.y - centerCy);

        if (dx > LOAD_RADIUS || dy > LOAD_RADIUS) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [centerCx, centerCy](const Enemy& e) {
                int enemyCx = floorDiv(e.getTileX(), CHUNK_SIZE);
                int enemyCy = floorDiv(e.getTileY(), CHUNK_SIZE);
                int dx = std::abs(enemyCx - centerCx);
                int dy = std::abs(enemyCy - centerCy);
                return dx > LOAD_RADIUS || dy > LOAD_RADIUS;
            }
        ),
        enemies.end()
    );
}

const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>&
World::getChunks() const {
    return chunks;
}

std::vector<Chunk*> World::getVisibleChunks(const sf::FloatRect& cameraBounds) const {
    std::vector<Chunk*> visible;

    int minCx = floorDiv(static_cast<int>(cameraBounds.left), CHUNK_SIZE * TILE_SIZE);
    int minCy = floorDiv(static_cast<int>(cameraBounds.top), CHUNK_SIZE * TILE_SIZE);
    int maxCx = floorDiv(static_cast<int>(cameraBounds.left + cameraBounds.width), CHUNK_SIZE * TILE_SIZE);
    int maxCy = floorDiv(static_cast<int>(cameraBounds.top + cameraBounds.height), CHUNK_SIZE * TILE_SIZE);

    for (int cy = minCy; cy <= maxCy; ++cy) {
        for (int cx = minCx; cx <= maxCx; ++cx) {
            auto it = chunks.find({cx, cy});
            if (it != chunks.end()) {
                visible.push_back(it->second.get());
            }
        }
    }

    return visible;
}

std::vector<const Enemy*> World::getVisibleEnemies(const sf::FloatRect& cameraBounds) const {
    std::vector<const Enemy*> result;

    for (const Enemy& enemy : enemies) {
        if (cameraBounds.intersects(enemy.getBounds())) {
            result.push_back(&enemy);
        }
    }

    return result;
}

const Tile* World::getTileGlobal(int wx, int wy) const {
    int cx = floorDiv(wx, CHUNK_SIZE);
    int cy = floorDiv(wy, CHUNK_SIZE);

    int tx = wx - cx * CHUNK_SIZE;
    int ty = wy - cy * CHUNK_SIZE;

    auto it = chunks.find({cx, cy});
    if (it == chunks.end()) {
        return nullptr;
    }

    return &it->second->tiles[ty][tx];
}

Chunk* World::getChunk(int cx, int cy) {
    auto it = chunks.find({cx, cy});
    return it != chunks.end() ? it->second.get() : nullptr;
}

const Chunk* World::getChunk(int cx, int cy) const {
    auto it = chunks.find({cx, cy});
    return it != chunks.end() ? it->second.get() : nullptr;
}