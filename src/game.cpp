#include "game.h"

#include "bingo.h"
#include "db.h"
#include "game_struct.h"
#include "ui_bingo.h"
#include "ui_game.h"
#include <algorithm>

extern Bingo *w;

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	/* Button accessability (script generated) */
	this->bingo_grid[0][0] = ui->pushButton_1;
	this->bingo_grid[0][1] = ui->pushButton_2;
	this->bingo_grid[0][2] = ui->pushButton_3;
	this->bingo_grid[0][3] = ui->pushButton_4;
	this->bingo_grid[0][4] = ui->pushButton_5;
	this->bingo_grid[1][0] = ui->pushButton_6;
	this->bingo_grid[1][1] = ui->pushButton_7;
	this->bingo_grid[1][2] = ui->pushButton_8;
	this->bingo_grid[1][3] = ui->pushButton_9;
	this->bingo_grid[1][4] = ui->pushButton_10;
	this->bingo_grid[2][0] = ui->pushButton_11;
	this->bingo_grid[2][1] = ui->pushButton_12;
	this->bingo_grid[2][2] = ui->pushButton_13;
	this->bingo_grid[2][3] = ui->pushButton_14;
	this->bingo_grid[2][4] = ui->pushButton_15;
	this->bingo_grid[3][0] = ui->pushButton_16;
	this->bingo_grid[3][1] = ui->pushButton_17;
	this->bingo_grid[3][2] = ui->pushButton_18;
	this->bingo_grid[3][3] = ui->pushButton_19;
	this->bingo_grid[3][4] = ui->pushButton_20;
	this->bingo_grid[4][0] = ui->pushButton_21;
	this->bingo_grid[4][1] = ui->pushButton_22;
	this->bingo_grid[4][2] = ui->pushButton_23;
	this->bingo_grid[4][3] = ui->pushButton_24;
	this->bingo_grid[4][4] = ui->pushButton_25;
	/* --- */

	/* Disable buttons */
	std::for_each(&(bingo_grid[0][0]), &(bingo_grid[0][0]) + BINGO_SIZE, [](QPushButton* b){
		b->setEnabled(false);
	});

	/* Set up button click handler */
    std::for_each(&(bingo_grid[0][0]), &(bingo_grid[0][0]) + BINGO_SIZE, [this](QPushButton* b){
        connect(b, SIGNAL(clicked()), SLOT(check_victory()));
	});

	/* Label buttons */
	for(int i = 0; i < BINGO_N; i++){
		for(int h = 0; h < BINGO_N; h++){
			auto& btn = bingo_grid[i][h];
			QLabel *l = new QLabel("");
			l->setWordWrap(true);
            l->setAlignment(Qt::AlignCenter);
			QHBoxLayout *lo = new QHBoxLayout(btn);
			lo->addWidget(l, 0);
		}
	}
	/* --- */

    this->render();

	/* Set up dropdown and default */
	if(db_data.size()){
		this->bingo = db_data[0];
		for(auto& i : db_data){
			ui->dropdown->addItem(i->name.c_str());
		}
	}else{
		ui->dropdown->addItem("None");
	}

}

Game::~Game()
{
    w->g = nullptr;
    w->ui->play->setEnabled(true);
    if(this->current_game){ delete this->current_game; } // maybe?
    delete ui;
}

void Game::on_start_clicked()
{
	if(not this->bingo){ return; }
    this->current_game = new game_struct(this->bingo->id);
    ui->start->setEnabled(false);
    ui->dropdown->setEnabled(false);
    ui->stop->setEnabled(true);
	std::for_each(&(bingo_grid[0][0]), &(bingo_grid[0][0]) + BINGO_SIZE, [](QPushButton* b){
		b->setChecked(false);
		b->setEnabled(true);
	});
}


void Game::on_stop_clicked()
{
    this->current_game->end = time(NULL);
    ui->stop->setEnabled(false);
    ui->dropdown->setEnabled(true);
    ui->start->setEnabled(true);
	for(int i = 0; i < BINGO_N; i++){
		for(int h = 0; h < BINGO_N; h++){
			QPushButton* &b = bingo_grid[i][h];
			current_game->hits[i*BINGO_N + h] = b->isChecked();
			b->setEnabled(false);
		}
    }

    games.push_back(current_game);
    current_game = nullptr;

    db_write_games();
}


