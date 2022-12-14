#ifndef STAT_H
#define STAT_H

#include <QMainWindow>

namespace Ui {
class Stat;
}

class Stat : public QMainWindow
{
    Q_OBJECT

public:
    explicit Stat(QWidget *parent = nullptr);
    ~Stat();

private slots:
    void on_reset_clicked();

private:
    Ui::Stat *ui;

	void render();
};

#endif // STAT_H
