#ifndef BINGO_STRUCT
#define BINGO_STRUCT

#include <string>
#include <algorithm>

#define BINGO_N 5
#define BINGO_SIZE BINGO_N * BINGO_N
#define BINGO_DEF_NAME "N/A"

struct bingo_struct{
	int id;
	std::string name;
	std::string desc;

	struct Data{
		std::string matrix[BINGO_N][BINGO_N];
		const std::string *str(){
			if(this->str_ == ""){
				for(int i = 0; i < BINGO_N; i++){
					for(int h = 0; h < BINGO_N - 1; h++){
						str_ += matrix[i][h] + ';';
					}
					str_ += matrix[i][BINGO_N-1] + '\n';
				}
                str_ = str_.substr(0, str_.size()-1);
			}
            return const_cast<const std::string*>(&(this->str_));
		}

		private:
			std::string str_ = "";
	} data;

	bingo_struct();
	bingo_struct(const int id_, const char* name_, const char* desc_, FILE* fname_);
	bingo_struct(const int id_, const char* name_, const char* desc_, const char* data_);
	~bingo_struct();

	private:
		void construction_share(const int id_, const char* name_, const char* desc_, const char* data_);
};

#endif 
