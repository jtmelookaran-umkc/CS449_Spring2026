#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>

Board::Board() : rows_(0), cols_(0), pegCount_(0) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

CellState Board::getCell(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        return INVALID;
    return grid_[row][col];
}

bool Board::isValidMove(int fr, int fc, int tr, int tc) const {
    // Source must have a peg
    if (getCell(fr, fc) != PEG) return false;
    // Destination must be empty
    if (getCell(tr, tc) != EMPTY) return false;

    int dr = tr - fr, dc = tc - fc;
    // Must be exactly 2 steps orthogonally (diagonal handled by subclasses)
    if (abs(dr) > 2 || abs(dc) > 2) return false;
    if (abs(dr) == 0 && abs(dc) == 0) return false;
    if (abs(dr) != 0 && abs(dc) != 0 && abs(dr) != abs(dc)) return false;
    if (abs(dr) != 2 && abs(dc) != 2) return false;

    // Middle cell must have a peg
    int mr = fr + dr / 2, mc = fc + dc / 2;
    return getCell(mr, mc) == PEG;
}

bool Board::makeMove(int fr, int fc, int tr, int tc) {
    if (!isValidMove(fr, fc, tr, tc)) return false;
    int mr = fr + (tr - fr) / 2;
    int mc = fc + (tc - fc) / 2;
    grid_[fr][fc] = EMPTY;
    grid_[mr][mc] = EMPTY;
    grid_[tr][tc] = PEG;
    pegCount_--;
    moveHistory_.push_back({fr, fc, tr, tc});
    return true;
}

std::vector<Move> Board::getValidMoves() const {
    std::vector<Move> moves;
    int deltas[5] = {-2, -1, 0, 1, 2};
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if (grid_[r][c] != PEG) continue;
            for (int dr : deltas) {
                for (int dc : deltas) {
                    if (dr == 0 && dc == 0) continue;
                    if (isValidMove(r, c, r + dr, c + dc))
                        moves.push_back({r, c, r + dr, c + dc});
                }
            }
        }
    }
    return moves;
}

bool Board::isGameOver() const {
    return getValidMoves().empty();
}

Move Board::randomMove() {
    auto moves = getValidMoves();
    if (moves.empty()) return {-1, -1, -1, -1};
    Move m = moves[std::rand() % moves.size()];
    makeMove(m.fromRow, m.fromCol, m.toRow, m.toCol);
    return m;
}

std::string Board::getRating() const {
    if (pegCount_ == 1) return "Outstanding";
    if (pegCount_ == 2) return "Very Good";
    if (pegCount_ == 3) return "Good";
    return "Average";
}

void Board::reset() {
    moveHistory_.clear();
    initialize();
}
