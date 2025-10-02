#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Decoration {
    sf::Sprite sprite;

    Decoration(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f position) {
        sprite.setTexture(texture);
        sprite.setTextureRect(rect);
        sprite.setPosition(position);
    }
};

class Map : public sf::Drawable, public sf::Transformable {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture tilesetTexture;
    std::vector<Decoration> decorations;

    std::unordered_map<std::string, sf::IntRect> catalog;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(backgroundSprite, states);
        for (const auto& deco : decorations) {
            target.draw(deco.sprite, states);
        }
    }

public:
    Map(const std::string& jsonFile = "assets/map.json") {
        loadFromJson(jsonFile);
    }

    bool loadFromJson(const std::string& jsonFile) {
        std::ifstream file(jsonFile);

        json j;
        file >> j;

        backgroundTexture.loadFromFile(j.value("background", ""));
        backgroundSprite.setTexture(backgroundTexture);

        tilesetTexture.loadFromFile(j.value("tileset", ""));

        for (auto& [name, rectData] : j["catalog"].items()) {
            catalog[name] = sf::IntRect(
                rectData.value("tileX", 0) * 16,
                rectData.value("tileY", 0) * 16,
                rectData.value("tilesW", 1) * 16,
                rectData.value("tilesH", 1) * 16
            );
        }

        for (auto& decoData : j["decorations"]) {
            std::string type = decoData.value("type", "");
            auto it = catalog.find(type);
            if (it != catalog.end()) {
                sf::Vector2f pos(
                    decoData.value("x", 0.f),
                    decoData.value("y", 0.f)
                );
                decorations.emplace_back(tilesetTexture, it->second, pos);
            }
        }

        return true;
    }
};
