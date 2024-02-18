#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <iterator>
#include <chrono>


float const time_multiplier = 1;
float const pi = 3.14159265358979323;
float const half_pi = pi / 2;
float const screenX = 1270;
float const screenY = 800;
int const numParticles = 500;
int const max_calc_dist = 2000; //px


sf::CircleShape shape_list[numParticles];

float acceleration_list_x[numParticles];
float acceleration_list_y[numParticles];
float velocity_list_x[numParticles];
float velocity_list_y[numParticles];
float position_list_x[numParticles];
float position_list_y[numParticles];



int init_particles() {
    for (int i = 0; i < numParticles; i++) {

        float r = sqrt(static_cast<float>(rand()) / RAND_MAX);

        r = r * r * r * r;

        float a = static_cast<float>(std::rand()) / RAND_MAX * 2 * pi;

        float m = 300;

        float x = (r * cos(a) * m);
        float y = (r * -sin(a) * m);


        position_list_x[i] = (1270.0f / 2.0f) + x;
        position_list_y[i] = (800.0f / 2.0f) + y;

        shape_list[i] = sf::CircleShape(1.0f, 4);
        shape_list[i].setRadius(1.0f);
        shape_list[i].setPosition(position_list_x[i], position_list_y[i]);
    

    }
    return 0;
}

int update_particles(float dt) {
    for (int i = 0; i < numParticles; i++) {
        velocity_list_x[i] = velocity_list_x[i] + (acceleration_list_x[i] * dt);
        velocity_list_y[i] = velocity_list_y[i] + (acceleration_list_y[i] * dt);

        position_list_x[i] = position_list_x[i] + (velocity_list_x[i] * dt);
        position_list_y[i] = position_list_y[i] + (velocity_list_y[i] * dt);

        shape_list[i].setPosition(position_list_x[i], position_list_y[i]);

        acceleration_list_x[i] = 0.0f;
        acceleration_list_y[i] = 0.0f;       
    }
    return 0;
}

int draw_particles(sf::RenderWindow& window) {
    for (int i = 0; i < numParticles; i++) {
        window.draw(shape_list[i]);
    }
    return 0;
}

float inv_square(float n) { //thank you Quake III
   
   const float threehalfs = 1.5f;
   float y = n;
   
   long i = * ( long * ) &y;

   i = 0x5f3759df - ( i >> 1 );
   y = * ( float * ) &i;
   
   y = y * ( threehalfs - ( (n * 0.5F) * y * y ) );
   
   return y;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Particle Gravity");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::Font font;
    if (!font.loadFromFile("Consola.ttf")) {
        return -1;
    }

    init_particles();



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
        for (int i = 0; i < numParticles; i++) {
            for (int j = i; j < numParticles; j++) {
                if (i == j) {
                    continue;
                }

                float r_x = position_list_x[j] - position_list_x[i];
                float r_y = position_list_y[j] - position_list_y[i];


                if (abs(r_x) > 0 && abs(r_y) > 0) {
                    float d_0 = (r_x * r_x + r_y * r_y);
                    if (d_0 > (max_calc_dist * max_calc_dist)) {
                        continue;
                    }
                    float d_1 = inv_square(d_0);

                    float accel_x = (d_1 * inv_square(d_0 * d_0)) * r_x;
                    float accel_y = (d_1 * inv_square(d_0 * d_0)) * r_y;


                    acceleration_list_x[i] += accel_x;
                    acceleration_list_y[i] += accel_y;

                    acceleration_list_x[j] += -accel_x;
                    acceleration_list_y[j] += -accel_y;



                }
            }
        }

        // Calculate frame rate
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds() * time_multiplier;


        for (int i = 0; i < numParticles; i++) {
            update_particles(dt);
        }

        
        if (index % 100 == 0) {
            fps = 1.0f / elapsed.asSeconds();
        }
        
        if (fps > max_fps) {
            max_fps = fps;
        }


        sf::Text fps_text;
        fps_text.setString("Current: " + std::to_string(fps) + " FPS " + std::to_string(dt) + "dt"); // Set the text string
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

        draw_particles(window);

        window.draw(fps_text);
        window.draw(max_fps_text);


        window.display();



        index++;












    }

    return 0;
}
