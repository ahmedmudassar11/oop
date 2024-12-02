#include "HangmanGame.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Hangmanbest.h"

using namespace std;


const string wordList[50] = {
    "apple", "table", "grape", "brush", "cream", "house", "heart", "bread", "dream", "light",
    "stone", "plant", "chair", "river", "cloud", "piano", "money", "grass", "scale", "flame",
    "paper", "pearl", "glass", "space", "clock", "mouse", "crowd", "drama", "water", "flute",
    "white", "smile", "shape", "blood", "round", "dance", "block", "plaza", "store", "beach",
    "shade", "horse", "shine", "ocean", "fruit", "plume", "dance", "sword", "vocal", "wings"
};

HangmanGame::HangmanGame() : lives(6) {
    srand(static_cast<unsigned>(time(0)));
    wordToGuess = wordList[rand() % 50];
    updateDisplayedWord();
}

void HangmanGame::drawHangman(sf::RenderWindow& window, sf::Font& font) {
    sf::Text hangmanText;
    hangmanText.setFont(font);
    hangmanText.setCharacterSize(40);
    hangmanText.setFillColor(sf::Color::Red);

    switch (lives) {
    case 5: hangmanText.setString("O"); break; 
    case 4: hangmanText.setString("O\n |"); break; 
    case 3: hangmanText.setString("O\n/|"); break; 
    case 2: hangmanText.setString("O\n/|\\"); break;
    case 1: hangmanText.setString("O\n/|\\\n/"); break; 
    case 0: hangmanText.setString("O\n/|\\\n/ \\"); break; 
    }

    hangmanText.setPosition(10, 10);
    window.draw(hangmanText);
}

void HangmanGame::updateDisplayedWord() {
    displayedWord = "";
    for (size_t i = 0; i < wordToGuess.length(); i++) {
        if (guessedLetters[i]) {
            displayedWord += wordToGuess[i];
        }
        else {
            displayedWord += "_";
        }
        displayedWord += " ";
    }
}

bool HangmanGame::isWordGuessed() {
    for (size_t i = 0; i < wordToGuess.length(); i++) {
        if (!guessedLetters[i]) return false;
    }
    return true;
}

void HangmanGame::processGuess(char guess) {
    bool correct = false;
    for (size_t i = 0; i < wordToGuess.length(); i++) {
        if (tolower(wordToGuess[i]) == tolower(guess) && !guessedLetters[i]) {
            guessedLetters[i] = true;
            correct = true;
        }
    }
    if (!correct) lives--;
}

void HangmanGame::run() {
    sf::RenderWindow window(sf::VideoMode(640, 640), "Hangman Game");
    sf::Font font;
    if (!font.loadFromFile("Movistar Text Regular.ttf")) {
        cerr << "Error: Could not load font!" << endl;
        return;
    }

    sf::Text wordText, messageText, livesText;
    wordText.setFont(font);
    wordText.setCharacterSize(30);
    wordText.setFillColor(sf::Color::White);
    wordText.setPosition(100, 500);

    messageText.setFont(font);
    messageText.setCharacterSize(24);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(100, 550);

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(500, 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered && lives > 0 && !isWordGuessed()) {
                char guess = static_cast<char>(event.text.unicode);
                if (isalpha(guess)) {
                    processGuess(guess);
                    updateDisplayedWord();
                }
            }
        }

        wordText.setString("Word: " + displayedWord);
        livesText.setString("Lives: " + to_string(lives));

        if (isWordGuessed()) {
            messageText.setString("Congratulations! You guessed the word!");
        }
        else if (lives <= 0) {
            messageText.setString("Game Over! The word was: " + wordToGuess);
        }

        window.clear(sf::Color::Black);
        drawHangman(window, font);
        window.draw(wordText);
        window.draw(messageText);
        window.draw(livesText);
        window.display();
    }
}
