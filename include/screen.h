#ifndef SCREEN_H
#define SCREEN_H

void printOnScreen(int line, const char* format, ...);
void printMainMenu();
void printWarningMenu();
void printOverwriteMenu(const char* backupPath);

#endif // SCREEN_H