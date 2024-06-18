#include "chess_game.h"
#include <iostream>
#include <cmath>


//TBD: Resync up to validate


bool loadTexture(sf::Texture& texture, const std::string& filepath) {
    if (!texture.loadFromFile(filepath)) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return false;
    }
    return true;
}

ChessGame::ChessGame(sf::RenderWindow& window) : selectedPiece(nullptr), isDragging(false), isWhiteTurn(true), gameOver(false), gameStarted(true) {
    loadTextures(window);
    initializeBoard(window);
}

void ChessGame::loadTextures(sf::RenderWindow& window) {

    loadTexture(chessBoardTexture, "C:/GameMaps/chess_board.jpg");
    loadTexture(whiteKingTexture, "C:/GameMaps/white-king.png");
    loadTexture(whiteQueenTexture, "C:/GameMaps/white-queen.png");
    loadTexture(whiteRookTexture, "C:/GameMaps/white-rook.png");
    loadTexture(whiteBishopTexture, "C:/GameMaps/white-bishop.png");
    loadTexture(whiteKnightTexture, "C:/GameMaps/white-knight.png");
    loadTexture(whitePawnTexture, "C:/GameMaps/white-pawn.png");

    loadTexture(blackKingTexture, "C:/GameMaps/black-king.png");
    loadTexture(blackQueenTexture, "C:/GameMaps/black-queen.png");
    loadTexture(blackRookTexture, "C:/GameMaps/black-rook.png");
    loadTexture(blackBishopTexture, "C:/GameMaps/black-bishop.png");
    loadTexture(blackKnightTexture, "C:/GameMaps/black-knight.png");
    loadTexture(blackPawnTexture, "C:/GameMaps/black-pawn.png");
    loadTexture(victoryTexture, "C:/GameMaps/Winner.png");
    loadTexture(defeatTexture, "C:/GameMaps/Defeat.png");

    sf::Texture pieceOffs;
    if (!pieceOffs.loadFromFile("C:/GameMaps/black-pawn.png")) {
        std::cerr << "Failed to load texture: C:/GameMaps/black-pawn.png" << std::endl;
        // Handle the error appropriately (e.g., return from the function, set a default texture, etc.)
    }


    if (victoryTexture.getSize() != sf::Vector2u(0, 0)) {
        this->victorySprite.setTexture(victoryTexture);

        sf::Vector2u textureSize = victoryTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        victorySprite.setPosition(float(windowSize.x) / 2, float(windowSize.x) / 4);
        victorySprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
        float scaleMin = std::min(float(windowSize.x) / textureSize.x / 2, float(windowSize.y) / textureSize.y / 2);
        victorySprite.setScale(
            scaleMin,
            scaleMin
        );
    }
    if (defeatTexture.getSize() != sf::Vector2u(0, 0)) {
        this->defeatSprite.setTexture(defeatTexture);
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = defeatTexture.getSize();
        defeatSprite.setPosition(float(windowSize.x) / 2, float(windowSize.x) / 4);
        defeatSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
        float scaleMin = std::min(float(windowSize.x) / textureSize.x / 3, float(windowSize.y) / textureSize.y / 3);
        defeatSprite.setScale(
            scaleMin,
            scaleMin
        );
    }

    sf::Vector2f pieceOff = { 30.f,30.f };
}

void ChessGame::initializeBoard(sf::RenderWindow& window) {
    windowHeight = window.getSize().y;
    boardSize = windowHeight * 0.9f; // 90% of the window height
    border = windowHeight * 0.05f;
    squareSize = boardSize / 8.0f;
    boardOffset = sf::Vector2f((window.getSize().x - boardSize) / 2.0f, (windowHeight - boardSize) / 2.0f);

    // Initialize white pieces
    pieces = {
        { whiteRookTexture, PieceType::Rook, Color::White, {0, 0} },
        { whiteKnightTexture, PieceType::Knight, Color::White, {1, 0} },
        { whiteBishopTexture, PieceType::Bishop, Color::White, {2, 0} },
        { whiteQueenTexture, PieceType::Queen, Color::White, {3, 0} },
        { whiteKingTexture, PieceType::King, Color::White, {4, 0} },
        { whiteBishopTexture, PieceType::Bishop, Color::White, {5, 0} },
        { whiteKnightTexture, PieceType::Knight, Color::White, {6, 0} },
        { whiteRookTexture, PieceType::Rook, Color::White, {7, 0} }
    };

    for (int i = 0; i < 8; ++i) {
        pieces.push_back({ whitePawnTexture, PieceType::Pawn, Color::White, {i, 1} });
    }

    // Initialize black pieces
    pieces.push_back({ blackRookTexture, PieceType::Rook, Color::Black, {0, 7} });
    pieces.push_back({ blackKnightTexture, PieceType::Knight, Color::Black, {1, 7} });
    pieces.push_back({ blackBishopTexture, PieceType::Bishop, Color::Black, {2, 7} });
    pieces.push_back({ blackQueenTexture, PieceType::Queen, Color::Black, {3, 7} });
    pieces.push_back({ blackKingTexture, PieceType::King, Color::Black, {4, 7} });
    pieces.push_back({ blackBishopTexture, PieceType::Bishop, Color::Black, {5, 7} });
    pieces.push_back({ blackKnightTexture, PieceType::Knight, Color::Black, {6, 7} });
    pieces.push_back({ blackRookTexture, PieceType::Rook, Color::Black, {7, 7} });

    for (int i = 0; i < 8; ++i) {
        pieces.push_back({ blackPawnTexture, PieceType::Pawn, Color::Black, {i, 6} });
    }
}

