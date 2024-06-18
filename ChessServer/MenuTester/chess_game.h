#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn };
enum class Color { White, Black };

struct ChessPiece {
    sf::Texture texture;
    PieceType type;
    Color color;
    sf::Vector2i position;
    bool captured;

    ChessPiece(sf::Texture& tex, PieceType t, Color c, sf::Vector2i pos)
        : texture(tex), type(t), color(c), position(pos), captured(false) {}
};

class ChessGame {
public:
    ChessGame(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void handleMouseClick(sf::Vector2i mousePos);
    void handleMouseRelease(sf::Vector2i mousePos);
    void handleMouseMove(sf::Vector2i mousePos);
    bool isGameOver() const { return gameOver; }
    bool itsWhiteTurn() const { return &isWhiteTurn; }
    bool isGameStarted() const { return gameStarted; }
    void startNewGame(sf::RenderWindow& window);
    std::string getLastMove();
    void processMove(const std::string& move);
    int fileToIndex(char file);
    int rankToIndex(char rank);
    std::string lastMove;

private:
    void loadTextures(sf::RenderWindow& window);
    void initializeBoard(sf::RenderWindow& window);
    sf::Vector2f getBoardPosition(sf::Vector2i mousePos);
    bool isValidMove(sf::Vector2f startPos, sf::Vector2f endPos);
    ChessPiece* getPieceAt(sf::Vector2i position);
    bool dragging = false;
    sf::Vector2f pieceOff = { 30.f,30.f };
    bool gameOver; // Flag to indicate game over
    bool winner = 0; //0 for white win and 1 for black;
    bool gameStarted;
    sf::Sprite victorySprite;
    sf::Texture defeatTexture;
    sf::Sprite defeatSprite;

    bool isValidPawnMove(sf::Vector2f startPos, sf::Vector2f endPos, Color color);
    bool isValidRookMove(sf::Vector2f startPos, sf::Vector2f endPos);
    bool isValidKnightMove(sf::Vector2f startPos, sf::Vector2f endPos);
    bool isValidBishopMove(sf::Vector2f startPos, sf::Vector2f endPos);
    bool isValidQueenMove(sf::Vector2f startPos, sf::Vector2f endPos);
    bool isValidKingMove(sf::Vector2f startPos, sf::Vector2f endPos);
    void displayVictoryScreen(Color winnerColor);

    void trackMousePosition(sf::Vector2i mousePos);

    std::vector<ChessPiece> pieces;
    sf::Texture whiteKingTexture, whiteQueenTexture, whiteRookTexture, whiteBishopTexture, whiteKnightTexture, whitePawnTexture;
    sf::Texture blackKingTexture, blackQueenTexture, blackRookTexture, blackBishopTexture, blackKnightTexture, blackPawnTexture;
    sf::Texture chessBoardTexture, victoryTexture;
    ChessPiece* selectedPiece;
    sf::Vector2i selectedPieceOriginalPos;
    bool isDragging;
    bool isWhiteTurn;

    float boardSize;
    float squareSize;
    sf::Vector2f boardOffset;
    float windowHeight;
    float border;
};
