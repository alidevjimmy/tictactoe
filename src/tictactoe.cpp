#include <tictactoe.hpp>
#include <vector>

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
            break;
        }
    }
    check(deleted , "game not found!");

}

ACTION tictactoe::challenges(const name &challenger) {
    check(has_auth(challenger) || has_auth(get_self()) , "operation not promitted!");

    bool gameFounded = false;

    games_table gamesTable(get_self(), get_self().value);
    auto gmChallengerIndex = gamesTable.get_index<name("challenger")>();

    for (auto gmItr = gmChallengerIndex.begin(); gmItr != gmChallengerIndex.end(); gmItr++) {
        if (gmItr->challenger.value == challenger.value) {
            print("Host: ", gmItr->host, " & Challenger (You): ", gmItr->challenger , "\n" );
            gameFounded = true;
        }
    }

    check(gameFounded, "no game in play right now!");
}

EOSIO_DISPATCH(tictactoe, (challenges)(create)(close));