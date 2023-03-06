#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Planet");
    sf::CircleShape shape(100.f);
    shape.setPosition(sf::Vector2f(412.f, 284.f));
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}