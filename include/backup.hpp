#ifndef BACKUP_HPP
#define BACKUP_HPP


void writeBackup(FILE* account, const std::string& backupPath, char* buffer);

void backupAccount();


#endif