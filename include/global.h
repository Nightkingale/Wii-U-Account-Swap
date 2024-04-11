#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

extern const char* NNID_BACKUP;
extern const char* PNID_BACKUP;
extern const char* INKAY_CONFIG;
extern const int BUFFER_SIZE;

extern unsigned int USER_ID;
extern std::string MII_NICKNAME;
extern std::string ACCOUNT_FILE;

void deinitialize();
void initialize();

#endif // GLOBAL_H