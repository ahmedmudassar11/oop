#include "Wordle.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

const std::string wordList[50] = {
    "apple", "table", "grape", "brush", "cream", "house", "heart", "bread", "dream", "light",
    "stone", "plant", "chair", "river", "cloud", "piano", "money", "grass", "scale", "flame",
    "paper", "pearl", "glass", "space", "clock", "mouse", "crowd", "drama", "water", "flute",
    "white", "smile", "shape", "blood", "round", "dance", "block", "plaza", "store", "beach",
    "shade", "horse", "shine", "ocean", "fruit", "plume", "dance", "sword", "vocal", "wings"
};

void evaluateGuess(const std::string& guess, std::string& feedback, const std::string& secretWord) {
    feedback = "     ";
    for (int i = 0; i < gridColumns; i++) {
        if (guess[i] == secretWord[i]) {
            feedback[i] = 'G'; 
        }
        else if (secretWord.find(guess[i]) != std::string::npos) {
            feedback[i] = 'Y'; 
        }
        else {
            feedback[i] = 'R'; 
        }
    }
}

WordleGrid::WordleGrid() : currentRow(0) {
    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridColumns; j++) {
            guesses[i][j] = ' '; 
        }
        feedback[i] = "     "; 
    }
}

bool WordleGrid::makeGuess(const std::string& guess, const std::string& secretWord) {
    if (currentRow >= gridRows) return false; 
    for (int i = 0; i < gridColumns; i++) {
        guesses[currentRow][i] = guess[i];
    }
    evaluateGuess(guess, feedback[currentRow], secretWord);
    currentRow++;
    return true;
}

void WordleGrid::draw(sf::RenderWindow& window, sf::Font& font, const std::string& currentGuess) {
    sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
    sf::Text letterText;
    letterText.setFont(font);
    letterText.setCharacterSize(32);
    letterText.setStyle(sf::Text::Bold);

    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridColumns; j++) {
            if (feedback[i][j] == 'G') box.setFillColor(sf::Color::Green);
            else if (feedback[i][j] == 'Y') box.setFillColor(sf::Color::Yellow);
            else if (feedback[i][j] == 'R') box.setFillColor(sf::Color::Red);
            else box.setFillColor(sf::Color::White);

            box.setOutlineColor(sf::Color::Black);
            box.setOutlineThickness(2);
            box.setPosition(j * boxSize, i * boxSize);
            window.draw(box);

            if (i < currentRow || (i == currentRow && j < currentGuess.length())) {
                letterText.setString(std::string(1, (i == currentRow ? currentGuess[j] : guesses[i][j])));
                letterText.setFillColor(sf::Color::Black);
                letterText.setPosition(j * boxSize + boxSize / 4, i * boxSize + boxSize / 4);
                window.draw(letterText);
            }
        }
    }
}

bool WordleGrid::isGameOver() {
    return currentRow >= gridRows || feedback[currentRow - 1] == "GGGGG";
}

bool WordleGrid::hasWon() {
    return feedback[currentRow - 1] == "GGGGG";
}
