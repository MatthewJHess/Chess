#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "menu_screen.h"
#include "settings_screen.h"
#include "chess_game.h"

// client game
//
// 
// 
//
int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Client");

    sf::TcpSocket socket;
    if (socket.connect("127.0.0.1", 54000) != sf::Socket::Done) {
        std::cerr << "Error connecting to server!" << std::endl;
        return -1;
    }

    bool isDraggingMusicSlider = false;
    bool isDraggingSoundSlider = false;
    bool isWhiteTurn = false;

    window.setFramerateLimit(144);

    // Font for the text
    sf::Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Define current screen
    Screen currentScreen = Screen::Menu;

    // Define slider handles
    sf::CircleShape musicSliderHandle(10);
    musicSliderHandle.setFillColor(sf::Color::Red);

    sf::CircleShape soundSliderHandle(10);
    soundSliderHandle.setFillColor(sf::Color::Red);

    ChessGame chessGame(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (currentScreen == Screen::Menu) {
                    handleMenuClick(mousePos, currentScreen, window);
                }
                else if (currentScreen == Screen::Settings) {
                    handleSettingsClick(mousePos, currentScreen, window, musicSliderHandle, soundSliderHandle, isDraggingMusicSlider, isDraggingSoundSlider);
                }
                else if (currentScreen == Screen::Game && !isWhiteTurn) {
                    chessGame.handleMouseClick(mousePos);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (currentScreen == Screen::Settings) {
                    isDraggingMusicSlider = false;
                    isDraggingSoundSlider = false;
                }
                else if (currentScreen == Screen::Game) {
                    if (!isWhiteTurn) {
                        chessGame.handleMouseRelease(sf::Mouse::getPosition(window));
                        // Send the move to the server
                        sf::Packet packet;
                        packet << chessGame.getLastMove();
                        std::cout << packet << std::endl;
                        if (socket.send(packet) != sf::Socket::Done) {
                            std::cerr << "Error sending move to server!" << std::endl;
                        }
                        isWhiteTurn = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (currentScreen == Screen::Settings) {
                    if (isDraggingMusicSlider) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        float newX = mousePos.x - musicSliderHandle.getRadius();
                        if (newX < window.getSize().x / 2 - 100)
                            newX = window.getSize().x / 2 - 100;
                        if (newX > window.getSize().x / 2 + 100 - 2 * musicSliderHandle.getRadius())
                            newX = window.getSize().x / 2 + 100 - 2 * musicSliderHandle.getRadius();
                        musicSliderHandle.setPosition(newX, musicSliderHandle.getPosition().y);
                    }
                    else if (isDraggingSoundSlider) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        float newX = mousePos.x - soundSliderHandle.getRadius();
                        if (newX < window.getSize().x / 2 - 100)
                            newX = window.getSize().x / 2 - 100;
                        if (newX > window.getSize().x / 2 + 100 - 2 * soundSliderHandle.getRadius())
                            newX = window.getSize().x / 2 + 100 - 2 * soundSliderHandle.getRadius();
                        soundSliderHandle.setPosition(newX, soundSliderHandle.getPosition().y);
                    }
                }
                else if (currentScreen == Screen::Game) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    chessGame.handleMouseMove(mousePos);
                }
            }
        }

        // Receive server's move
        if (isWhiteTurn) {
            sf::Packet packet;
            if (socket.receive(packet) == sf::Socket::Done) {
                std::string command, move;
                packet >> move;
                std::cout << "Received command: " << move << std::endl;
                if (move.size() >= 4) { // Validate received move
                    chessGame.processMove(move);
                    isWhiteTurn = false;
                }
                else {
                    std::cerr << "Received invalid command or move" << std::endl;
                }
            }
        }

        window.clear();
        if (currentScreen == Screen::Menu) {
            drawMenuScreen(window, font);
        }
        else if (currentScreen == Screen::Settings) {
            drawSettingsScreen(window, font, musicSliderHandle, soundSliderHandle);
        }
        else if (currentScreen == Screen::Game) {
            chessGame.draw(window);
        }
        window.display();
    }

    return 0;
}
