#ifndef SWITCH_H
#define SWITCH_H

void handleCleanup(FILE* account, FILE* backup, char* buffer, bool isError);
void switchAccount(const char* backupFile, const char* accountType);

#endif // SWITCH_H