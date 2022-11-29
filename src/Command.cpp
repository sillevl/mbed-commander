#include "Command.h"

namespace Commander {

    const char Command::ERROR_MESSAGE[] = MBED_CONF_COMMANDER_DEFAULT_ERROR_MESSAGE;
    const char Command::SUCCESS_MESSAGE[] = MBED_CONF_COMMANDER_DEFAULT_SUCCESS_MESSAGE;
    const std::string Command::whitespaces = std::string(" \t\n\r\f\v");

    Command::Command(const char* command){
        strcpy(this->command, command);
    }

    bool Command::execute(char* command) {

        size_t command_size = strlen(this->command);
        if( strncmp(command, this->command, command_size) != 0){
            return false;   // command starts with wrong characters
        }

        if( strlen(command) == command_size ) {
            std::string result = get();
            printf("%s\n", result.c_str());
            return true;
        }

        if( command[command_size] == '!') {
            std::string result = action();
            printf("%s\n", result.c_str());
            return true;
        }
        
        if( command[command_size] == '=') {
            char argument[100] = { 0 };
            strcpy(argument, command + command_size + 1);
            std::string result = set(argument);
            printf("%s\n", result.c_str());
            return true;
        }

        return false;
    }

    std::string Command::get() {
        return Command::ERROR_MESSAGE;
    }

    std::string Command::set(std::string argument) {
        return Command::ERROR_MESSAGE;
    }

    std::string Command::action() {
        return Command::ERROR_MESSAGE;
    }

    std::vector<std::string> Command::getArguments(std::string input, char _separator) {
        std::vector<std::string> arguments;
        std::string separator(1, _separator);
        size_t position = 0;
        while(position != std::string::npos) {
            position = input.find_first_of(separator);
            std::string argument = input.substr(0,position);
            arguments.push_back(trim(argument));
            input.erase(0, position + 1);
        }
        return arguments;
    }

    std::string& Command::rtrim(std::string& text, std::string whitespaces) {
        text.erase(text.find_last_not_of(whitespaces) + 1);
        return text;
    }

    std::string& Command::ltrim(std::string& text, std::string whitespaces) {
        text.erase(0, text.find_first_not_of(whitespaces));
        return text;
    }

    std::string& Command::trim(std::string& text, std::string whitespaces) {
        return ltrim(rtrim(text, whitespaces), whitespaces);
    }

};
