#pragma once
#include "Board.h"
#include "BoardTypes.h"
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

/**
 * GameRecorder — saves a completed game to a text file and
 * can replay it move-by-move or all at once.
 *
 * File format:
 *   BOARD_TYPE <English|Hexagon|Diamond>
 *   MOVE <fromRow> <fromCol> <toRow> <toCol>
 *   ...
 */
class GameRecorder {
public:
    // Save the board's full move history to a file
    static void save(const Board& board, const std::string& filename) {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing: " + filename);
        out << "BOARD_TYPE " << board.boardType() << "\n";
        for (const auto& m : board.moveHistory())
            out << "MOVE " << m.fromRow << " " << m.fromCol
                << " " << m.toRow   << " " << m.toCol << "\n";
    }

    // Load moves from a file; returns board type string and move list
    static std::pair<std::string, std::vector<Move>>
    load(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Cannot open file for reading: " + filename);

        std::string boardType;
        std::string token;
        in >> token >> boardType;   // "BOARD_TYPE <type>"

        std::vector<Move> moves;
        while (in >> token) {       // "MOVE fr fc tr tc"
            if (token != "MOVE") throw std::runtime_error("Unexpected token: " + token);
            Move m;
            in >> m.fromRow >> m.fromCol >> m.toRow >> m.toCol;
            moves.push_back(m);
        }
        return {boardType, moves};
    }

    // Replay all recorded moves onto a fresh board; returns the board
    static std::unique_ptr<Board> replay(const std::string& filename) {
        auto [boardType, moves] = load(filename);
        auto board = createBoard(boardType);
        for (const auto& m : moves)
            board->makeMove(m.fromRow, m.fromCol, m.toRow, m.toCol);
        return board;
    }

    // Create a board from a type name string
    static std::unique_ptr<Board> createBoard(const std::string& type) {
        if (type == "English")  return std::make_unique<EnglishBoard>();
        if (type == "Hexagon")  return std::make_unique<HexagonBoard>();
        if (type == "Diamond")  return std::make_unique<DiamondBoard>();
        throw std::runtime_error("Unknown board type: " + type);
    }
};
