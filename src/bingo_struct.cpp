#include "bingo_struct.h"

#include <stdexcept>
#include <string.h>
#include <vector>

extern std::vector<bingo_struct*> db_data;

bingo_struct::bingo_struct(){
    construction_share(0, NULL, NULL, " ; ; ; ; \n ; ; ; ; \n ; ; ; ; \n ; ; ; ; \n ; ; ; ; \n");
}

bingo_struct::bingo_struct(const int id_, const char* name_,
                            const char* desc_, FILE* fname){
	if(!fname){ throw int(1); }

	fseek(fname, 0, SEEK_END);
	int flen = ftell(fname);
	rewind(fname);
	char fstr[flen+1];
	fstr[flen] = '\00';
	fread(fstr, flen, sizeof(char), fname);

    construction_share(id_, name_, desc_, fstr);
}

bingo_struct::bingo_struct(const int id_, const char* name_,
							const char* desc_, const char* data_){
    construction_share(id_, name_, desc_, data_);
}

void bingo_struct::construction_share(const int id_, const char* name_,
							const char* desc_, const char* data_){
    if(id_ == 0){
        int next_id = 0;
        for(auto &h : db_data){
            if(h->id > next_id){
                next_id = h->id;
            }
        }
        ++next_id;
        this->id = next_id;
    }else{
        this->id = id_;
    }

    if(name_ == NULL){
		this->name = BINGO_DEF_NAME;
        this->name += ' ' + std::to_string(this->id);
    }else{
        this->name = name_;
    }

    if(desc_ == NULL){
		this->desc = BINGO_DEF_NAME;
	}else{
        this->desc = desc_;
	}

    char* fstr = strdup(data_);

	char* line_save;
	char* line = strtok_r(fstr, "\n", &line_save);
	int i = 0;
	int h;
	do{
		h = 0;
		char* col = strtok(line, ";");
		if(col == NULL){ goto PARSE_ERROR; }
		do{
            this->data.matrix[i][h] = col;
			++h;
		}while((col = strtok(NULL, ";"), col));
		if(h != BINGO_N){ goto PARSE_ERROR; }
		++i;
	}while((line = strtok_r(NULL, "\n", &line_save), line));
	if(i != BINGO_N){ goto PARSE_ERROR; }

	db_data.push_back(this);

	return;

	PARSE_ERROR:
		throw std::invalid_argument("parse error uppon bingo_struct construction");
}

bingo_struct::~bingo_struct(){
}
