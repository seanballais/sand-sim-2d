#include <algorithm>
#include <iostream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2f> sandLocs);
void applyPhysics(std::vector<sf::Vector2f>* sandLocs, float deltaTime);
sf::Vector2i getCellPositionFromMouse(sf::RenderWindow& window);
sf::Vector2i getCellGridPosition(int x, int y);

float physicsAccumulator = 0.f;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int SAND_SIZE = 2;

int main()
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "Sands Sim 2D",
                          sf::Style::Close);

  sf::RectangleShape rectangle(sf::Vector2f((float) SAND_SIZE,
                                            (float) SAND_SIZE));
  rectangle.setFillColor(sf::Color(255, 255, 153));

  // We need to use a float vector so that things will be easier for our
  // physics "engine".
  std::vector<sf::Vector2f> sandLocs;

  sf::Clock appClock;

  bool isPhysicsEnabled = true;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mouseCellPos = getCellPositionFromMouse(window);
            sandLocs.push_back(sf::Vector2f((float) mouseCellPos.x,
                                            (float) mouseCellPos.y));
          }
          break;
        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::C) {
            sandLocs.clear();
          } else if (event.key.code == sf::Keyboard::P) {
            isPhysicsEnabled = !isPhysicsEnabled;
          }
          break;
      }
    }

    sf::Time timeElapsed = appClock.restart();
    float deltaTime = timeElapsed.asMilliseconds() / 1000.f;

    window.clear(sf::Color::Black);

    draw(window, &rectangle, sandLocs);

    window.display();

    if (isPhysicsEnabled) {
      applyPhysics(&sandLocs, deltaTime);
    }
  }

  return 0;
}

void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2f> sandLocs)
{
  // Draw the cursor.
  sf::Vector2i cursorPos = getCellPositionFromMouse(window);
  rectangle->setPosition((float) cursorPos.x, (float) cursorPos.y);
  window.draw(*rectangle);

  // Draw the other sand grains.
  for (auto it = sandLocs.begin(); it != sandLocs.end(); it++) {
    // Put the rectangles in the appropriate grids first.
    sf::Vector2i gridifiedPos = getCellGridPosition((int) it->x, (int) it->y);
    rectangle->setPosition((float) gridifiedPos.x, (float) gridifiedPos.y);
    window.draw(*rectangle);
  }
}

void applyPhysics(std::vector<sf::Vector2f>* sandLocs, float deltaTime)
{
  // Just really about applying physics to the sand grains.
  // Assume gravity puts the grains 150 cells down on every second.
  const float PHYSICS_TIME_STEP = 1.f / 30.f;
  const float GRAVITY = 150.f; // Origin starts from the top-left corner.
  const float Y_DELTA = GRAVITY * PHYSICS_TIME_STEP;

  physicsAccumulator += deltaTime;
  while (physicsAccumulator >= PHYSICS_TIME_STEP) {
    for (auto it = sandLocs->begin(); it != sandLocs->end(); it++) {
      it->y = std::min(it->y + Y_DELTA, (float) WINDOW_HEIGHT * 0.75f);
    }

    physicsAccumulator -= PHYSICS_TIME_STEP;
  }
}

sf::Vector2i getCellPositionFromMouse(sf::RenderWindow& window)
{
  sf::Vector2i currMousePos = sf::Mouse::getPosition(window);
  return getCellGridPosition(currMousePos.x, currMousePos.y);
}

sf::Vector2i getCellGridPosition(int x, int y)
{
  int xCellDelta = x % 2;
  int yCellDelta = y % 2;
  int cellXPos = x - xCellDelta;
  int cellYPos = y - yCellDelta;
  return sf::Vector2i(cellXPos, cellYPos);
}
