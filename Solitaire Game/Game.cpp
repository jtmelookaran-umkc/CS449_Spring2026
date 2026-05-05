#include "Game.h"

// ============================================================
// Game base
// ============================================================

Game::Game(std::unique_ptr<Board> board) : board_(std::move(board)) {}

bool Game::isOver() const {
    return board_->isGameOver();
}

std::string Game::getStatus() const {
    if (!isOver())
        return "Game in progress. Pegs remaining: " + std::to_string(board_->pegCount());
    return "Game over! Pegs remaining: " + std::to_string(board_->pegCount())
         + " — Rating: " + board_->getRating();
}

// ============================================================
// ManualGame
// ============================================================

ManualGame::ManualGame(std::unique_ptr<Board> board)
    : Game(std::move(board)) {}

bool ManualGame::playTurn() {
    // In manual mode, the UI drives moves; this checks if a move is possible
    return !board_->isGameOver();
}

bool ManualGame::makeMove(int fr, int fc, int tr, int tc) {
    return board_->makeMove(fr, fc, tr, tc);
}

void ManualGame::randomizeState() {
    board_->randomMove();
}

// ============================================================
// AutoplayGame
// ============================================================

AutoplayGame::AutoplayGame(std::unique_ptr<Board> board)
    : Game(std::move(board)) {}

bool AutoplayGame::playTurn() {
    if (board_->isGameOver()) return false;
    board_->randomMove();
    return true;
}

void AutoplayGame::playAll() {
    while (!board_->isGameOver()) {
        board_->randomMove();
    }
}
