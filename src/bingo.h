#ifndef BINGO_H
#define BINGO_H

#include "game.h"
#include "stat.h"
#include "manager.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Bingo; }
QT_END_NAMESPACE


class Bingo : public QMainWindow
{
    Q_OBJECT

    friend class Game;
	friend class Stat;
	friend class Manager;

	Game *g = nullptr;
    Manager *m = nullptr;
    Stat *s = nullptr;

public:
    Bingo(QWidget *parent = nullptr);
    ~Bingo();

private slots:
    void on_play_clicked();

    void on_collections_clicked();

    void on_exit_clicked();

    void on_stats_clicked();

private:
    Ui::Bingo *ui;
};
#endif // BINGO_H
