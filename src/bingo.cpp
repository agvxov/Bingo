#include "bingo.h"
#include "db.h"
#include "ui_bingo.h"
#include "game.h"
#include "stat.h"
#include "manager.h"

Bingo::Bingo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Bingo)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
}

Bingo::~Bingo()
{
	sqlite3_close(db);
	if(this->g){ delete g; }
	if(this->m){ delete m; }
    delete ui;
}


void Bingo::on_play_clicked()
{
    this->g = new Game;
    g->show();
	ui->play->setEnabled(false);
}

void Bingo::on_stats_clicked()
{
    this->s = new Stat;
    s->show();
    ui->stats->setEnabled(false);

}

void Bingo::on_collections_clicked()
{
    this->m = new Manager;
    m->show();
	ui->collections->setEnabled(false);
}


void Bingo::on_exit_clicked()
{
    exit(0);
}

