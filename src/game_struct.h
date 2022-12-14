#ifndef GAME_STRUCT
#define GAME_STRUCT

#include <string.h>
#include <time.h>
#include <string>

struct game_struct{
	int bingo_id;
	bool is_won = false;
	bool hits[25];
	time_t start = 0;
	time_t end = 0;

	game_struct(int _bingo_id);
	game_struct(int _bingo_id, bool _is_won, size_t _hits,
					decltype(start) _start, decltype(end) _end
				);
	private:
		static constexpr size_t T_BUF_SIZE = 64;
        char* time_len = nullptr;

	public:
		char* len(){
            static tm *tm_ = nullptr;
            if(not time_len){
                time_len = (char*)calloc(100, 1);
                double ddiff = difftime(this->end, this->start);
                time_t diff = (time_t)ddiff;
                time_len = ctime(&diff);
                tm_ = localtime(&diff);
				tm_->tm_hour -= 1;
                strftime(time_len, T_BUF_SIZE, "%T", tm_);
            }
            return time_len;
		}
};

#endif
