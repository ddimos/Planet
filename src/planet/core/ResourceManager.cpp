#include "core/ResourceManager.hpp"
#include "core/Log.hpp"

void ResourceManager::loadTexture(std::string _path, std::string _name, TextureConfig _config /*= {}*/)
{
    sf::Texture texture;
    if (!texture.loadFromFile(_path))
    {
        LOG_ERROR("Couldn't load a texture " + _path);
        return;
    }
    texture.setSmooth(_config.isSmooth); // Not sure if it's supposed to be here
    texture.setRepeated(_config.isRepeated);
    m_textures.insert_or_assign(_name, texture);
}

void ResourceManager::loadFont(std::string _path, std::string _name)
{
    sf::Font font;
    if (!font.loadFromFile(_path))
    {
        LOG_ERROR("Couldn't load a font " + _path);
        return;
    }
    m_fonts.insert_or_assign(_name, font);
}

const sf::Texture& ResourceManager::getTexture(std::string _name)
{
    auto it = m_textures.find(_name);
    if (it != m_textures.end())
        return it->second;

    LOG_ERROR("Couldn't find a texture " + _name);
    return m_nullTexture;
}

const sf::Font& ResourceManager::getFont(std::string _name)
{
    auto it = m_fonts.find(_name);
    if (it != m_fonts.end())
        return it->second;

    LOG_ERROR("Couldn't find a font " + _name);
    return m_nullFont;
}