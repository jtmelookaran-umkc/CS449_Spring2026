#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QTimer>
#include <memory>

#include "BoardWidget.h"
#include "Game.h"
#include "GameRecorder.h"

/**
 * MainWindow — contains layout for radio buttons as well as control for most of the UI
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onNewGame();
    void onMoveRequested(int fr, int fc, int tr, int tc);
    void onAutoplayStep();       // called by autoplay timer
    void onAutoplayToggle();
    void onRandomize();
    void onReplay();

private:
    // ── UI elements ──────────────────────────────────────────────
    BoardWidget*  boardWidget_;
    QLabel*       statusLabel_;
    QLabel*       pegCountLabel_;
    QLabel*       ratingLabel_;
    QPushButton*  newGameBtn_;
    QPushButton*  autoplayBtn_;
    QPushButton*  randomizeBtn_;
    QPushButton*  replayBtn_;
    QCheckBox*    recordCheck_;
    QComboBox*    boardSizeBox_;
    QRadioButton* englishRadio_;
    QRadioButton* hexagonRadio_;
    QRadioButton* diamondRadio_;
    QButtonGroup* boardTypeGroup_;

    // ── Game state ───────────────────────────────────────────────
    std::unique_ptr<ManualGame>   manualGame_;
    std::unique_ptr<AutoplayGame> autoGame_;
    bool          autoplayRunning_ = false;
    QTimer*       autoplayTimer_;

    // ── Helpers ──────────────────────────────────────────────────
    void buildUI();
    void applyStyleSheet();
    void startNewGame();
    void updateStatus();
    void setControlsEnabled(bool enabled);
    std::string selectedBoardType() const;
};
