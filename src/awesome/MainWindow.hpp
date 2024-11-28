#pragma once

#include <QMainWindow>

namespace awesome {

class MainWindow final : public QMainWindow {
	Q_OBJECT

	using Super = QMainWindow;
	using Self = MainWindow;

public:
	explicit MainWindow(QWidget* parent = nullptr);
};

} // namespace awesome
