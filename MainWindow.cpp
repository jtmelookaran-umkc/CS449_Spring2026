#include "MainWindow.h"
#include "BoardTypes.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QSizePolicy>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Peg Solitaire - CS 449");
    setMinimumSize(900, 640);

    autoplayTimer_ = new QTimer(this);
    autoplayTimer_->setInterval(300);
    connect(autoplayTimer_, &QTimer::timeout, this, &MainWindow::onAutoplayStep);

    buildUI();
    applyStyleSheet();
    startNewGame();
}

void MainWindow::buildUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout* root = new QHBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(16);

    // Left panel
    QGroupBox* leftPanel = new QGroupBox("Board Type");
    leftPanel->setFixedWidth(150);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(10);

    englishRadio_ = new QRadioButton("English");
    hexagonRadio_ = new QRadioButton("Hexagon");
    diamondRadio_ = new QRadioButton("Diamond");
    englishRadio_->setChecked(true);

    boardTypeGroup_ = new QButtonGroup(this);
    boardTypeGroup_->addButton(englishRadio_);
    boardTypeGroup_->addButton(hexagonRadio_);
    boardTypeGroup_->addButton(diamondRadio_);

    recordCheck_ = new QCheckBox("Record game");

    leftLayout->addWidget(englishRadio_);
    leftLayout->addWidget(hexagonRadio_);
    leftLayout->addWidget(diamondRadio_);
    leftLayout->addSpacing(12);
    leftLayout->addWidget(recordCheck_);
    leftLayout->addStretch();

    // Center board
    boardWidget_ = new BoardWidget();
    connect(boardWidget_, &BoardWidget::moveRequested,
            this,         &MainWindow::onMoveRequested);

    QScrollArea* scroll = new QScrollArea;
    scroll->setWidget(boardWidget_);
    scroll->setAlignment(Qt::AlignCenter);
    scroll->setWidgetResizable(false);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scroll->setFrameShape(QFrame::NoFrame);

    // Right panel
    QWidget* rightPanel = new QWidget;
    rightPanel->setFixedWidth(160);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(10);

    QGroupBox* sizeBox = new QGroupBox("Board size");
    QVBoxLayout* sizeLayout = new QVBoxLayout(sizeBox);
    boardSizeBox_ = new QComboBox;
    boardSizeBox_->addItems({"5","7","9"});
    boardSizeBox_->setCurrentText("7");
    sizeLayout->addWidget(boardSizeBox_);

    newGameBtn_   = new QPushButton("New Game");
    replayBtn_    = new QPushButton("Replay");
    autoplayBtn_  = new QPushButton("Autoplay");
    randomizeBtn_ = new QPushButton("Randomize");

    autoplayBtn_->setObjectName("autoplayBtn");
    randomizeBtn_->setObjectName("randomizeBtn");

    connect(newGameBtn_,   &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(autoplayBtn_,  &QPushButton::clicked, this, &MainWindow::onAutoplayToggle);
    connect(randomizeBtn_, &QPushButton::clicked, this, &MainWindow::onRandomize);
    connect(replayBtn_,    &QPushButton::clicked, this, &MainWindow::onReplay);

    QGroupBox* statusBox = new QGroupBox("Status");
    QVBoxLayout* statusLayout = new QVBoxLayout(statusBox);
    pegCountLabel_ = new QLabel("Pegs: -");
    ratingLabel_   = new QLabel("");
    statusLabel_   = new QLabel("New game ready.");
    statusLabel_->setWordWrap(true);
    statusLayout->addWidget(pegCountLabel_);
    statusLayout->addWidget(ratingLabel_);
    statusLayout->addWidget(statusLabel_);

    rightLayout->addWidget(sizeBox);
    rightLayout->addWidget(newGameBtn_);
    rightLayout->addWidget(replayBtn_);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(autoplayBtn_);
    rightLayout->addWidget(randomizeBtn_);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(statusBox);
    rightLayout->addStretch();

    root->addWidget(leftPanel);
    root->addWidget(scroll, 1);
    root->addWidget(rightPanel);
}

