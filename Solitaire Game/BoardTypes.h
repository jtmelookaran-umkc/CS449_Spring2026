#pragma once
#include "Board.h"

/**
 * English board only allows certain moves to be made
 */
class EnglishBoard : public Board {
public:
    EnglishBoard();
    void initialize() override;
    std::string boardType() const override { return "English"; }
    bool isValidMove(int fr, int fc, int tr, int tc) const;
};

/**
 * Hexagon board — diamond/rhombus shape played on a square grid.
 * Allows orthogonal and diagonal moves.
 */
class HexagonBoard : public Board {
public:
    HexagonBoard();
    void initialize() override;
    std::string boardType() const override { return "Hexagon"; }
};

/**
 * Diamond board — rotated square shape.
 * Only orthogonal moves allowed.
 */
class DiamondBoard : public Board {
public:
    DiamondBoard();
    void initialize() override;
    std::string boardType() const override { return "Diamond"; }
    bool isValidMove(int fr, int fc, int tr, int tc) const;
};