void ChessGame::draw(sf::RenderWindow& window) {
    windowHeight = window.getSize().y;
    boardSize = windowHeight * 0.9f; // 90% of the window height
    squareSize = boardSize / 8.f;
    boardOffset = sf::Vector2f((window.getSize().x - boardSize) / 2.0f, (windowHeight - boardSize) / 2.0f);

    if (gameOver) {
        if (winner) window.draw(this->victorySprite);
        else window.draw(this->defeatSprite);
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(boardOffset.x + i * squareSize, boardOffset.y + (7 - j) * squareSize);
            if ((i + j) % 2 == 0) {
                square.setFillColor(sf::Color::White);
            }
            else {
                square.setFillColor(sf::Color::Blue);
            }
            window.draw(square);
        }
    }

    for (const auto& piece : pieces) {
        sf::Sprite sprite(piece.texture);
        if (piece.captured) {
            continue; // Skip drawing captured pieces
        }
        if (isDragging && &piece == selectedPiece) {
            sprite.setPosition(static_cast<sf::Vector2f>(piece.position) - pieceOff);
        }
        else {
            sprite.setPosition(piece.position.x * squareSize + boardOffset.x, (7 - piece.position.y) * squareSize + boardOffset.y);
        }
        window.draw(sprite);
    }
    if (gameOver) {
        if (winner) window.draw(this->victorySprite);
        else window.draw(this->defeatSprite);
    }
}

int ChessGame::fileToIndex(char file) {
    return file - 'a'; // Example: 'a' -> 0, 'b' -> 1, ..., 'h' -> 7
}

int ChessGame::rankToIndex(char rank) {
    return '8' - rank; // Example: '8' -> 0, '7' -> 1, ..., '1' -> 7
}

void ChessGame::handleMouseClick(sf::Vector2i mousePos) {
    sf::Vector2f worldPos = static_cast<sf::Vector2f>(mousePos);
    for (auto& piece : pieces) {
        sf::Sprite sprite(piece.texture);
        sprite.setPosition(piece.position.x * squareSize + boardOffset.x, (7 - piece.position.y) * squareSize + boardOffset.y);
        if (sprite.getGlobalBounds().contains(worldPos)) {
            if ((isWhiteTurn && piece.color == Color::White) || (!isWhiteTurn && piece.color == Color::Black)) {
                selectedPiece = &piece;
                selectedPieceOriginalPos = piece.position; // Store the original board position as sf::Vector2i
                isDragging = true;
                return;
            }
        }
    }
}