void MainWindow::applyStyleSheet() {
    setStyleSheet(
        "QMainWindow, QWidget {"
        "  background-color: #1e1e2e;"
        "  color: #cdd6f4;"
        "  font-family: 'Segoe UI', Arial, sans-serif;"
        "  font-size: 13px;"
        "}"
        "QGroupBox {"
        "  border: 1px solid #45475a;"
        "  border-radius: 8px;"
        "  margin-top: 10px;"
        "  padding-top: 8px;"
        "  font-weight: bold;"
        "  color: #89b4fa;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  subcontrol-position: top left;"
        "  padding: 0 6px;"
        "}"
        "QPushButton {"
        "  background-color: #313244;"
        "  color: #cdd6f4;"
        "  border: 1px solid #45475a;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover  { background-color: #45475a; }"
        "QPushButton:pressed{ background-color: #585b70; }"
        "QPushButton#autoplayBtn  { background-color: #40a02b; color: #fff; border: none; }"
        "QPushButton#autoplayBtn:hover { background-color: #37901f; }"
        "QPushButton#randomizeBtn { background-color: #fe640b; color: #fff; border: none; }"
        "QPushButton#randomizeBtn:hover { background-color: #e05200; }"
        "QRadioButton, QCheckBox { color: #cdd6f4; spacing: 6px; }"
        "QComboBox {"
        "  background-color: #313244;"
        "  color: #cdd6f4;"
        "  border: 1px solid #45475a;"
        "  border-radius: 4px;"
        "  padding: 4px 8px;"
        "}"
        "QScrollArea { background: transparent; border: none; }"
        "QLabel { color: #cdd6f4; }"
    );
}

std::string MainWindow::selectedBoardType() const {
    if (hexagonRadio_->isChecked()) return "Hexagon";
    if (diamondRadio_->isChecked()) return "Diamond";
    return "English";
}

void MainWindow::startNewGame() {
    autoplayRunning_ = false;
    autoplayTimer_->stop();
    autoplayBtn_->setText("Autoplay");
    autoplayBtn_->setProperty("running", false);
    autoplayBtn_->style()->unpolish(autoplayBtn_);
    autoplayBtn_->style()->polish(autoplayBtn_);

    autoGame_.reset();

    std::string type = selectedBoardType();
    auto board = GameRecorder::createBoard(type);
    manualGame_ = std::make_unique<ManualGame>(std::move(board));

    boardWidget_->setBoard(manualGame_->getBoard());
    boardWidget_->resize(boardWidget_->sizeHint());
    setControlsEnabled(true);
    updateStatus();
}

void MainWindow::updateStatus() {
    Board* board = manualGame_ ? manualGame_->getBoard()
                 : autoGame_  ? autoGame_->getBoard()
                 : nullptr;
    if (!board) return;

    pegCountLabel_->setText(QString("Pegs: %1").arg(board->pegCount()));

    if (board->isGameOver()) {
        QString rating   = QString::fromStdString(board->getRating());
        int     pegsLeft = board->pegCount();

        ratingLabel_->setText(QString("Rating: %1").arg(rating));
        statusLabel_->setText("Game over!");
        setControlsEnabled(false);
        newGameBtn_->setEnabled(true);
        replayBtn_->setEnabled(true);

        // ── Step 1: Save FIRST, before any dialog that could reset state ──
        if (recordCheck_->isChecked() && !board->moveHistory().empty()) {
            QString savePath = QFileDialog::getSaveFileName(
                this, "Save Game Record", "records/game.txt",
                "Text files (*.txt)");
            if (!savePath.isEmpty()) {
                try {
                    GameRecorder::save(*board, savePath.toStdString());
                    statusLabel_->setText("Saved: " + savePath.section('/', -1));
                } catch (...) {
                    QMessageBox::warning(this, "Save Failed",
                                         "Could not write the record file.");
                }
            }
        }

        // ── Step 2: Show game-over popup AFTER save is done ──
        QString msg = QString(
            "<h2 style='margin:0 0 8px 0'>Game Over</h2>"
            "<p style='font-size:15px'>Rating: <b>%1</b></p>"
            "<p style='font-size:14px'>Pegs remaining: <b>%2</b></p>"
            "<hr/>"
            "<p style='font-size:12px;color:#aaa'>"
            "Outstanding = 1 peg | Very Good = 2 | Good = 3 | Average = 4+</p>"
        ).arg(rating).arg(pegsLeft);

        QMessageBox resultBox(this);
        resultBox.setWindowTitle("Game Over");
        resultBox.setText(msg);
        resultBox.setIcon(QMessageBox::Information);
        QPushButton* newBtn = resultBox.addButton("New Game", QMessageBox::AcceptRole);
        QPushButton* repBtn = resultBox.addButton("Replay",   QMessageBox::RejectRole);
        resultBox.addButton("Close", QMessageBox::DestructiveRole);
        resultBox.exec();

        // ── Step 3: Act on the button chosen ──
        if (resultBox.clickedButton() == newBtn)      onNewGame();
        else if (resultBox.clickedButton() == repBtn) onReplay();

    } else {
        ratingLabel_->setText("");
        statusLabel_->setText(QString("%1 board\n%2 mode")
            .arg(QString::fromStdString(board->boardType()))
            .arg(manualGame_ ? "Manual" : "Autoplay"));
    }
}

