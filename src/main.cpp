#include "bingo.h"
#include "db.h"

#include <QApplication>
#include <QStaticText>

Bingo *w;

int main(int argc, char *argv[]){
	if(db_init != SQLITE_OK){ return 1; }
	db_read();
	QApplication a(argc, argv);
	w = new Bingo;
    w->show();
	return a.exec();
}