void ChessGame::handleMouseRelease(sf::Vector2i mousePos) {
    if (selectedPiece) {
        sf::Vector2f boardPos = getBoardPosition(mousePos);
        boardPos.x = std::floor(boardPos.x);
        boardPos.y = std::floor(boardPos.y);
        boardPos.x = std::max(0.f, std::min(7.f, boardPos.x));
        boardPos.y = std::max(0.f, std::min(7.f, boardPos.y));
        sf::Vector2i newBoardPos = static_cast<sf::Vector2i>(boardPos);
        std::string move;
        move += selectedPieceOriginalPos.x + '0';
        move += selectedPieceOriginalPos.y + '0';
        move += newBoardPos.x + '0';
        move += newBoardPos.y + '0';

        selectedPiece->position = selectedPieceOriginalPos;

        std::cout << "Attempting move from (" << selectedPieceOriginalPos.x << ", " << selectedPieceOriginalPos.y << ") to (" << newBoardPos.x << ", " << newBoardPos.y << ")" << std::endl;

        if (isValidMove(static_cast<sf::Vector2f>(selectedPieceOriginalPos), boardPos)) {
            lastMove = move;
            ChessPiece* targetPiece = getPieceAt(newBoardPos);
            if (targetPiece) {
                std::cout << "Target piece at (" << newBoardPos.x << ", " << newBoardPos.y << ") of color " << (targetPiece->color == Color::White ? "White" : "Black") << std::endl;
                if (targetPiece->color == selectedPiece->color) {
                    selectedPiece->position = selectedPieceOriginalPos;
                    std::cout << "Cannot capture piece of the same color. Reverting to original position." << std::endl;
                }
                else {
                    targetPiece->position = { -1,-1 };
                    targetPiece->captured = true;
                    selectedPiece->position = newBoardPos;
                    if (targetPiece->type == PieceType::King) {
                        if (targetPiece->color == Color::White) {
                            // Black wins - display victory screen
                            std::cout << "White's king is captured. Black wins!" << std::endl;
                            // Call a function to display victory screen
                            gameStarted = false;
                            gameOver = true;
                            winner = 1;
                        }
                        else {
                            // White wins - display victory screen
                            std::cout << "Black's king is captured. White wins!" << std::endl;
                            // Call a function to display victory screen
                            gameStarted = false;
                            gameOver = true;
                            winner = 0;
                        }
                        // Optionally, end the game loop or handle other game-ending logic
                    }
                    else {
                        isWhiteTurn = !isWhiteTurn; // Switch turns if it's not a king capture
                    }
                }
            }
            else {
                selectedPiece->position = newBoardPos;
                std::cout << "Moved piece to (" << newBoardPos.x << ", " << newBoardPos.y << ")" << std::endl;
                isWhiteTurn = !isWhiteTurn; // Switch turns
            }
        }
        else {
            selectedPiece->position = selectedPieceOriginalPos;
            std::cout << "Invalid move. Reverted to original position (" << selectedPieceOriginalPos.x << ", " << selectedPieceOriginalPos.y << ")" << std::endl;
        }

        // Print out all piece positions for debugging
        std::cout << "Piece positions after move:" << std::endl;
        for (const auto& piece : pieces) {
            std::cout << "Piece: " << (piece.color == Color::White ? "White" : "Black")
                << " Position: (" << piece.position.x << ", " << piece.position.y << ")" << std::endl;
        }

        selectedPiece = nullptr;
        isDragging = false;
    }
}





void ChessGame::handleMouseMove(sf::Vector2i mousePos) {
    //trackMousePosition(mousePos);
    if (isDragging && selectedPiece) {
        // Convert the mouse position to world coordinates, adjusting for board offset
        selectedPiece->position = sf::Vector2i(mousePos - static_cast<sf::Vector2i>(pieceOff));
    }
}

void ChessGame::startNewGame(sf::RenderWindow& window) {
    // Reset game variables for a new game
    pieces.clear(); // Clear previous game pieces
    // Initialize new game pieces and other necessary variables
    // Example:
    // pieces.push_back(ChessPiece(...));
    initializeBoard(window);
    gameStarted = true;
    gameOver = false;
    isWhiteTurn = true; // Assuming white starts first
}

std::string ChessGame::getLastMove() {
    // For example, return "e2e4" for a move from e2 to e4
    return lastMove;
}

