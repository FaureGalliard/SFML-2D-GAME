// map.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <map>              
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Decoration : public sf::Sprite {
public:
    Decoration() {}

    Decoration(sf::Texture& tileset, sf::IntRect rect) {
        setTexture(tileset);
        setTextureRect(rect);
    }
};

class Map {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture tilesetTexture;
    std::vector<Decoration> decorations;

    sf::IntRect tileRect(int tileX, int tileY, int tilesW, int tilesH) {
        return sf::IntRect(tileX * 16, tileY * 16, tilesW * 16, tilesH * 16);
    }

public:
    Map() {
        const std::string jsonFile = "assets/map.json";
        loadFromJson(jsonFile);
    }

    void loadFromJson(const std::string& jsonFile) {
        std::ifstream file(jsonFile);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir " + jsonFile);
        }

        json j;
        file >> j;

        backgroundTexture.loadFromFile(j["background"].get<std::string>());
        backgroundSprite.setTexture(backgroundTexture);

        tilesetTexture.loadFromFile(j["tileset"].get<std::string>());

        std::map<std::string, sf::IntRect> catalog;
        for (auto& [name, rectData] : j["catalog"].items()) {
            catalog[name] = sf::IntRect(
                rectData["tileX"].get<int>() * 16,
                rectData["tileY"].get<int>() * 16,
                rectData["tilesW"].get<int>() * 16,
                rectData["tilesH"].get<int>() * 16
            );
        }

        for (auto& decoData : j["decorations"]) {
            std::string type = decoData["type"].get<std::string>();
            if (catalog.find(type) != catalog.end()) {
                Decoration deco(tilesetTexture, catalog[type]);
                deco.setPosition(
                    decoData["x"].get<float>(),
                    decoData["y"].get<float>()
                );
                decorations.push_back(deco);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        for (auto& deco : decorations) {
            window.draw(deco); 
        }
    }
};
