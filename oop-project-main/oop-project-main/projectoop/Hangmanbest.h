#include <SFML/Graphics.hpp>
#include <string>

class HangmanGame {
private:
    std::string wordToGuess;
    bool guessedLetters[50] = { false };
    int lives;
    std::string currentGuess;
    std::string displayedWord;

    void drawHangman(sf::RenderWindow& window, sf::Font& font);
    void updateDisplayedWord();
    bool isWordGuessed();
    void processGuess(char guess);

public:
    HangmanGame();
    void run();
};

