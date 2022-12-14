#include "game_struct.h"

game_struct::game_struct(int _bingo_id) : bingo_id(_bingo_id) {
	const int zero = 0;
	memcpy(hits, &zero, sizeof(hits));
    this->start = time(NULL);
}
game_struct::game_struct(int _bingo_id, bool _is_won, size_t _hits,
							decltype(start) _start, decltype(end) _end
						)
						: bingo_id(_bingo_id), is_won(_is_won),
							start(_start), end(_end){
	memcpy(hits, &_hits, sizeof(hits));
}