void ChessGame::processMove(const std::string& move) {
    // Example implementation (assuming you have a chess board represented as a matrix)
    char fromFile = move[0]; // Get the file of the starting square
    char fromRank = move[1]; // Get the rank of the starting square
    char toFile = move[2];   // Get the file of the ending square
    char toRank = move[3];   // Get the rank of the ending square

    // Convert file and rank characters to board indices (if using a board matrix)
    int fromFileIndex = fromFile - '0';
    int fromRankIndex = fromRank - '0';
    int toFileIndex = toFile - '0';
    int toRankIndex = toRank - '0';
    sf::Vector2i position = { fromFileIndex , fromRankIndex };
    sf::Vector2f boardPos = { static_cast<float>(fromFileIndex) , static_cast<float>(fromRankIndex) };
    sf::Vector2f newBoardPos = { static_cast<float>(toFileIndex) , static_cast<float>(toRankIndex) };
    for (auto& piece : pieces) {
        if (piece.position == position) {
            selectedPiece = &piece;
        }
    }
    if (isValidMove(boardPos, newBoardPos)) { // Implement your own logic to validate moves

        // Implement your logic to move the piece on the board
        if (isValidMove(static_cast<sf::Vector2f>(selectedPieceOriginalPos), boardPos)) {
            ChessPiece* targetPiece = getPieceAt(position);
            if (targetPiece) {
                std::cout << "Target piece at (" << newBoardPos.x << ", " << newBoardPos.y << ") of color " << (targetPiece->color == Color::White ? "White" : "Black") << std::endl;
                if (targetPiece->color == selectedPiece->color) {
                    selectedPiece->position = selectedPieceOriginalPos;
                    std::cout << "Cannot capture piece of the same color. Reverting to original position." << std::endl;
                }
                else {
                    targetPiece->position = { -1,-1 };
                    targetPiece->captured = true;
                    selectedPiece->position = position;
                    if (targetPiece->type == PieceType::King) {
                        if (targetPiece->color == Color::White) {
                            // Black wins - display victory screen
                            std::cout << "White's king is captured. Black wins!" << std::endl;
                            // Call a function to display victory screen
                            gameStarted = false;
                            gameOver = true;
                            winner = 1;
                        }
                        else {
                            // White wins - display victory screen
                            std::cout << "Black's king is captured. White wins!" << std::endl;
                            // Call a function to display victory screen
                            gameStarted = false;
                            gameOver = true;
                            winner = 0;
                        }
                        // Optionally, end the game loop or handle other game-ending logic
                    }
                    else {
                        isWhiteTurn = !isWhiteTurn; // Switch turns if it's not a king capture
                    }
                }
            }
            else {
                selectedPiece->position = position;
                std::cout << "Moved piece to (" << newBoardPos.x << ", " << newBoardPos.y << ")" << std::endl;
                isWhiteTurn = !isWhiteTurn; // Switch turns
            }
        }

        // Update lastMove member variable to store the move
        lastMove = move;
    }
    else {
        // Handle invalid move
        std::cerr << "Invalid move: " << move << std::endl;
    }
}



sf::Vector2f ChessGame::getBoardPosition(sf::Vector2i mousePos) {
    // Convert window coordinates to board coordinates
    sf::Vector2f boardPos;
    boardPos.x = (mousePos.x - boardOffset.x) / squareSize;
    boardPos.y = (windowHeight - mousePos.y - border) / squareSize;
    return boardPos;
}

std::string pieceTypeToString(PieceType type) {
    switch (type) {
    case PieceType::Pawn:   return "Pawn";
    case PieceType::Rook:   return "Rook";
    case PieceType::Knight: return "Knight";
    case PieceType::Bishop: return "Bishop";
    case PieceType::Queen:  return "Queen";
    case PieceType::King:   return "King";
    default:                return "Unknown";
    }
}

bool ChessGame::isValidMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    // Get piece at startPos
    sf::Vector2i startBoardPos = sf::Vector2i(startPos);
    sf::Vector2i endBoardPos = sf::Vector2i(endPos);
    ChessPiece* piece = getPieceAt(startBoardPos);
    std::cout << startBoardPos.x << ", " << startBoardPos.y << std::endl;
    std::cout << endBoardPos.x << ", " << endBoardPos.y << std::endl;
    if (!piece) return false;
    // Determine move validity based on piece type
    std::cout << "Piece type: " << pieceTypeToString(piece->type) << std::endl;
    switch (piece->type) {
    case PieceType::Pawn:
        std::cout << "Pawn move" << std::endl;
        return isValidPawnMove(startPos, endPos, piece->color);
    case PieceType::Rook:
        std::cout << "Rook move" << std::endl;
        return isValidRookMove(startPos, endPos);
    case PieceType::Knight:
        std::cout << "Knight move" << std::endl;
        return isValidKnightMove(startPos, endPos);
    case PieceType::Bishop:
        std::cout << "Bishop move" << std::endl;
        return isValidBishopMove(startPos, endPos);
    case PieceType::Queen:
        std::cout << "Queen move" << std::endl;
        return isValidQueenMove(startPos, endPos);
    case PieceType::King:
        std::cout << "King move" << std::endl;
        return isValidKingMove(startPos, endPos);
    default:
        std::cout << "Unknown piece type" << std::endl;
        return false;
    }
}

ChessPiece* ChessGame::getPieceAt(sf::Vector2i position) {
    for (auto& piece : pieces) {
        if (piece.position == position) {
            return &piece;
        }
    }
    return nullptr;
}

