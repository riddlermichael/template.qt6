#include "MainWindow.hpp"

using namespace awesome::app;

MainWindow::MainWindow(QWidget* parent)
    : Super{parent} {
	ui_.setupUi(this);

	setWindowTitle("Awesome");
	setWindowIcon(QIcon{":/images/logo.png"});

	connect(ui_.actionExit, &QAction::triggered, this, &Self::close);
	connect(ui_.actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}
