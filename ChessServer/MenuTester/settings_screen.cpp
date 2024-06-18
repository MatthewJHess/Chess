#include "settings_screen.h"
#include <iostream>

void drawSettingsScreen(sf::RenderWindow& window, sf::Font& font, sf::CircleShape& musicSliderHandle, sf::CircleShape& soundSliderHandle) {
    sf::Text musicText("Music", font, 24);
    musicText.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 100);

    sf::RectangleShape musicSlider(sf::Vector2f(200, 10));
    musicSlider.setFillColor(sf::Color::Blue);
    musicSlider.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 50);

    sf::Text soundText("Sound Effects", font, 24);
    soundText.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50);

    sf::RectangleShape soundSlider(sf::Vector2f(200, 10));
    soundSlider.setFillColor(sf::Color::Green);
    soundSlider.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 100);

    sf::RectangleShape backButton(sf::Vector2f(200, 50));
    backButton.setFillColor(sf::Color::Red);
    backButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 200);

    sf::Text backButtonText("Back", font, 24);
    backButtonText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 + 210);

    window.draw(musicSlider);
    window.draw(musicSliderHandle);
    window.draw(musicText);
    window.draw(soundSlider);
    window.draw(soundSliderHandle);
    window.draw(soundText);
    window.draw(backButton);
    window.draw(backButtonText);
}

void handleSettingsClick(sf::Vector2i mousePos, Screen& currentScreen, sf::RenderWindow& window, sf::CircleShape& musicSliderHandle, sf::CircleShape& soundSliderHandle, bool& isDraggingMusicSlider, bool& isDraggingSoundSlider) {
    if (mousePos.x >= window.getSize().x / 2 - 50 && mousePos.x <= window.getSize().x / 2 + 100 && mousePos.y >= window.getSize().y / 2 + 210 && mousePos.y <= window.getSize().y / 2 + 260) {
        currentScreen = Screen::Menu;
    }
    else if (musicSliderHandle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        isDraggingMusicSlider = true;
    }
    else if (soundSliderHandle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        isDraggingSoundSlider = true;
    }
}
