#include <iostream>
#include <memory>
#include <string>
#include "src/BoardTypes.h"
#include "src/Game.h"
#include "src/GameRecorder.h"
#include "src/ConsoleUI.h"

// ---------------------------------------------------------------
// Factory helpers
// ---------------------------------------------------------------

std::unique_ptr<Board> makeBoard(const std::string& type) {
    return GameRecorder::createBoard(type);
}

std::unique_ptr<Game> makeGame(const std::string& mode, std::unique_ptr<Board> board) {
    if (mode == "Autoplay")
        return std::make_unique<AutoplayGame>(std::move(board));
    return std::make_unique<ManualGame>(std::move(board));
}

// ---------------------------------------------------------------
// Manual game loop
// ---------------------------------------------------------------

void runManualGame(ManualGame& game, bool record) {
    Board* board = game.getBoard();
    ConsoleUI::printBoard(*board);
    ConsoleUI::printValidMoves(*board);

    while (!game.isOver()) {
        std::cout << "\nOptions:\n"
                  << "  m) Make a move\n"
                  << "  r) Randomize board state\n"
                  << "  v) Show valid moves\n"
                  << "  q) Quit\n"
                  << "Choice: ";
        char choice;
        std::cin >> choice;

        if (choice == 'q') break;

        if (choice == 'r') {
            game.randomizeState();
            std::cout << "Board randomized (one random move made).\n";
            ConsoleUI::printBoard(*board);
            continue;
        }

        if (choice == 'v') {
            ConsoleUI::printValidMoves(*board);
            continue;
        }

        if (choice == 'm') {
            int fr, fc, tr, tc;
            if (!ConsoleUI::promptMove(fr, fc, tr, tc)) continue;
            if (game.makeMove(fr, fc, tr, tc)) {
                std::cout << "Move accepted.\n";
                ConsoleUI::printBoard(*board);
                ConsoleUI::printValidMoves(*board);
            } else {
                std::cout << "Invalid move. Try again.\n";
            }
        }
    }

    ConsoleUI::printGameOver(*board);

    if (record && !board->moveHistory().empty()) {
        std::string filename = ConsoleUI::promptFilename("save game record to");
        try {
            GameRecorder::save(*board, "records/" + filename);
            std::cout << "Game saved to records/" << filename << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Save failed: " << e.what() << "\n";
        }
    }
}

// ---------------------------------------------------------------
// Autoplay game loop
// ---------------------------------------------------------------

void runAutoplayGame(AutoplayGame& game) {
    Board* board = game.getBoard();
    std::cout << "Running autoplay...\n\n";

    int step = 0;
    while (!game.isOver()) {
        game.playTurn();
        step++;
        // Print every 5 steps to avoid too much output
        if (step % 5 == 0 || board->isGameOver()) {
            std::cout << "[Step " << step << "] ";
            ConsoleUI::printBoard(*board);
        }
    }
    ConsoleUI::printGameOver(*board);
}

// ---------------------------------------------------------------
// Replay a saved game
// ---------------------------------------------------------------

void runReplay(const std::string& filename) {
    try {
        auto [boardType, moves] = GameRecorder::load("records/" + filename);
        auto board = makeBoard(boardType);

        ConsoleUI::printHeader("REPLAY: " + boardType + " board — " +
                               std::to_string(moves.size()) + " moves");
        ConsoleUI::printBoard(*board);

        for (int i = 0; i < (int)moves.size(); i++) {
            std::cout << "\nPress Enter for move " << (i + 1) << "/" << moves.size() << "...";
            std::cin.ignore();
            std::cin.get();

            Move m = moves[i];
            board->makeMove(m.fromRow, m.fromCol, m.toRow, m.toCol);
            ConsoleUI::printMove(m);
            ConsoleUI::printBoard(*board);
        }
        ConsoleUI::printGameOver(*board);
    } catch (const std::exception& e) {
        std::cerr << "Replay error: " << e.what() << "\n";
    }
}

// ---------------------------------------------------------------
// Main menu
// ---------------------------------------------------------------

int main() {
    ConsoleUI::printHeader("PEG SOLITAIRE — CS 449 Sprint 3");

    while (true) {
        std::cout << "\nMAIN MENU\n"
                  << "  1) New Manual Game\n"
                  << "  2) New Autoplay Game\n"
                  << "  3) Replay Saved Game\n"
                  << "  4) Exit\n"
                  << "Choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 4) break;

        if (choice == 3) {
            std::string filename = ConsoleUI::promptFilename("load replay from");
            runReplay(filename);
            continue;
        }

        std::string boardType = ConsoleUI::promptBoardType();
        auto board = makeBoard(boardType);

        ConsoleUI::printHeader(boardType + " Board — " +
                               std::to_string(board->pegCount()) + " pegs");

        if (choice == 1) {
            ManualGame game(std::move(board));
            std::cout << "Record this game? (y/n): ";
            char rec; std::cin >> rec;
            runManualGame(game, rec == 'y' || rec == 'Y');
        } else {
            AutoplayGame game(std::move(board));
            runAutoplayGame(game);
        }
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
