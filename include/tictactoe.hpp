#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

class [[eosio::contract]] tictactoe : public contract {
    public:
        using contract::contract;
        // creating the games and make sure that host and challenger not same
        // also check player didn't have same game in same time.
        [[eosio::action]]
        void create(const name &challenger, const name &host);
        // close players game after complete
        [[eosio::action]]
        void close(const name &challenger, const name &host);
        // find challenger current games
        [[eosio::action]]
        void challenges(const name &challenger);
    private:
        struct [[eosio::table]] games {
            name host;
            name challenger;

            uint64_t primary_key() const {return host.value;}
            uint64_t by_secondary() const {return challenger.value;}
        };
        typedef multi_index<
            name("games"),
            games,
            indexed_by<
                name("challenger"),
                const_mem_fun<games,
                uint64_t,
                &games::by_secondary
                >
            >
        > games_table;
};