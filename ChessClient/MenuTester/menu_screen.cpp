#include "menu_screen.h"
#include "screen.h"
#include <iostream>

void drawMenuScreen(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape playButton(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Green);
    playButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 75);

    sf::Text playButtonText("Play", font, 24);
    playButtonText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 - 65);

    sf::RectangleShape settingsButton(sf::Vector2f(200, 50));
    settingsButton.setFillColor(sf::Color::Blue);
    settingsButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 25);

    sf::Text settingsButtonText("Settings", font, 24);
    settingsButtonText.setPosition(window.getSize().x / 2 - 70, window.getSize().y / 2 + 35);

    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 125);

    sf::Text quitButtonText("Quit", font, 24);
    quitButtonText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 + 135);

    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(settingsButton);
    window.draw(settingsButtonText);
    window.draw(quitButton);
    window.draw(quitButtonText);
}

void handleMenuClick(sf::Vector2i mousePos, Screen& currentScreen, sf::RenderWindow& window) {
    if (mousePos.x >= window.getSize().x / 2 - 100 && mousePos.x <= window.getSize().x / 2 + 100) {
        if (mousePos.y >= window.getSize().y / 2 - 75 && mousePos.y <= window.getSize().y / 2 - 25) {
            currentScreen = Screen::Game;
        }
        else if (mousePos.y >= window.getSize().y / 2 + 25 && mousePos.y <= window.getSize().y / 2 + 75) {
            currentScreen = Screen::Settings;
        }
        else if (mousePos.y >= window.getSize().y / 2 + 125 && mousePos.y <= window.getSize().y / 2 + 175) {
            window.close();
        }
    }
}
