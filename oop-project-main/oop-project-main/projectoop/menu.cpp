#include <SFML/Graphics.hpp>
#include <iostream>
#include "Snake.h"
#include "Hangmanbest.h"
#include "Wordle.h"

class GameBoy {
private:
    sf::RenderWindow window;
    int currentSelection;
    const char* menuItems[3] = { "1. Hangman", "2. Snake", "3. Wordle" };

    bool isOn;

    void displayMenu() {
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            std::cerr << "Error: Could not load background image!" << std::endl;
            return;
        }
        sf::Sprite backgroundSprite(backgroundTexture);

        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
        );

        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            return;
        }

        sf::Text text[3];
        for (int i = 0; i < 3; i++) {
            text[i].setFont(font);
            text[i].setString(menuItems[i]);
            text[i].setCharacterSize(24);
            text[i].setFillColor(i == currentSelection ? sf::Color::Red : sf::Color::White);

            text[i].setPosition(300, 200 + i * 50); 
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::S) {
                        isOn = !isOn; 
                    }

                    if (!isOn) continue; 

                    if (event.key.code == sf::Keyboard::Up) {
                        currentSelection = (currentSelection - 1 + 3) % 3; 
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        currentSelection = (currentSelection + 1) % 3; 
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        return;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (currentSelection == 0) { 
                            HangmanGame hangman;
                            hangman.run(); 
                        }
                    }
                }
            }

            window.clear();
            window.draw(backgroundSprite); 

            if (isOn) {
                for (int i = 0; i < 3; i++) {
                    text[i].setFillColor(i == currentSelection ? sf::Color::Red : sf::Color::White);
                    window.draw(text[i]); 
                }
            }
            else {
                sf::RectangleShape dimScreen(sf::Vector2f(window.getSize()));
                dimScreen.setFillColor(sf::Color(0, 0, 0, 200));
                window.draw(dimScreen);
            }

            window.display();
        }
    }

public:
    GameBoy() : window(sf::VideoMode(800, 600), "GameBoy Menu"), currentSelection(0), isOn(true) {}

    void run() {
        while (window.isOpen()) {
            displayMenu();
        }
    }
};


