#ifndef UTILS2_H
#define UTILS2_H

#include <string>

void display_welcome_message();

void disp_prompt();

void display_error_message();
void display_help_prompt();

void display_help();

std::string convertToString(char* a, int size);
void show_not_implemented_info();

void show_command_failed_info();

#endif