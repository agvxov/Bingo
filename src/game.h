#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "bingo_struct.h"
#include "game_struct.h"
#include "qmainwindow.h"

class QPushButton;

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

	bingo_struct* bingo = nullptr;
	game_struct* current_game = nullptr;

	QPushButton *bingo_grid[BINGO_N][BINGO_N];

private slots:
    void on_start_clicked();

    void on_stop_clicked();

    void on_dropdown_currentIndexChanged(const QString &arg1);

    void check_victory();

private:
    Ui::Game *ui;
	void render();
};

#endif // GAME_H
