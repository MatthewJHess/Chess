#pragma once

#include <SFML/Graphics.hpp>
#include "screen.h"

void drawMenuScreen(sf::RenderWindow& window, sf::Font& font);
void handleMenuClick(sf::Vector2i mousePos, Screen& currentScreen, sf::RenderWindow& window);

