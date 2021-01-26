#include <tictactoe.hpp>

ACTION tictactoe::create(const name &challenger , const name &host) {
    require_auth(host);
    check(challenger != host ,"host and challenger can't be same!");

    games_table gamesTable(get_self(), get_self().value);

    for (auto gameItr = gamesTable.begin(); gameItr != gamesTable.end(); gameItr++) {
        check(gameItr->host != host || gameItr->challenger != challenger , "this game already exists!");
    }
    gamesTable.emplace(get_self(), [&](auto &new_row) {
        new_row.host = host;
        new_row.challenger = challenger;
    });
}

ACTION tictactoe::close(const name &challenger , const name &host) {
    check(has_auth(host) || has_auth(host) || has_auth(get_self()) , "you'r not promitted for this operation!");
    games_table gamesTable(get_self(), get_self().value);
    bool deleted = false;
    for (auto gameItr = gamesTable.begin(); gameItr != gamesTable.end();) {
        if (gameItr->host == host && gameItr->challenger == challenger) {
            gamesTable.erase(gameItr++);
            deleted = true;
        }
    }
    check(deleted , "game not found!");

}

EOSIO_DISPATCH(tictactoe, (create)(close));