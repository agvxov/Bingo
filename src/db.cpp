#include "db.h"

#include <iostream>

static int db_init_();

inline sqlite3* db;
inline int db_init = db_init_();
inline std::vector<bingo_struct*> db_data;
inline std::vector<game_struct*> games;

bool db_read(){
    return db_read_collection() and db_read_games();
}

bool db_read_collection(){
	int exec;

	const auto read_bingo = [](void* ignore [[ maybe_unused ]],
							int c [[ maybe_unused ]],
							char** val,
							char** col [[ maybe_unused ]]){
			try{
				new bingo_struct(
                        std::stoi(val[0]),
						val[1],
						val[2],
						val[3]
					);
            }catch(std::invalid_argument& e){
                fputs(e.what(), stderr);
			}
			return 0;
		};


	exec = sqlite3_exec(db, 
				"SELECT rowid, * FROM bingos;",
				*read_bingo,
				NULL,
				NULL
	);
	if(exec != SQLITE_OK){ return false; }

	return true;
}

bool db_read_games(){
	int exec;

	const auto read_game = [](void* ignore [[ maybe_unused ]],
							int c [[ maybe_unused ]],
							char** val,
							char** col [[ maybe_unused ]]){
			games.push_back(
				new game_struct(
					std::stoi(val[0]),
					std::stoi(val[1]),
					std::stoi(val[2]),
					std::stoi(val[3]),
					std::stoi(val[4])
				)
			);

			return 0;
		};

	exec = sqlite3_exec(db, 
				"SELECT * FROM games;",
				*read_game,
				NULL,
				NULL
	);
	if(exec != SQLITE_OK){ return false; }

	return true;
}

#define CHECK_ERR(e){ \
	exec = e; \
	if(not(exec == SQLITE_OK || exec == SQLITE_DONE)){ goto ERROR; } \
}

#define LABEL_ERR \
    ERROR: \
        fprintf(stderr, "%d: %s\n", exec, sqlite3_errmsg(db)); \
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL); \
        return false

bool db_write_collection(){
	int exec;
	sqlite3_stmt *stmt;
	static constexpr char query[] = "INSERT INTO bingos (rowid, name, desc, data)\
							VALUES (?1, ?2, ?3, ?4);";

	CHECK_ERR(sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL));

	CHECK_ERR(sqlite3_exec(db, "DELETE FROM bingos;", NULL, NULL, NULL));

    CHECK_ERR(sqlite3_prepare_v2(db, query, sizeof(query)-1, &stmt, NULL));
	for(auto i : db_data){
		CHECK_ERR(sqlite3_bind_int(stmt, 1, i->id));
		CHECK_ERR(sqlite3_bind_text(stmt, 2, i->name.c_str(), i->name.size(), SQLITE_STATIC));
		CHECK_ERR(sqlite3_bind_text(stmt, 3, i->desc.c_str(), i->desc.size(), SQLITE_STATIC));
        CHECK_ERR(sqlite3_bind_text(stmt, 4, i->data.str()->c_str(), i->data.str()->size(), SQLITE_STATIC));
		CHECK_ERR(sqlite3_step(stmt));
        CHECK_ERR(sqlite3_reset(stmt));
    }
    CHECK_ERR(sqlite3_finalize(stmt));

	
	CHECK_ERR(sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL));

	return true;

    LABEL_ERR;
}

bool db_write_games(){
	int exec;
	sqlite3_stmt *stmt;
	static constexpr char query[] = "INSERT INTO games (bingo_id, is_won, hits, start, end)\
							VALUES (?1, ?2, ?3, ?4, ?5);";

	CHECK_ERR(sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL));

	CHECK_ERR(sqlite3_exec(db, "DELETE FROM games;", NULL, NULL, NULL));

    CHECK_ERR(sqlite3_prepare_v2(db, query, sizeof(query)-1, &stmt, NULL));
	for(auto i : games){
		CHECK_ERR(sqlite3_bind_int(stmt, 1, i->bingo_id));
        CHECK_ERR(sqlite3_bind_int(stmt, 2, i->is_won));
        CHECK_ERR(sqlite3_bind_int(stmt, 3, (int)(*(i->hits))));
        CHECK_ERR(sqlite3_bind_int(stmt, 4, i->start));
        CHECK_ERR(sqlite3_bind_int(stmt, 5, i->end));
		CHECK_ERR(sqlite3_step(stmt));
        CHECK_ERR(sqlite3_reset(stmt));
    }
    CHECK_ERR(sqlite3_finalize(stmt));

	
	CHECK_ERR(sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL));

	return true;

    LABEL_ERR;
}

bool db_write(){
	return db_write_collection() and db_write_games();
}

static int db_init_(){
    static constexpr char create_statements[] = "\
            CREATE TABLE IF NOT EXISTS bingos (name text, desc text, data text); \
            CREATE TABLE IF NOT EXISTS games(bingo_id int, is_won bool, hits integer(4), start int, end int); \
    ";
    int exec;
    CHECK_ERR(sqlite3_open(db_file, &db));
    CHECK_ERR(sqlite3_exec(db, create_statements, NULL, NULL, NULL));

    return 0;

    LABEL_ERR;
}