bool ChessGame::isValidPawnMove(sf::Vector2f startPos, sf::Vector2f endPos, Color color) {
    int direction = (color == Color::White) ? 1 : -1;
    endPos.x -= fmod(endPos.x, 1.f);
    endPos.y -= fmod(endPos.y, 1.f);
    std::cout << endPos.x << ", " << endPos.y << std::endl;
    if (startPos.x == endPos.x && startPos.y + direction == endPos.y) {
        // Standard move
        //std::cout << "Standard" << std::endl;
        return !getPieceAt(sf::Vector2i(endPos));
    }
    else if (startPos.x == endPos.x && startPos.y + 2 * direction == endPos.y && (startPos.y == 1 || startPos.y == 6)) {
        // Initial double move
        //std::cout << "Double" << std::endl;
        return !getPieceAt(sf::Vector2i(endPos)) && !getPieceAt(sf::Vector2i(startPos) + sf::Vector2i(0, direction));
    }
    else if (std::abs(startPos.x - endPos.x) == 1 && startPos.y + direction == endPos.y) {
        // Capture move
        //std::cout << "Capture" << std::endl;
        ChessPiece* target = getPieceAt(sf::Vector2i(endPos));
        return target && target->color != color;
    }
    return false;
}

bool ChessGame::isValidRookMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    if (startPos.x == endPos.x || startPos.y == endPos.y) {
        // Determine direction
        int stepX = (startPos.x == endPos.x) ? 0 : (endPos.x > startPos.x ? 1 : -1);
        int stepY = (startPos.y == endPos.y) ? 0 : (endPos.y > startPos.y ? 1 : -1);

        // Check each square between startPos and endPos
        sf::Vector2i currentPos = sf::Vector2i(startPos) + sf::Vector2i(stepX, stepY);
        while (currentPos != sf::Vector2i(endPos)) {
            if (getPieceAt(currentPos)) {
                return false; // Path is blocked
            }
            currentPos += sf::Vector2i(stepX, stepY);
        }
        return true;
    }
    return false;
}

bool ChessGame::isValidKnightMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    int dx = std::abs(startPos.x - endPos.x);
    int dy = std::abs(startPos.y - endPos.y);

    // Knight moves in an "L" shape: (2,1) or (1,2)
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool ChessGame::isValidBishopMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    int dx = std::abs(startPos.x - endPos.x);
    int dy = std::abs(startPos.y - endPos.y);

    if (dx == dy) {
        // Determine direction
        int stepX = (endPos.x > startPos.x) ? 1 : -1;
        int stepY = (endPos.y > startPos.y) ? 1 : -1;

        // Check each square between startPos and endPos
        sf::Vector2i currentPos = sf::Vector2i(startPos) + sf::Vector2i(stepX, stepY);
        while (currentPos != sf::Vector2i(endPos)) {
            if (getPieceAt(currentPos)) {
                std::cout << currentPos.x << ", " << currentPos.y << std::endl;
                return false; // Path is blocked
            }
            //std::cout << stepX << " moved x. move y: " << stepY << std::endl;
            currentPos += sf::Vector2i(stepX, stepY);
        }
        return true;
    }
    return false;
}

void ChessGame::trackMousePosition(sf::Vector2i mousePos) {
    sf::Vector2f worldPos = static_cast<sf::Vector2f>(mousePos);
    mousePos.x += boardOffset.x;
    mousePos.y += boardOffset.y / 2;
    sf::Vector2f boardPos = getBoardPosition(mousePos - static_cast<sf::Vector2i>(boardOffset));
    char file = 'a' + static_cast<int>(boardPos.x);
    int rank = 1 + static_cast<int>(boardPos.y);
    if (file >= 'a' && file <= 'h' && rank >= 1 && rank <= 8) {
        std::cout << "Mouse is hovering over: " << file << rank << std::endl;
    }
}

bool ChessGame::isValidQueenMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    // Queen moves horizontally, vertically, or diagonally
    if (startPos.x == endPos.x || startPos.y == endPos.y) {
        // Check if it's a valid rook move
        return isValidRookMove(startPos, endPos);
    }
    else if (std::abs(startPos.x - endPos.x) == std::abs(startPos.y - endPos.y)) {
        // Check if it's a valid bishop move
        return isValidBishopMove(startPos, endPos);
    }
    return false;
}

bool ChessGame::isValidKingMove(sf::Vector2f startPos, sf::Vector2f endPos) {
    int dx = std::abs(startPos.x - endPos.x);
    int dy = std::abs(startPos.y - endPos.y);

    // King moves one square in any direction
    return dx <= 1 && dy <= 1;
}
