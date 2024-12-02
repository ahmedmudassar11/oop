#include <SFML/Graphics.hpp>
#include <string>

const int resolutionX = 640;
const int resolutionY = 640;
const int boxSize = 64;
const int gridRows = 6; 
const int gridColumns = 5; 

extern const std::string wordList[50];

void evaluateGuess(const std::string& guess, std::string& feedback, const std::string& secretWord);

class WordleGrid {
private:
    char guesses[gridRows][gridColumns]; 
    std::string feedback[gridRows];     
    int currentRow;                    

public:
    WordleGrid();
    bool makeGuess(const std::string& guess, const std::string& secretWord);
    void draw(sf::RenderWindow& window, sf::Font& font, const std::string& currentGuess);
    bool isGameOver();
    bool hasWon();
};

