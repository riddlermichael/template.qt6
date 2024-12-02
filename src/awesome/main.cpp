#include <awesome/app/MainWindow.hpp>

using namespace awesome;

int main(int argc, char* argv[]) {
	QApplication const app{argc, argv};

	app::MainWindow mainWindow;
	mainWindow.resize(320, 240);
	mainWindow.show();

	return QApplication::exec();
}
