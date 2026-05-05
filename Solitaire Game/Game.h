#pragma once
#include "Board.h"
#include <string>
#include <memory>

/**
 * Abstract Game base class.
 * Owns a Board and defines the interface for both game modes.
 */
class Game {
public:
    explicit Game(std::unique_ptr<Board> board);
    virtual ~Game() = default;

    virtual std::string gameMode() const = 0;
    virtual bool playTurn() = 0;   // Execute one turn; returns false if no move possible

    Board* getBoard() const { return board_.get(); }
    bool isOver() const;
    std::string getStatus() const;  // Current state summary

protected:
    std::unique_ptr<Board> board_;
};

/**
 * ManualGame — a human selects moves interactively.
 */
class ManualGame : public Game {
public:
    explicit ManualGame(std::unique_ptr<Board> board);
    std::string gameMode() const override { return "Manual"; }

    // Returns true if move was accepted
    bool playTurn() override;
    bool makeMove(int fr, int fc, int tr, int tc);
    void randomizeState();   // Make one random move to randomize board
};

/**
 * AutoplayGame — computer picks random valid moves automatically.
 */
class AutoplayGame : public Game {
public:
    explicit AutoplayGame(std::unique_ptr<Board> board);
    std::string gameMode() const override { return "Autoplay"; }

    bool playTurn() override;   // Execute one random move
    void playAll();             // Run until game over
};
