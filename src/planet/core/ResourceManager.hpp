#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    void loadTexture(std::string _path, std::string _name);
    void loadFont(std::string _path, std::string _name);

    const sf::Texture& getTexture(std::string _name);
    const sf::Font& getFont(std::string _name);

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font> m_fonts;

    sf::Texture m_nullTexture;
    sf::Font m_nullFont;
};