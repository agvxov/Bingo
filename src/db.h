#ifndef BINGO_DB
#define BINGO_DB

#include <sqlite3.h>
#include <vector>
#include <string>
#include "bingo_struct.h"
#include "game_struct.h"

const char *const db_file = "db.sqlite";

extern sqlite3* db;
extern int db_init;
extern std::vector<bingo_struct*> db_data;
extern std::vector<game_struct*> games;

bool db_read();
bool db_read_collection();
bool db_read_games();
bool db_write();
bool db_write_collection();
bool db_write_games();

#endif
