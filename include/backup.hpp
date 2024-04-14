#ifndef BACKUP_H
#define BACKUP_H

void writeBackup(FILE* account, const std::string& backupPath, char* buffer);
void backupAccount();

#endif // BACKUP_H