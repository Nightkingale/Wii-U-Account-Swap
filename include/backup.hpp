#ifndef BACKUP_HPP
#define BACKUP_HPP


void write_backup(FILE* account, const std::string& backup_path, char* buffer);
void backup_account();


#endif