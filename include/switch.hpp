#ifndef SWITCH_HPP
#define SWITCH_HPP


void handle_cleanup(FILE* account, FILE* backup, char* buffer, bool is_error);
void switch_account(const char* backupFile, const char* accountType);


#endif