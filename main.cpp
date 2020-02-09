#include <algorithm>
#include <climits>
#include <random>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

std::string getBinFolder();
void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2i> sandLocs);
void drawFPS(sf::RenderWindow& window, int fps, sf::Text& fpsText);
void applyPhysics(std::vector<sf::Vector2i>* sandLocs, float deltaTime);
sf::Vector2i getCellPositionFromMouse(sf::RenderWindow& window);
sf::Vector2i getCellGridPosition(int x, int y);
bool isSandGrainExists(std::vector<sf::Vector2i>& sandLocs, sf::Vector2i grain);

// Eek! Global variables. It's okay for this occasion though since we're just
// working on a demo.
float physicsAccumulator = 0.f;
std::default_random_engine randomGenerator;
std::uniform_int_distribution<int> numDistribution(0, 1);

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int SAND_SIZE = 8;

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
  std::vector<sf::Vector2i> sandLocs;
  
  sf::Font fpsFont;
  fpsFont.loadFromFile(getBinFolder() + "/data/SourceCodePro-Regular.ttf");
  sf::Text fpsText;
  fpsText.setFont(fpsFont);
  fpsText.setCharacterSize(16);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition(sf::Vector2f(15.f, 15.f));

  sf::Clock appClock;

  bool isPhysicsEnabled = true;

  int fps = 0;

  // To allow limit the spawning of new sand grains to every 50ms.
  float grainSpawnerAccumulator = 0.0f;
  while (window.isOpen()) {
    sf::Time timeElapsed = appClock.restart();
    float deltaTime = timeElapsed.asMilliseconds() / 1000.f;
    fps = (int) (1.f / deltaTime);

    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
        && grainSpawnerAccumulator >= 0.05f) {
      sf::Vector2i mouseCellPos = getCellPositionFromMouse(window);
      sandLocs.push_back(sf::Vector2i(mouseCellPos.x, mouseCellPos.y));

      grainSpawnerAccumulator = 0.0f;
    }

    grainSpawnerAccumulator += deltaTime;

    window.clear(sf::Color::Black);

    draw(window, &rectangle, sandLocs);
    drawFPS(window, fps, fpsText);

    window.display();

    if (isPhysicsEnabled) {
      applyPhysics(&sandLocs, deltaTime);
    }
  }

  return 0;
}

std::string getBinFolder()
{
  // NOTE: THIS WILL ONLY WORK IN LINUX SYSTEMS.
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  std::string binPath = std::string(path, (count > 0) ? count : 0);

  return std::filesystem::path(binPath).parent_path();
}

void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2i> sandLocs)
{
  // Draw the cursor.
  sf::Vector2i cursorPos = getCellPositionFromMouse(window);
  rectangle->setPosition((float) cursorPos.x, (float) cursorPos.y);
  window.draw(*rectangle);

  // Draw the other sand grains.
  for (auto it = sandLocs.begin(); it != sandLocs.end(); it++) {
    // Put the rectangles in the appropriate grids first.
    rectangle->setPosition((float) it->x, (float) it->y);
    window.draw(*rectangle);
  }
}

void drawFPS(sf::RenderWindow& window, int fps, sf::Text& fpsText)
{
  fpsText.setString("FPS: " + std::to_string(fps));
  window.draw(fpsText);
}

void applyPhysics(std::vector<sf::Vector2i>* sandLocs, float deltaTime)
{
  // Just really about applying physics to the sand grains.
  const float PHYSICS_TIME_STEP = 1.f / 30.f;
  const int DELTA = SAND_SIZE;

  physicsAccumulator += deltaTime;
  while (physicsAccumulator >= PHYSICS_TIME_STEP) {
    for (auto it = sandLocs->begin(); it != sandLocs->end(); it++) {
      sf::Vector2i possibleBottomGrain(it->x, it->y + DELTA);
      if (isSandGrainExists(*sandLocs, possibleBottomGrain)) {
        // Move the grain either to the left or right.
        int grainDirection = numDistribution(randomGenerator);
        switch (grainDirection) {
          case 0:
            // Let's move the grain to the left.
            {
              sf::Vector2i possibleBottomLeftGrain(it->x - DELTA,
                                                   it->y + DELTA);
              if (!isSandGrainExists(*sandLocs, possibleBottomLeftGrain)) {
                it->x -= DELTA;
              }
            }
            break;
          case 1:
            // Let's move the grain to the right.
            {
              sf::Vector2i possibleBottomRightGrain(it->x + DELTA,
                                                    it->y + DELTA);
              if (!isSandGrainExists(*sandLocs, possibleBottomRightGrain)) {
                it->x += DELTA;
              }
            }
            break;
        }
      } else {
        it->y = std::min(it->y + DELTA, (int) (WINDOW_HEIGHT * 0.75f));
      }
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
  int xCellDelta = x % SAND_SIZE;
  int yCellDelta = y % SAND_SIZE;
  int cellXPos = x - xCellDelta;
  int cellYPos = y - yCellDelta;
  return sf::Vector2i(cellXPos, cellYPos);
}

bool isSandGrainExists(std::vector<sf::Vector2i>& sandLocs, sf::Vector2i grain)
{
  if (std::find(sandLocs.begin(), sandLocs.end(), grain) != sandLocs.end()) {
    return true;
  }

  return false;
}
