#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <cmath>

// ── Color palette ────────────────────────────────────────────────
const QColor BoardWidget::COL_BG           ("#1e1e2e");
const QColor BoardWidget::COL_BOARD        ("#313244");
const QColor BoardWidget::COL_PEG          ("#89b4fa");  // blue marble
const QColor BoardWidget::COL_PEG_SELECTED ("#f38ba8");  // red highlight
const QColor BoardWidget::COL_PEG_OUTLINE  ("#cdd6f4");
const QColor BoardWidget::COL_HOLE         ("#45475a");  // dark empty hole
const QColor BoardWidget::COL_TARGET       ("#a6e3a1");  // green valid target
const QColor BoardWidget::COL_INVALID      ("#1e1e2e");  // same as background

// ────────────────────────────────────────────────────────────────

BoardWidget::BoardWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(sizeHint());
}

void BoardWidget::setBoard(Board* board) {
    board_ = board;
    clearSelection();
    update();
}

void BoardWidget::clearSelection() {
    selRow_ = selCol_ = -1;
    validTargets_.clear();
    update();
}

QSize BoardWidget::sizeHint() const {
    int cols = board_ ? board_->cols() : 9;
    int rows = board_ ? board_->rows() : 9;
    return QSize(cols * CELL + 2 * PAD, rows * CELL + 2 * PAD);
}

// ── Coordinate helpers ───────────────────────────────────────────

QPoint BoardWidget::cellCenter(int row, int col) const {
    return QPoint(PAD + col * CELL + CELL / 2,
                  PAD + row * CELL + CELL / 2);
}

std::pair<int,int> BoardWidget::cellAt(QPoint pos) const {
    int col = (pos.x() - PAD) / CELL;
    int row = (pos.y() - PAD) / CELL;
    return {row, col};
}

bool BoardWidget::isValidTarget(int r, int c) const {
    for (auto& [vr, vc] : validTargets_)
        if (vr == r && vc == c) return true;
    return false;
}

// ── Paint ────────────────────────────────────────────────────────

void BoardWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    p.fillRect(rect(), COL_BG);

    if (!board_) return;

    int rows = board_->rows(), cols = board_->cols();

    // Board background panel
    QRect panel(PAD - 8, PAD - 8,
                cols * CELL + 16, rows * CELL + 16);
    p.setBrush(COL_BOARD);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(panel, 12, 12);

    // Draw cells
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            CellState state = board_->getCell(r, c);
            QPoint center   = cellCenter(r, c);

            if (state == INVALID) continue;

            bool isSelected = (r == selRow_ && c == selCol_);
            bool isTarget   = isValidTarget(r, c);

            if (state == PEG) {
                // Shadow
                p.setPen(Qt::NoPen);
                p.setBrush(QColor(0, 0, 0, 60));
                p.drawEllipse(center + QPoint(2, 3), PEG_R, PEG_R);

                // Marble gradient
                QRadialGradient grad(center - QPoint(6, 6), PEG_R * 1.4);
                if (isSelected) {
                    grad.setColorAt(0, COL_PEG_SELECTED.lighter(140));
                    grad.setColorAt(1, COL_PEG_SELECTED.darker(140));
                } else {
                    grad.setColorAt(0, COL_PEG.lighter(130));
                    grad.setColorAt(1, COL_PEG.darker(160));
                }
                p.setBrush(grad);
                p.setPen(QPen(isSelected ? COL_PEG_SELECTED.darker(110)
                                         : COL_PEG_OUTLINE, 1.5));
                p.drawEllipse(center, PEG_R, PEG_R);

                // Shine
                QRadialGradient shine(center - QPoint(8, 9), PEG_R * 0.5);
                shine.setColorAt(0, QColor(255, 255, 255, 90));
                shine.setColorAt(1, QColor(255, 255, 255, 0));
                p.setPen(Qt::NoPen);
                p.setBrush(shine);
                p.drawEllipse(center, PEG_R, PEG_R);

            } else { // EMPTY
                // Subtle ring for hole
                QColor holeColor = isTarget ? COL_TARGET : COL_HOLE;

                p.setPen(Qt::NoPen);
                p.setBrush(holeColor.darker(150));
                p.drawEllipse(center, HOLE_R + 2, HOLE_R + 2);

                p.setBrush(holeColor);
                p.drawEllipse(center, HOLE_R, HOLE_R);

                if (isTarget) {
                    // Pulsing ring around valid target
                    p.setPen(QPen(COL_TARGET, 2));
                    p.setBrush(Qt::NoBrush);
                    p.drawEllipse(center, HOLE_R + 6, HOLE_R + 6);
                }
            }
        }
    }
}

// ── Mouse ────────────────────────────────────────────────────────

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    if (!board_) return;

    auto [row, col] = cellAt(event->pos());
    if (row < 0 || row >= board_->rows() || col < 0 || col >= board_->cols()) return;

    CellState state = board_->getCell(row, col);
    if (state == INVALID) return;

    // Clicking a valid target => emit move
    if (selRow_ >= 0 && isValidTarget(row, col)) {
        emit moveRequested(selRow_, selCol_, row, col);
        clearSelection();
        return;
    }

    // Clicking a peg => select it and compute targets
    if (state == PEG) {
        selRow_ = row;
        selCol_ = col;
        validTargets_.clear();
        for (auto& m : board_->getValidMoves())
            if (m.fromRow == row && m.fromCol == col)
                validTargets_.push_back({m.toRow, m.toCol});
        update();
        return;
    }

    // Clicking elsewhere => deselect
    clearSelection();
}
