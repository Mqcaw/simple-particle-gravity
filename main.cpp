#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <iterator>
#include <chrono>


class Particle;

float const time_multiplier = 1;
float const pi = 3.14159265358979323;
float const screenX = 1270;
float const screenY = 800;
const int numParticles = 1000;
const float G = 1; //6.674e-5
float const dt = time_multiplier * 0.0625f;//0.0625f 

std::vector<Particle> particles;

class Particle {
public:
    Particle(float x, float y, float m, bool f) {
        position.x = x;
        position.y = y;
        velocity.x = 0;
        velocity.y = 0;
        acceleration.x = 0;
        acceleration.y = 0;
        mass = m;
        fixed = f;

        shape.setRadius(1.0f);
        shape.setPosition(position);

    }

    void update() {
        if (fixed == false) {
            velocity = velocity + (acceleration * dt);
            position = position + (velocity * dt);

            shape.setPosition(position);
        }
        

        acceleration = sf::Vector2f(0, 0);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }

public:
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    bool fixed;
    float mass;
    
    
};






int main() {
    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Particle Gravity");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < numParticles; i++) {

        float r = sqrt(static_cast<float>(rand()) / RAND_MAX);

        r = r * r;

        float a = static_cast<float>(std::rand()) / RAND_MAX * 2 * pi;

        float m = 300;

        float x = r * cos(a) * m;
        float y = -r * sin(a) * m;

        particles.emplace_back(1270 / 2 + x, 800 / 2 + y, 1, false);
    }

    //particles.emplace_back(1270 / 2, 800 / 2, 1000, true);



    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }



        // Calculate and update acceleration for each particle
        for (int i = 0; i < particles.size(); i++) {
            for (int j = i; j < particles.size(); j++) {
                if (i == j) {
                    continue;
                }
                sf::Vector2f r = particles[j].position - particles[i].position;


                if (abs(r.x) > 0 && abs(r.y) > 0) {
                    float mag_sq = r.x * r.x + r.y * r.y;
                    float mag = sqrt(mag_sq);

                    sf::Vector2f accel = (particles[j].mass / (mag_sq * mag)) * r;

                    particles[i].acceleration += accel;
                    particles[j].acceleration += -accel;
                }
            }
        }

        //update postion of each particle
        for (int i = 0; i < particles.size(); i++) {
            particles[i].update();
        }

        sf::Text text;
        text.setString("test"); // Set the text string
        text.setCharacterSize(16); // Set the character size
        text.setFillColor(sf::Color::White); // Set the fill color
        text.setPosition(10.0f, 10.0f);
        sf::Font font;
        if (!font.loadFromFile("Consola.ttf")) {
            // Replace "arial.ttf" with the path to a font file on your system
            return EXIT_FAILURE;
        }
        text.setFont(font);


        window.clear();

        for (const auto& particle : particles) {
            particle.draw(window);
        }

        window.draw(text);


        window.display();
















    }

    return 0;
}
