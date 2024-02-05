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
float const half_pi = pi / 2;
float const screenX = 1270;
float const screenY = 800;
const int numParticles = 500;
const float G = 1; //6.674e-5

std::vector<Particle> particles;

class Particle {
public:
    Particle(float x, float y, float m, bool f, float v_x, float v_y) {
        position.x = x;
        position.y = y;
        velocity.x = v_x;
        velocity.y = v_y;
        acceleration.x = 0;
        acceleration.y = 0;
        mass = m;
        fixed = f;

        shape.setRadius(1.0f);
        shape.setPosition(position);

    }

    void update(float dt) {
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

    sf::Font font;
    if (!font.loadFromFile("Consola.ttf")) {
        // Replace "arial.ttf" with the path to a font file on your system
        return EXIT_FAILURE;
    }

    for (int i = 0; i < numParticles; i++) {

        float r = sqrt(static_cast<float>(rand()) / RAND_MAX);

        r = r * r * r;

        float a = static_cast<float>(std::rand()) / RAND_MAX * 2 * pi;

        float m = 300;

        float x = (r * cos(a) * m);
        float y = (-r * sin(a) * m);

        float slope = y / x;
        float angle = atan(-1 * (1 / slope)) + half_pi + (((-1 * y) / abs(y)) * half_pi) - (0.3 * pi);
        float x_v = cos(angle);
        float y_v = sin(angle);

        particles.emplace_back((1270.0f / 2.0f) + x, (800.0f / 2.0f) + y, 1.0f, false, x_v, y_v);
    }



    sf::Clock clock;
    float max_fps = 0;
    int index = 0;
    float fps = 0;
    sf::Time last_time;

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

        // Calculate frame rate
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds() * time_multiplier;


        for (int i = 0; i < particles.size(); i++) {
            particles[i].update(dt);
        }

        
        if (index % 100 == 0) {
            fps = 1.0f / elapsed.asSeconds();
        }
        
        if (fps > max_fps) {
            max_fps = fps;
        }


        sf::Text fps_text;
        fps_text.setString("Current: " + std::to_string(fps) + "FPS " + std::to_string(dt) + "dt"); // Set the text string
        fps_text.setCharacterSize(16); // Set the character size
        fps_text.setFillColor(sf::Color::White); // Set the fill color
        fps_text.setPosition(10.0f, 10.0f);
        fps_text.setFont(font);

        sf::Text max_fps_text;
        max_fps_text.setString("Max: " + std::to_string(static_cast<int>(max_fps)) + " FPS"); // Set the text string
        max_fps_text.setCharacterSize(16); // Set the character size
        max_fps_text.setFillColor(sf::Color::White); // Set the fill color
        max_fps_text.setPosition(10.0f, 28.0f);
        max_fps_text.setFont(font);




        window.clear();

        for (const auto& particle : particles) {
            particle.draw(window);
        }

        window.draw(fps_text);
        window.draw(max_fps_text);


        window.display();



        index++;












    }

    return 0;
}
