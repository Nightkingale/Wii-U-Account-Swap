#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>


extern const int BUFFER_SIZE;

extern unsigned int USER_ID;
extern std::string NNID_BACKUP;
extern std::string PNID_BACKUP;
extern std::string MII_NICKNAME;
extern std::string ACCOUNT_FILE;
extern std::string INKAY_CONFIG;

void deinitialize();
void initialize();
void printMainMenu();
void printBackupMenu();
void printOverwriteMenu(const char* backupPath);
void backupAccount();

#endif // GLOBAL_H