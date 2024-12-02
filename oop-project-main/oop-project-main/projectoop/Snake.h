#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>


const int resolutionX = 840;
const int resolutionY = 840;
const int boxSize = 32;
const int gridRows = resolutionX / boxSize;
const int gridColumns = resolutionY / boxSize;
const int maxSnakeLength = gridRows * gridColumns; 


const int GAME_RUNNING = 0;
const int GAME_OVER = 1;
const int GAME_PAUSED = 2;


enum Direction { UP, DOWN, LEFT, RIGHT };


class Grid {
public:
    int rows, columns, boxSize;

    Grid(int rows, int columns, int boxSize);

    void renderGrid(sf::RenderWindow& window);
};

class Food {
public:
    int x, y;

    Food();

    void generateFood();
    void draw(sf::RenderWindow& window, sf::RectangleShape& foodShape);
};

class Snake {
public:
    int segments[maxSnakeLength][2];  
    int length;
    Direction direction;
    bool growFlag;

    Snake();

    void changeDirection(Direction newDirection);
    void move();
    void grow();
    bool checkCollision();
    void draw(sf::RenderWindow& window, sf::RectangleShape& snakeShape);
};

class Game {
private:
    Snake snake;
    Food food;
    Grid grid;
    int score;
    int state;

public:
    Game();

    void handleInput();
    void update();
    void render(sf::RenderWindow& window);
    void restart();
};

