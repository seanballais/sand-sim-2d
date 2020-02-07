#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2i> sandLocs);
sf::Vector2i getCellPositionFromMouse(sf::RenderWindow& window);

int main()
{
  const int WINDOW_WIDTH = 640;
  const int WINDOW_HEIGHT = 640;
  const int SAND_SIZE = 2;

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "Sands Sim 2D",
                          sf::Style::Close);

  sf::RectangleShape rectangle(sf::Vector2f((float) SAND_SIZE,
                                            (float) SAND_SIZE));
  rectangle.setFillColor(sf::Color(255, 255, 153));

  std::vector<sf::Vector2i> sandLocs;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left) {
            sandLocs.push_back(getCellPositionFromMouse(window));
          }
          break;
        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::C) {
            sandLocs.clear();
          }
          break;
      }

      window.clear(sf::Color::Black);

      draw(window, &rectangle, sandLocs);

      window.display();
    }
  }

  return 0;
}

void draw(sf::RenderWindow& window,
          sf::RectangleShape* rectangle,
          std::vector<sf::Vector2i> sandLocs)
{
  // Draw the cursor.
  sf::Vector2i currMousePos = getCellPositionFromMouse(window);
  sandLocs.push_back(currMousePos);

  // Draw the other sand grains.
  for (auto it = sandLocs.begin(); it != sandLocs.end(); it++) {
    rectangle->setPosition((float) it->x, (float) it->y);
    window.draw(*rectangle);
  }
}

sf::Vector2i getCellPositionFromMouse(sf::RenderWindow& window)
{
  sf::Vector2i currMousePos = sf::Mouse::getPosition(window);
  return sf::Vector2i(currMousePos.x / 2, currMousePos.y / 2);
}
