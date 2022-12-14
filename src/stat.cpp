#include "stat.h"

#include "db.h"
#include "bingo.h"
#include "ui_bingo.h"
#include "ui_stat.h"

extern Bingo *w;

#define NOT_AVAILABLE "N/A"

Stat::Stat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stat)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    render();
}

Stat::~Stat()
{
	w->ui->stats->setEnabled(true);
    delete ui;
}

void Stat::render(){
    static constexpr size_t D_BUF_SIZE = 8;
    static constexpr size_t T_BUF_SIZE = 64;

    ui->NumOfGames_value->setText(NOT_AVAILABLE);
    ui->NumOfVic_value->setText(NOT_AVAILABLE);
    ui->TimePlayed_value->setText(NOT_AVAILABLE);

    ui->table->setRowCount(0);

	/* --- Add entries --- */
    ui->table->setRowCount(0);
    unsigned long i = 0;
	for(; i < games.size(); i++){
		auto& g = games[i];
		// Textual
        this->ui->table->setRowCount(i+1);
		// -- bingo name
		QString bn = "MISSING";
		for(auto& h : db_data){
			if(h->id == g->bingo_id){
				bn = h->name.c_str();
				break;
			}
		}
		// --- n hits
		int n_hits = 0;
		for(int i = 0; i < BINGO_SIZE; i++){
			if(g->hits[i]){
				++n_hits;
			}
		}
		char _n_hits[D_BUF_SIZE];
        snprintf(_n_hits, D_BUF_SIZE, "%d", n_hits);
		// --- start
		char _start[T_BUF_SIZE];
        tm* _tm = localtime(&(g->start));
		_tm->tm_hour -= 1;
		strftime(_start, T_BUF_SIZE, "%c", _tm);
		// ---
		QTableWidgetItem *items[] = {
            new QTableWidgetItem(bn),
			new QTableWidgetItem((g->is_won ? "Igen" : "Nem")),
			new QTableWidgetItem(_n_hits),
			new QTableWidgetItem(g->len()),
			new QTableWidgetItem(_start)
        };
		const size_t columns = sizeof(items) / sizeof(QTableWidgetItem*);
		for(size_t c = 0; c < columns; c++){
            this->ui->table->setItem(i, c, items[c]);
		}
	}


	/* --- Set counters --- */
	// --- time played
	double dt_sum = 0;
	for(auto& i : games){
        dt_sum += difftime(i->end, i->start);
	}
    time_t t_sum = (double)dt_sum;
	char _tp[D_BUF_SIZE];
    tm *_tm = localtime(&t_sum);
	_tm->tm_hour -= 1;
    strftime(_tp, T_BUF_SIZE, "%T", _tm);
	ui->TimePlayed_value->setText(_tp);
	// --- number of victories
	size_t n_won = 0;
	for(auto& i : games){
		if(i->is_won){
			++n_won;
		}
	}
    ui->NumOfVic_value->setText(QString::number(n_won));
	// --- number of games
    ui->NumOfGames_value->setText(QString::number(games.size()));

}

void Stat::on_reset_clicked()
{
    games.clear();
	sqlite3_exec(db, "DELETE FROM games;", NULL, NULL, NULL);
    render();
}

