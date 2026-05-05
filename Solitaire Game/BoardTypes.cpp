#include "BoardTypes.h"
#include <cmath>

// ============================================================
// EnglishBoard — classic cross, 7x7, orthogonal moves only
// ============================================================

EnglishBoard::EnglishBoard() { initialize(); }

void EnglishBoard::initialize() {
    rows_ = 7; cols_ = 7; pegCount_ = 0;
    // Cross shape: corners are invalid
    //   col: 0 1 2 3 4 5 6
    // row 0: . . P P P . .
    // row 1: . . P P P . .
    // row 2: P P P P P P P
    // row 3: P P P E P P P  (E = center hole)
    // row 4: P P P P P P P
    // row 5: . . P P P . .
    // row 6: . . P P P . .
    grid_.assign(7, std::vector<CellState>(7, INVALID));
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            bool corner = (r < 2 || r > 4) && (c < 2 || c > 4);
            if (!corner) {
                if (r == 3 && c == 3)
                    grid_[r][c] = EMPTY;   // center starts empty
                else {
                    grid_[r][c] = PEG;
                    pegCount_++;
                }
            }
        }
    }
    moveHistory_.clear();
}

// Override: English board only allows orthogonal moves
bool EnglishBoard::isValidMove(int fr, int fc, int tr, int tc) const {
    int dr = tr - fr, dc = tc - fc;
    // Reject diagonal
    if (dr != 0 && dc != 0) return false;
    return Board::isValidMove(fr, fc, tr, tc);
}

// ============================================================
// HexagonBoard — compact hexagonal layout on a 9x9 grid
// Allows orthogonal + diagonal moves
// ============================================================

HexagonBoard::HexagonBoard() { initialize(); }

void HexagonBoard::initialize() {
    // We model a hexagon as a diamond on a 9x5 grid
    // Rows of lengths: 3, 4, 5, 4, 3 (centered)
    rows_ = 5; cols_ = 9; pegCount_ = 0;
    grid_.assign(rows_, std::vector<CellState>(cols_, INVALID));

    // Row 0: cols 3,4,5
    // Row 1: cols 2,3,4,5
    // Row 2: cols 1,2,3,4,5,6,7  (center row, hole at col 4)
    // Row 3: cols 2,3,4,5
    // Row 4: cols 3,4,5
    int starts[] = {3, 2, 1, 2, 3};
    int ends[]   = {5, 5, 7, 5, 5};

    for (int r = 0; r < rows_; r++) {
        for (int c = starts[r]; c <= ends[r]; c++) {
            if (r == 2 && c == 4) {
                grid_[r][c] = EMPTY;  // center hole
            } else {
                grid_[r][c] = PEG;
                pegCount_++;
            }
        }
    }
    moveHistory_.clear();
}

// ============================================================
// DiamondBoard — rotated square, orthogonal moves only
// ============================================================

DiamondBoard::DiamondBoard() { initialize(); }

void DiamondBoard::initialize() {
    // Diamond of radius 4 centered in a 9x9 grid
    // A cell (r,c) is valid if |r-4| + |c-4| <= 4
    rows_ = 9; cols_ = 9; pegCount_ = 0;
    grid_.assign(9, std::vector<CellState>(9, INVALID));

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (std::abs(r - 4) + std::abs(c - 4) <= 4) {
                if (r == 4 && c == 4) {
                    grid_[r][c] = EMPTY;  // center hole
                } else {
                    grid_[r][c] = PEG;
                    pegCount_++;
                }
            }
        }
    }
    moveHistory_.clear();
}

// Override: Diamond board only allows orthogonal moves
bool DiamondBoard::isValidMove(int fr, int fc, int tr, int tc) const {
    int dr = tr - fr, dc = tc - fc;
    if (dr != 0 && dc != 0) return false;
    return Board::isValidMove(fr, fc, tr, tc);
}
