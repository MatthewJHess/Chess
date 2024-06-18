#pragma once

#include <SFML/Graphics.hpp>
#include "screen.h"

void drawSettingsScreen(sf::RenderWindow& window, sf::Font& font, sf::CircleShape& musicSliderHandle, sf::CircleShape& soundSliderHandle);
void handleSettingsClick(sf::Vector2i mousePos, Screen& currentScreen, sf::RenderWindow& window, sf::CircleShape& musicSliderHandle, sf::CircleShape& soundSliderHandle, bool& isDraggingMusicSlider, bool& isDraggingSoundSlider);


