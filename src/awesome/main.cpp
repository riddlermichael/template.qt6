#include <QApplication>

#include <awesome/MainWindow.hpp>

using namespace awesome;

int main(int argc, char* argv[]) {
	QApplication const app(argc, argv);

	MainWindow mainWindow;
	mainWindow.resize(320, 240);
	mainWindow.show();

	return QApplication::exec();
}
