#ifndef SUB_SCREENS_HPP
#define SUB_SCREENS_HPP


enum class success {
    backup,
    unlink,
    swap
};


void draw_unlink_menu();
void draw_backup_menu();
void draw_overwrite_menu(const char* backup_path);
void draw_error_menu(const char* error_message, const char* hint_message = nullptr);
void draw_success_menu(success type, bool inkay_configured = false);


#endif