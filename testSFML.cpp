#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <iostream>

// Global variables to store input state
bool moveLeft = false;
bool moveRight = false;

// Function to take input in a separate thread
void* inputThread(void* arg) {
    while (true) {
        char input;
        std::cin >> input;
        if (input == 'a') {
            moveLeft = true;
        } else if (input == 'd') {
            moveRight = true;
        }
    }
    return NULL;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML with Input Thread");

    // Create a shape to move
    sf::RectangleShape shape(sf::Vector2f(50, 50));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(375, 275);

    // Create a thread for input
    pthread_t inputThreadID;
    pthread_create(&inputThreadID, NULL, inputThread, NULL);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move the shape based on input
        if (moveLeft) {
            shape.move(-5, 0);
            moveLeft = false; // Reset input flag
        }
        if (moveRight) {
            shape.move(5, 0);
            moveRight = false; // Reset input flag
        }

        // Main thread logic here

        window.clear(sf::Color::White);
        window.draw(shape);
        window.display();
    }

    // Wait for the input thread to finish
    pthread_join(inputThreadID, NULL);

    return 0;
}
