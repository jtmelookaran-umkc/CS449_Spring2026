#pragma once
#include <vector>
#include <tuple>
#include <string>

// Cell states
enum CellState { INVALID = -1, EMPTY = 0, PEG = 1 };

struct Move {
    int fromRow, fromCol, toRow, toCol;
};

/**
 * Abstract class for multiple board types
 */
class Board {
public:
    Board();
    virtual ~Board() = default;

    // --- Pure virtual interface (subclasses must implement) ---
    virtual void initialize() = 0;          // Set up grid shape and pegs
    virtual std::string boardType() const = 0;  // "English", "Hexagon", "Diamond"

    // --- Common queries ---
    CellState getCell(int row, int col) const;
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int pegCount() const { return pegCount_; }
    const std::vector<std::vector<CellState>>& grid() const { return grid_; }
    const std::vector<Move>& moveHistory() const { return moveHistory_; }

    // --- Move logic ---
    bool isValidMove(int fr, int fc, int tr, int tc) const;
    bool makeMove(int fr, int fc, int tr, int tc);
    std::vector<Move> getValidMoves() const;
    bool isGameOver() const;

    // --- Gameplay helpers ---
    Move randomMove();          // Returns a random valid move and executes it
    std::string getRating() const;
    void reset();

protected:
    std::vector<std::vector<CellState>> grid_;
    int rows_, cols_, pegCount_;
    std::vector<Move> moveHistory_;
};