void MainWindow::setControlsEnabled(bool enabled) {
    englishRadio_->setEnabled(enabled);
    hexagonRadio_->setEnabled(enabled);
    diamondRadio_->setEnabled(enabled);
    boardSizeBox_->setEnabled(enabled);
    autoplayBtn_->setEnabled(enabled);
    randomizeBtn_->setEnabled(enabled);
    replayBtn_->setEnabled(enabled);
}

void MainWindow::onNewGame() {
    startNewGame();
}

void MainWindow::onMoveRequested(int fr, int fc, int tr, int tc) {
    if (!manualGame_ || manualGame_->getBoard()->isGameOver()) return;
    bool ok = manualGame_->makeMove(fr, fc, tr, tc);
    if (ok) {
        boardWidget_->clearSelection();
        boardWidget_->update();
        updateStatus();
    }
}

void MainWindow::onAutoplayToggle() {
    if (!manualGame_) return;

    if (!autoplayRunning_) {
        autoGame_ = std::make_unique<AutoplayGame>(
            GameRecorder::createBoard(
                manualGame_->getBoard()->boardType()));

        for (auto& m : manualGame_->getBoard()->moveHistory())
            autoGame_->getBoard()->makeMove(m.fromRow, m.fromCol,
                                            m.toRow,   m.toCol);

        manualGame_.reset();
        boardWidget_->setBoard(autoGame_->getBoard());

        autoplayRunning_ = true;
        autoplayBtn_->setText("Stop");
        autoplayBtn_->setProperty("running", true);
        autoplayBtn_->style()->unpolish(autoplayBtn_);
        autoplayBtn_->style()->polish(autoplayBtn_);
        autoplayTimer_->start();
    } else {
        autoplayRunning_ = false;
        autoplayTimer_->stop();
        autoplayBtn_->setText("Autoplay");
        autoplayBtn_->setProperty("running", false);
        autoplayBtn_->style()->unpolish(autoplayBtn_);
        autoplayBtn_->style()->polish(autoplayBtn_);
    }
}

void MainWindow::onAutoplayStep() {
    if (!autoGame_) return;
    if (autoGame_->getBoard()->isGameOver()) {
        autoplayTimer_->stop();
        autoplayRunning_ = false;
        autoplayBtn_->setText("Autoplay");
        updateStatus();
        return;
    }
    autoGame_->playTurn();
    boardWidget_->update();
    updateStatus();
}

void MainWindow::onRandomize() {
    if (!manualGame_ || manualGame_->getBoard()->isGameOver()) return;
    manualGame_->randomizeState();
    boardWidget_->clearSelection();
    boardWidget_->update();
    updateStatus();
}

void MainWindow::onReplay() {
    QString path = QFileDialog::getOpenFileName(
        this, "Open Game Record", "records",
        "Text files (*.txt)");
    if (path.isEmpty()) return;

    try {
        auto [boardType, moves] = GameRecorder::load(path.toStdString());
        auto board = GameRecorder::createBoard(boardType);

        autoplayRunning_ = false;
        autoplayTimer_->stop();
        autoGame_.reset();
        manualGame_.reset();

        boardWidget_->setBoard(board.get());
        boardWidget_->update();

        auto sharedBoard = std::shared_ptr<Board>(board.release());
        auto movesCopy   = std::make_shared<std::vector<Move>>(moves);
        auto index       = std::make_shared<int>(0);

        QTimer* replayTimer = new QTimer(this);
        replayTimer->setInterval(500);

        connect(replayTimer, &QTimer::timeout, this,
            [this, sharedBoard, movesCopy, index, replayTimer]() {
                if (*index >= (int)movesCopy->size()) {
                    replayTimer->stop();
                    replayTimer->deleteLater();
                    updateStatus();
                    return;
                }
                Move& m = (*movesCopy)[*index];
                sharedBoard->makeMove(m.fromRow, m.fromCol,
                                      m.toRow,   m.toCol);
                boardWidget_->update();
                pegCountLabel_->setText(
                    QString("Pegs: %1").arg(sharedBoard->pegCount()));
                (*index)++;
            });

        replayTimer->start();
        setControlsEnabled(false);
        newGameBtn_->setEnabled(true);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Replay Error", e.what());
    }
}