void Game::on_dropdown_currentIndexChanged(const QString &index_str)
{
	for(auto& i : db_data){
		if(index_str == i->name.c_str()){
			this->bingo = i;
			break;
		}
	}
    this->render();
}

void Game::render(){
	if(this->bingo){
		/* Fill bingo grid */
		for(int i = 0; i < BINGO_N; i++){
			for(int h = 0; h < BINGO_N; h++){
				auto& btn = bingo_grid[i][h];
                ((QLabel*)(btn->children().at(1)))->setText(this->bingo->data.matrix[i][h].c_str());
			}
		}
		/* Set title */
		QString t = (this->bingo->name + " Bingo").c_str();
		t = t[0].toUpper() + t.right(t.length()-1);
        ui->title->setText(t);
	}else{
		/* Fill bingo grid */
        std::for_each(&(bingo_grid[0][0]), &(bingo_grid[0][0]) + BINGO_SIZE, [](QPushButton* b){
            ((QLabel*)(b->children().at(1)))->setText(BINGO_DEF_NAME);
		});
		/* Set title */
        ui->title->setText("Bingo");
	}
}

#define CHECK_WON if(is_won){ goto WON; }; \
    is_won = true
void Game::check_victory(){
    bool is_won;

    for(int i = 0; i < BINGO_N; i++){
        // --- vertical
        for(int h = 0; h < BINGO_N; h++){
            is_won = true;
            if(not this->bingo_grid[i][h]->isChecked()){
                is_won = false;
                break;
            }
        }
        CHECK_WON;
        // --- horizontal
        for(int h = 0; h < BINGO_N; h++){
            is_won = true;
            if(not this->bingo_grid[h][i]->isChecked()){
                is_won = false;
                break;
            }
        }
        CHECK_WON;
    }
    // --- increasing diagonal
    for(int j = 0; j < BINGO_N; j++){
        if(not this->bingo_grid[j][j]->isChecked()){
            is_won = false;
            break;
        }
    }
    CHECK_WON;
    // --- decreasig diagonal
    for(int j = 0; j < BINGO_N; j++){
        if(not this->bingo_grid[BINGO_N-1-j][j]->isChecked()){
            is_won = false;
            break;
        }
    }
    CHECK_WON;

    this->current_game->is_won = false;
    ui->title->setStyleSheet("");
    return;

    WON:
        this->current_game->is_won = true;
        ui->title->setStyleSheet("color: rgb(38, 162, 105);");
}

// WIP OPTIMIZED VERSION
//void Game::check_victory(){
//    static int ticked = 0;
//    static int n_is_won = 0;
//
//    QPushButton *s = qobject_cast<QPushButton*>(sender());
//    // Find in grid
//    for(int i = 0; i < BINGO_N; i++){
//        for(int h = 0; h < BINGO_N; h++){
//            auto& b = this->bingo_grid[i][h];
//            if(b != s){ continue; }
//			b->isChecked() ? ++ticked : --ticked;
//			if(ticked < BINGO_N){ goto NOT_WON; }
//            // Check affected lines
//            // --- vertical
//            bool is_won = true;
//            for(int j = 0; j < BINGO_N; j++){
//                if(not this->bingo_grid[j][h]->isChecked()){
//                    is_won = false;
//                    break;
//                }
//            }
//            CHECK_WON;
//            // --- horizontal
//            for(int j = 0; j < BINGO_N; j++){
//                if(not this->bingo_grid[i][j]->isChecked()){
//                    is_won = false;
//                    break;
//                }
//            }
//            CHECK_WON;
//            // --- increasing diagonal
//            if(i != h){ continue; }
//            for(int j = 0; j < BINGO_N; j++){
//                if(not this->bingo_grid[j][j]->isChecked()){
//                    is_won = false;
//                    break;
//                }
//            }
//            CHECK_WON;
//            // --- decreasig diagonal
//            if(BINGO_N-1 - i != h){ continue; }
//            for(int j = 0; j < BINGO_N; j++){
//                if(not this->bingo_grid[BINGO_N-1-j][j]->isChecked()){
//                    is_won = false;
//                    break;
//                }
//            }
//            CHECK_WON;
//        }
//    }
//
//	NOT_WON:
//		this->current_game->is_won = false;
//		ui->title->setStyleSheet("");
//		return;
//
//    WON:
//		this->current_game->is_won = true;
//        ui->title->setStyleSheet("color: rgb(38, 162, 105);");
//}
