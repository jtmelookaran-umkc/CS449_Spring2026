#pragma once
#include <QWidget>
#include <QPoint>
#include "Board.h"

/**
 * BoardWidget : creates solitare board and handles input
 *
 *
 * Responsibilities: handles game logic and user decisions
 *
 */
class BoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit BoardWidget(QWidget* parent = nullptr);

    void setBoard(Board* board);   // Give widget a pointer to the current board
    void clearSelection();         // Deselect any highlighted peg

    QSize sizeHint() const override;

signals:
    void moveRequested(int fromRow, int fromCol, int toRow, int toCol);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    Board* board_ = nullptr;

    // Currently selected cell (-1 = none)
    int selRow_ = -1, selCol_ = -1;

    // Valid landing cells for the selected peg
    std::vector<std::pair<int,int>> validTargets_;

    // Layout helpers
    static constexpr int CELL  = 62;   // pixels per cell
    static constexpr int PAD   = 20;   // border padding
    static constexpr int PEG_R = 24;   // peg circle radius
    static constexpr int HOLE_R= 10;   // empty hole radius

    QPoint cellCenter(int row, int col) const;
    std::pair<int,int> cellAt(QPoint pos) const;
    bool isValidTarget(int r, int c) const;

    // Color palette
    static const QColor COL_BG;
    static const QColor COL_BOARD;
    static const QColor COL_PEG;
    static const QColor COL_PEG_SELECTED;
    static const QColor COL_PEG_OUTLINE;
    static const QColor COL_HOLE;
    static const QColor COL_TARGET;
    static const QColor COL_INVALID;
};
