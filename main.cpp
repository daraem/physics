#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "fisicas.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics");
    sf::Mouse mouse;

    window.setFramerateLimit(60);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::vector<Circle> circles;

    for (int i = 0; i < 10; ++i) {
        Circle circle;
        circle.shape.setRadius(25);
        circle.shape.setOrigin(circle.shape.getRadius(), circle.shape.getRadius());
        circle.posicionParticula = { float(std::rand() % 500), float(std::rand() % 500) };

        circle.shape.setPosition(circle.posicionParticula);
        circle.shape.setFillColor(sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256)); // Color aleatorio
        // circle.velocidad.x = float(std::rand() % 200);
        circle.velocidad.y = float(std::rand() % 500);
        circle.velocidad.y = 10.f;
        circles.push_back(circle);
    }

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        for (const Circle& circle : circles) {
            window.draw(circle.shape);
        }
        update(circles);
        window.display();
    }

    return 0;
}
