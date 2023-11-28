#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
#include <windows.h>

struct Circle {
    sf::CircleShape shape;
    sf::Vector2f velocidad;
    sf::Vector2f posicionParticula;
};

bool collision = false;

void checkCollision(Circle& circles) {
    float window_width = 800;
    float window_height = 600;
    float coef = 1.f;

    sf::Vector2f suelo = { static_cast<float>(window_width), static_cast<float>(window_height)};

    sf::Vector2f distancia = suelo - circles.posicionParticula;
    float ModDist = sqrt(distancia.x * distancia.x + distancia.y * distancia.y);

    if (distancia.y < circles.shape.getRadius()) {
        circles.posicionParticula.y = suelo.y - circles.shape.getRadius();
        circles.velocidad *= coef;
    }

    if (distancia.x < circles.shape.getRadius()) {
        circles.posicionParticula.x = suelo.x - circles.shape.getRadius();
    }

    if (distancia.x > window_width - circles.shape.getRadius()) {
        circles.posicionParticula.x = circles.shape.getRadius();
    }

    if (distancia.y > window_height - circles.shape.getRadius()) {
        circles.posicionParticula.y = circles.shape.getRadius();
    }
}

void bolasSeparadas(std::vector<Circle>& circles) {
    const float responseCoef = 4.f;

    for (uint64_t i = 0; i < circles.size(); ++i) {
        Circle object1 = circles[i];

        for (uint64_t k{ i + 1 }; k < circles.size(); ++k) {
            Circle object2 = circles[k];

            sf::Vector2f distancia = (object1.posicionParticula - object2.posicionParticula);
            float SemiModuloDist = distancia.x * distancia.x + distancia.y * distancia.y;
            float minDist = object1.shape.getRadius() + object2.shape.getRadius();

            if (SemiModuloDist < minDist * minDist) {
                float ModuloDist = sqrt(SemiModuloDist);
                sf::Vector2f uniDist = distancia / ModuloDist;

                float mass1 = object1.shape.getRadius() / (object1.shape.getRadius() + object2.shape.getRadius());
                float mass2 = object2.shape.getRadius() / (object1.shape.getRadius() + object2.shape.getRadius());

                const float delta = 0.5f * responseCoef * (ModuloDist - minDist);

                object1.posicionParticula -= uniDist * (mass2 * delta);
                object2.posicionParticula += uniDist * (mass1 * delta);
                collision = true;
            }
            else {
                collision = false;
            }
            circles[i] = object1;
            circles[k] = object2;
        }

    }
}

void updatePosition(float dt, std::vector<Circle>& circles) {
        for (uint64_t i = 0; i < circles.size(); ++i) {
            sf::Vector2f gravedad = { 0.0f, 1000.0f };

            if (collision) {
                gravedad = { 0.0f, 0.0f };
            }
            else {
                gravedad = { 0.0f, 1000.0f };
            }

            circles[i].velocidad = circles[i].velocidad + gravedad * dt;
            circles[i].posicionParticula = circles[i].posicionParticula + circles[i].velocidad * dt;

            checkCollision(circles[i]);
            circles[i].shape.setPosition(circles[i].posicionParticula);
        }
}

void update(std::vector<Circle>& circles) {
    float dt = 1.0f / 60.0f;
    float sub_steps = 16;
    float sub_dt = dt / sub_steps;

    for (uint32_t i(sub_steps); i--;) {
        updatePosition(sub_dt, circles);
        bolasSeparadas(circles);
    }
}