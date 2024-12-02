#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

const int MAX_LIVES = 6;
const string wordList[50] = {
    "apple", "table", "grape", "brush", "cream", "house", "heart", "bread", "dream", "light",
    "stone", "plant", "chair", "river", "cloud", "piano", "money", "grass", "scale", "flame",
    "paper", "pearl", "glass", "space", "clock", "mouse", "crowd", "drama", "water", "flute",
    "white", "smile", "shape", "blood", "round", "dance", "block", "plaza", "store", "beach",
    "shade", "horse", "shine", "ocean", "fruit", "plume", "dance", "sword", "vocal", "wings"
};

class Game {
public:
    virtual void run() = 0; 
    virtual ~Game() {}     
};

class HangmanGame : public Game {
private:
    string wordToGuess;
    bool guessedLetters[50] = { false };
    int lives;
    string displayedWord;

    void updateDisplayedWord() {
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

    bool isWordGuessed() {
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (!guessedLetters[i]) return false;
        }
        return true;
    }

    void processGuess(char guess) {
        bool correct = false;
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (tolower(wordToGuess[i]) == tolower(guess) && !guessedLetters[i]) {
                guessedLetters[i] = true;
                correct = true;
            }
        }
        if (!correct) lives--;
    }

    void drawHangman(sf::RenderWindow& window, sf::Font& font) {
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

public:
    HangmanGame() : lives(MAX_LIVES) {
        srand(static_cast<unsigned>(time(0)));
        wordToGuess = wordList[rand() % 50];
        updateDisplayedWord();
    }

    void run() override {
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
};

class GameBoy {
private:
    sf::RenderWindow window;
    int currentSelection;
    Game* games[1];
    bool isOn;

    void displayMenu() {
        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            cerr << "Error: Could not load font!" << endl;
            return;
        }

        sf::Text menuText[1];
        menuText[0].setFont(font);
        menuText[0].setString("1. Hangman");
        menuText[0].setCharacterSize(24);
        menuText[0].setFillColor(sf::Color::Red);
        menuText[0].setPosition(300, 200);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        games[0]->run(); 
                    }
                }
            }

            window.clear(sf::Color::Black);
            window.draw(menuText[0]);
            window.display();
        }
    }

public:
    GameBoy() : window(sf::VideoMode(800, 600), "GameBoy Menu"), currentSelection(0), isOn(true) {
        games[0] = new HangmanGame();
    }

    ~GameBoy() {
        delete games[0];
    }

    void run() {
        displayMenu();
    }
};


int main() {
    GameBoy gameboy;
    gameboy.run();
    return 0;
}