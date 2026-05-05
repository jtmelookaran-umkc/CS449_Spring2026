#include <QApplication>
#include "gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Peg Solitaire");
    app.setApplicationVersion("1.0");

    MainWindow window;
    window.show();

    return app.exec();
}
