#ifndef SWAP_HPP
#define SWAP_HPP


enum class account {
    nintendo_network_id,
    pretendo_network_id
};


bool swap_account(const char* backup_file, account account_type);


#endif