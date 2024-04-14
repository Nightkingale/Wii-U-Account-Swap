#ifndef SWITCH_HPP
#define SWITCH_HPP


void handleCleanup(FILE* account, FILE* backup, char* buffer, bool isError);

void switchAccount(const char* backupFile, const char* accountType);


#endif