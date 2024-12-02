#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants for maximum lives and word list size
const int MAX_LIVES = 6;
const string WORDS[] = {
    "hello", "illustrate", "softly", "brave", "challenge",
    "remarkable", "keyboard", "computer", "rainbow", "pineapple",
    "universe", "butterfly", "adventure", "waterfall", "sunshine",
    "vacation", "discovery", "harmony", "serenity", "inspiration"
};
const int WORD_COUNT = sizeof(WORDS) / sizeof(WORDS[0]);

// Hangman Game class
class HangmanGame {
private:
    string playerName;
    string wordToGuess;
    bool guessedLetters[50] = { false }; // Max word length is 50
    int lives;

    // SFML Variables
    sf::RenderWindow window;
    sf::Font font;
    sf::Text hangmanText, guessedWordText, messageText, livesText;
    sf::RectangleShape hangmanDrawing;

    // Random word generator
    string getRandomWord() {
        srand(static_cast<unsigned>(time(0)));
        return WORDS[rand() % WORD_COUNT];
    }

    // Update displayed guessed word
    string getDisplayedWord() {
        string displayedWord = "";
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (guessedLetters[i]) {
                displayedWord += wordToGuess[i];
            }
            else {
                displayedWord += "_";
            }
            displayedWord += " ";
        }
        return displayedWord;
    }

    // Check if all letters are guessed
    bool isWordGuessed() {
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (!guessedLetters[i]) return false;
        }
        return true;
    }

    // Draw the hangman based on remaining lives
    void drawHangman() {
        hangmanDrawing.setSize(sf::Vector2f(200, 300));
        hangmanDrawing.setFillColor(sf::Color::White);
        hangmanDrawing.setPosition(50, 50);

        switch (lives) {
        case 5:
            hangmanText.setString("O"); // Head
            hangmanText.setPosition(150, 100);
            break;
        case 4:
            hangmanText.setString("O\n |"); // Body
            hangmanText.setPosition(150, 100);
            break;
        case 3:
            hangmanText.setString("O\n/|"); // Left arm
            hangmanText.setPosition(150, 100);
            break;
        case 2:
            hangmanText.setString("O\n/|\\"); // Both arms
            hangmanText.setPosition(150, 100);
            break;
        case 1:
            hangmanText.setString("O\n/|\\\n/"); // Left leg
            hangmanText.setPosition(150, 100);
            break;
        case 0:
            hangmanText.setString("O\n/|\\\n/ \\"); // Both legs
            hangmanText.setPosition(150, 100);
            break;
        default:
            hangmanText.setString(""); // No hangman initially
            break;
        }
    }

    // Process player's guess
    void processGuess(char guess) {
        bool correct = false;

        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (tolower(wordToGuess[i]) == tolower(guess) && !guessedLetters[i]) {
                guessedLetters[i] = true;
                correct = true;
            }
        }

        if (correct) {
            messageText.setString("Correct guess!");
        }
        else {
            lives--;
            messageText.setString("Wrong guess! Lives -1");
        }
    }

public:
    HangmanGame() : lives(MAX_LIVES), window(sf::VideoMode(800, 600), "Hangman Game") {
        // Initialize font and text
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            cerr << "Error loading font!" << endl;
            exit(1);
        }

        hangmanText.setFont(font);
        hangmanText.setCharacterSize(40);
        hangmanText.setFillColor(sf::Color::Red);

        guessedWordText.setFont(font);
        guessedWordText.setCharacterSize(30);
        guessedWordText.setFillColor(sf::Color::White);
        guessedWordText.setPosition(300, 400);

        messageText.setFont(font);
        messageText.setCharacterSize(24);
        messageText.setFillColor(sf::Color::Yellow);
        messageText.setPosition(300, 500);

        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(600, 50);

        wordToGuess = getRandomWord();
    }

    void setPlayerName(const string& name) {
        playerName = name;
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::TextEntered) {
                    if (lives > 0 && !isWordGuessed()) {
                        char guess = static_cast<char>(event.text.unicode);
                        if (isalpha(guess)) {
                            processGuess(guess);
                        }
                    }
                }
            }

            // Update visuals
            drawHangman();
            guessedWordText.setString("Word: " + getDisplayedWord());
            livesText.setString("Lives: " + to_string(lives));

            window.clear(sf::Color::Black);
            window.draw(hangmanDrawing);
            window.draw(hangmanText);
            window.draw(guessedWordText);
            window.draw(messageText);
            window.draw(livesText);
            window.display();

            // Check win/lose condition
            if (isWordGuessed()) {
                messageText.setString("Congratulations! You won!");
            }
            else if (lives <= 0) {
                messageText.setString("Game Over! The word was: " + wordToGuess);
            }
        }
    }
};

// GameBoy Class
class GameBoy {
private:
    sf::RenderWindow window;
    int currentSelection;
    const char* menuItems[3] = { "1. Hangman", "2. Snake", "3. Wordle" };

    // GameBoy states
    bool isOn;

    void displayMenu() {
        // Load background texture
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            std::cerr << "Error: Could not load background image!" << std::endl;
            return;
        }
        sf::Sprite backgroundSprite(backgroundTexture);

        // Scale the background to fit the window
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
        );

        // Load font
        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            return;
        }

        // Create menu text
        sf::Text text[3];
        for (int i = 0; i < 3; i++) {
            text[i].setFont(font);
            text[i].setString(menuItems[i]);
            text[i].setCharacterSize(24);
            text[i].setFillColor(i == currentSelection ? sf::Color::Red : sf::Color::White);

            // Position text to fit within the green screen area
            text[i].setPosition(300, 200 + i * 50); // Adjust the x and y offsets
        }

        // Main menu loop
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::S) {
                        isOn = !isOn; // Toggle on/off state
                    }

                    if (!isOn) continue; // Ignore other inputs if the GameBoy is off

                    if (event.key.code == sf::Keyboard::Up) {
                        currentSelection = (currentSelection - 1 + 3) % 3; // Wrap around
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        currentSelection = (currentSelection + 1) % 3; // Wrap around
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        // Start selected game
                        if (currentSelection == 0) {
                            startHangmanGame(); // Start Hangman
                        }
                        // Add other game start logic here for Snake or Wordle
                        return; // Exits menu
                    }
                }
            }

            window.clear();
            window.draw(backgroundSprite); // Draw the background

            if (isOn) {
                for (int i = 0; i < 3; i++) {
                    text[i].setFillColor(i == currentSelection ? sf::Color::Red : sf::Color::White);
                    window.draw(text[i]); // Draw menu items
                }
            }
            else {
                // Dim the screen when the GameBoy is off
                sf::RectangleShape dimScreen(sf::Vector2f(window.getSize()));
                dimScreen.setFillColor(sf::Color(0, 0, 0, 200));
                window.draw(dimScreen);
            }

            window.display();
        }
    }

    void startHangmanGame() {
        HangmanGame hangman;
        hangman.run();
    }

public:
    GameBoy() : window(sf::VideoMode(800, 600), "GameBoy Menu"), currentSelection(0), isOn(true) {}

    void run() {
        while (window.isOpen()) {
            displayMenu();
        }
    }
};
