#include "manager.h"

#include "db.h"
#include "bingo.h"
#include "ui_bingo.h"
#include "ui_manager.h"

#include <QFileDialog>

extern Bingo *w;

Manager::Manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->render();
}

void Manager::render(){
	/* --- Add entries --- */
    ui->table->setRowCount(0);
    unsigned long i = 0;
	for(; i < db_data.size(); i++){
		auto& b = db_data[i];
        ui->table->setRowCount(i+1);

		char _id[8];
        snprintf(_id, 8, "%d", b->id);
		QTableWidgetItem* w_id = new QTableWidgetItem(_id);
        w_id->setFlags(w_id->flags() & Qt::ItemIsEnabled);

		QTableWidgetItem *items[] = {
			w_id,
			new QTableWidgetItem(b->name.c_str()),
            new QTableWidgetItem(b->desc.c_str()),
            new QTableWidgetItem(b->data.str()->c_str())
        };
        const size_t columns = (sizeof(items) / sizeof(QTableWidgetItem*));
		for(size_t c = 0; c < columns; c++){
            this->ui->table->setItem(i, c, items[c]);
        }

		// Delete button
		QPushButton *d = new QPushButton("Törlés");
		d->setObjectName("delete_button");
        connect(d, &QPushButton::clicked, [=](){
            db_data.erase(db_data.begin() + i);
            ui->table->removeRow(i);
            render();
		});
        ui->table->setCellWidget(i, 4, d);
	}

    ui->table->setRowCount(i+1);
	int h = 0;

	// Save button
	QPushButton *s = new QPushButton("Mentés");
	s->setObjectName("save_button");
	connect(s, &QPushButton::clicked, [&](){
        db_data.clear();
        for(int i = 0; i < (ui->table->rowCount()-1); i++){
            try{
                new bingo_struct(
                        ui->table->item(i, 0)->text().toInt(),
                        ui->table->item(i, 1)->text().toStdString().c_str(),
                        ui->table->item(i, 2)->text().toStdString().c_str(),
                        ui->table->item(i, 3)->text().toStdString().c_str()
                    );
            }catch(std::invalid_argument e){}
        }
		if(not db_write_collection()){
			throw std::runtime_error("db write error");
		}
	});
    ui->table->setCellWidget(i, h++, s);

	// "New from file" button
	QPushButton *nf = new QPushButton("💾");
	nf->setObjectName("new");
	connect(nf, &QPushButton::clicked, [&](){
		QString filename = QFileDialog::getOpenFileName(this, tr("Upload Bingo"), QDir::currentPath(), "Bingo Files (*.bingo);;All Files (*.*)");
		if(not filename.isEmpty() && not filename.isNull()){
            FILE* f = fopen(filename.toStdString().c_str(), "r");
			try{
				new bingo_struct(0, NULL, NULL, f);
            }catch(std::invalid_argument& e){
                fputs(e.what(), stderr);
			}
			fclose(f);
			render();
		}
	});
    ui->table->setCellWidget(i, h++, nf);

	// Add new
	QPushButton *n = new QPushButton("+");
	n->setObjectName("new");
    connect(n, &QPushButton::clicked, [this](){
		new bingo_struct();
		render();
	});
    ui->table->setCellWidget(i, h++, n);

	//// blank
	//QPushButton *placeholder = new QPushButton("");
	//placeholder->setEnabled(false);
    //ui->table->setCellWidget(i, h++, placeholder);

	// reload button
	QPushButton *r = new QPushButton("Újra töltés");
	r->setObjectName("reload");
    connect(r, &QPushButton::clicked, [this](){
		db_data.clear();
		db_read_collection();
		render();
	});
    ui->table->setCellWidget(i, h++, r);

	// Clear button
	QPushButton *c = new QPushButton("Ürítés");
	c->setObjectName("clear");
    connect(c, &QPushButton::clicked, [this](){
		db_data.clear();
		render();
	});
    ui->table->setCellWidget(i, h++, c);
}

Manager::~Manager(){
    w->m = nullptr;
    w->ui->collections->setEnabled(true);
    delete ui;
}
