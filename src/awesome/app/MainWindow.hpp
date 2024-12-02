#pragma once

#include "ui_MainWindow.h"

namespace awesome::app {

class MainWindow final : public QMainWindow {
	Q_OBJECT

	using Super = QMainWindow;
	using Self = MainWindow;

public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	Ui::MainWindow ui_{};
};

} // namespace awesome::app
