#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <algorithm>

namespace Ui {
class Manager;
}

class Manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private:
	void render();
    Ui::Manager *ui;
};

#endif // MANAGER_H
