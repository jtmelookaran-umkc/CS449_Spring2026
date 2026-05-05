#pragma once
#include "Board.h"
#include <iostream>
#include <string>

/**
 * ConsoleUI — separated UI layer (not mixed into game logic).
 * Handles all display and input parsing.
 */
class ConsoleUI {
public:
    // Print the board grid with row/col labels
    static void printBoard(const Board& board) {
        const auto& grid = board.grid();
        int rows = board.rows(), cols = board.cols();

        // Column header
        std::cout << "   ";
        for (int c = 0; c < cols; c++) std::cout << c << " ";
        std::cout << "\n";
        std::cout << "   ";
        for (int c = 0; c < cols; c++) std::cout << "--";
        std::cout << "\n";

        for (int r = 0; r < rows; r++) {
            std::cout << r << " |";
            for (int c = 0; c < cols; c++) {
                CellState cell = grid[r][c];
                if (cell == INVALID) std::cout << "  ";
                else if (cell == PEG) std::cout << "O ";
                else                  std::cout << ". ";
            }
            std::cout << "\n";
        }
        std::cout << "\nPegs remaining: " << board.pegCount() << "\n";
    }

    // Print a move record
    static void printMove(const Move& m) {
        std::cout << "  Move: (" << m.fromRow << "," << m.fromCol << ")"
                  << " -> (" << m.toRow << "," << m.toCol << ")\n";
    }

    // Print all valid moves available
    static void printValidMoves(const Board& board) {
        auto moves = board.getValidMoves();
        if (moves.empty()) { std::cout << "No valid moves available.\n"; return; }
        std::cout << "Valid moves (" << moves.size() << "):\n";
        for (auto& m : moves) {
            std::cout << "  (" << m.fromRow << "," << m.fromCol << ")"
                      << " -> (" << m.toRow << "," << m.toCol << ")\n";
        }
    }

    // Print game-over summary
    static void printGameOver(const Board& board) {
        std::cout << "\n========================================\n";
        std::cout << "           GAME OVER\n";
        std::cout << "========================================\n";
        std::cout << "Pegs remaining: " << board.pegCount() << "\n";
        std::cout << "Rating:         " << board.getRating() << "\n";
        std::cout << "========================================\n\n";
    }

    // Print a section divider with a label
    static void printHeader(const std::string& title) {
        std::cout << "\n======== " << title << " ========\n";
    }

    // Prompt user for board type selection; returns "English", "Hexagon", or "Diamond"
    static std::string promptBoardType() {
        std::cout << "\nSelect board type:\n";
        std::cout << "  1) English\n";
        std::cout << "  2) Hexagon\n";
        std::cout << "  3) Diamond\n";
        std::cout << "Choice (1-3): ";
        int choice = 0;
        std::cin >> choice;
        if (choice == 2) return "Hexagon";
        if (choice == 3) return "Diamond";
        return "English";
    }

    // Prompt user for game mode; returns "Manual" or "Autoplay"
    static std::string promptGameMode() {
        std::cout << "\nSelect game mode:\n";
        std::cout << "  1) Manual\n";
        std::cout << "  2) Autoplay\n";
        std::cout << "Choice (1-2): ";
        int choice = 0;
        std::cin >> choice;
        return (choice == 2) ? "Autoplay" : "Manual";
    }

    // Prompt for a move; returns false if user wants to quit/menu
    static bool promptMove(int& fr, int& fc, int& tr, int& tc) {
        std::cout << "\nEnter move (fromRow fromCol toRow toCol), or -1 to cancel: ";
        std::cin >> fr;
        if (fr == -1) return false;
        std::cin >> fc >> tr >> tc;
        return true;
    }

    // Prompt for a filename
    static std::string promptFilename(const std::string& action) {
        std::cout << "Enter filename to " << action << " (e.g., game.txt): ";
        std::string name;
        std::cin >> name;
        return name;
    }
};
