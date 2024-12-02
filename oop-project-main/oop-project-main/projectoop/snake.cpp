#include "Snake.h"

Grid::Grid(int rows, int columns, int boxSize) : rows(rows), columns(columns), boxSize(boxSize) {}

void Grid::renderGrid(sf::RenderWindow& window) {
    sf::RectangleShape line(sf::Vector2f(resolutionX, 1));
    line.setFillColor(sf::Color::White);

    for (int i = 0; i <= rows; i++) {
        line.setPosition(0, i * boxSize);
        window.draw(line);
    }

    line.setSize(sf::Vector2f(1, resolutionY));
    for (int j = 0; j <= columns; j++) {
        line.setPosition(j * boxSize, 0);
        window.draw(line);
    }
}

Food::Food() {
    srand(time(0));
    generateFood();
}

void Food::generateFood() {
    x = rand() % gridColumns;
    y = rand() % gridRows;
}

void Food::draw(sf::RenderWindow& window, sf::RectangleShape& foodShape) {
    foodShape.setPosition(x * boxSize, y * boxSize);
    window.draw(foodShape);
}

Snake::Snake() : length(1), direction(UP), growFlag(false) {
    segments[0][0] = gridColumns / 2;
    segments[0][1] = gridRows / 2;    
}

void Snake::changeDirection(Direction newDirection) {
    if ((direction == UP && newDirection != DOWN) ||
        (direction == DOWN && newDirection != UP) ||
        (direction == LEFT && newDirection != RIGHT) ||
        (direction == RIGHT && newDirection != LEFT)) {
        direction = newDirection;
    }
}

void Snake::move() {
    for (int i = length - 1; i > 0; --i) {
        segments[i][0] = segments[i - 1][0];
        segments[i][1] = segments[i - 1][1];
    }

    switch (direction) {
    case UP: segments[0][1]--; break;
    case DOWN: segments[0][1]++; break;
    case LEFT: segments[0][0]--; break;
    case RIGHT: segments[0][0]++; break;
    }

    if (growFlag) {
        ++length;
        growFlag = false;
    }
}

void Snake::grow() { growFlag = true; }

bool Snake::checkCollision() {
    if (segments[0][0] < 0 || segments[0][0] >= gridColumns || segments[0][1] < 0 || segments[0][1] >= gridRows) {
        return true;
    }

    for (int i = 1; i < length; i++) {
        if (segments[i][0] == segments[0][0] && segments[i][1] == segments[0][1]) {
            return true;
        }
    }

    return false;
}

void Snake::draw(sf::RenderWindow& window, sf::RectangleShape& snakeShape) {
    for (int i = 0; i < length; i++) {
        snakeShape.setPosition(segments[i][0] * boxSize, segments[i][1] * boxSize);
        window.draw(snakeShape);
    }
}

Game::Game() : grid(gridRows, gridColumns, boxSize), score(0), state(GAME_RUNNING) {}

void Game::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) snake.changeDirection(UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) snake.changeDirection(DOWN);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) snake.changeDirection(LEFT);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.changeDirection(RIGHT);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (state == GAME_RUNNING) state = GAME_PAUSED;
        else if (state == GAME_PAUSED) state = GAME_RUNNING;
    }
}

void Game::update() {
    if (state == GAME_OVER || state == GAME_PAUSED) return;

    snake.move();

    if (snake.segments[0][0] == food.x && snake.segments[0][1] == food.y) {
        snake.grow();
        food.generateFood();
        score += 10;
    }

    if (snake.checkCollision()) {
        state = GAME_OVER;
    }
}

void Game::render(sf::RenderWindow& window) {
    if (state == GAME_OVER) {
        sf::Font font;
        if (font.loadFromFile("Movistar Text Regular.ttf")) {
            sf::Text gameOverText("Game Over! Press R to Restart", font, 30);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(resolutionX / 4, resolutionY / 2);
            window.draw(gameOverText);
        }
        return;
    }

    if (state == GAME_PAUSED) {
        sf::Font font;
        if (font.loadFromFile("arial.ttf")) {
            sf::Text pauseText("Game Paused! Press Space to Resume", font, 30);
            pauseText.setFillColor(sf::Color::Yellow);
            pauseText.setPosition(resolutionX / 4, resolutionY / 2);
            window.draw(pauseText);
        }
        return;
    }

    grid.renderGrid(window);

    sf::RectangleShape foodShape(sf::Vector2f(boxSize, boxSize));
    foodShape.setFillColor(sf::Color::Red);
    food.draw(window, foodShape);

    sf::RectangleShape snakeShape(sf::Vector2f(boxSize - 2, boxSize - 2));
    snakeShape.setFillColor(sf::Color::Green);

    snake.draw(window, snakeShape);
}

void Game::restart() {
    state = GAME_RUNNING;
    snake = Snake();
    food.generateFood();
    score = 0;
}
