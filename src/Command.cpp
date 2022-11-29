#include "Command.h"
#include "mbed_trace.h"

#define TRACE_GROUP "COMMANDER"

namespace Commander {

    const char Command::ERROR_MESSAGE[] = MBED_CONF_COMMANDER_DEFAULT_ERROR_MESSAGE;
    const char Command::SUCCESS_MESSAGE[] = MBED_CONF_COMMANDER_DEFAULT_SUCCESS_MESSAGE;
    const std::string Command::whitespaces = std::string(" \t\n\r\f\v");

    Command::Command(const char* command){
        strcpy(this->command, command);
    }

    Response Command::execute(char* command) {

        size_t command_size = strlen(this->command);
        if( strncmp(command, this->command, command_size) != 0){
            return { Status::NOT_FOUND, NULL };   // command starts with wrong characters
        }

        if( strlen(command) == command_size ) {
            std::string result = get();
            tr_debug("get command found for %s", this->command);
            return { Status::SUCCESS, result };
        }

        if( command[command_size] == '!') {
            std::string result = action();
            tr_debug("action command found for %s", this->command);
            return { Status::SUCCESS, result };
        }
        
        if( command[command_size] == '=') {
            char argument[MBED_CONF_COMMANDER_MAX_SET_ARGUMENT_SIZE] = { 0 };
            strcpy(argument, command + command_size + 1);
            std::string result = set(argument);
            tr_debug("set command found for %s", this->command);
            tr_debug("set() argument size: %d", strlen(argument));
            return { Status::SUCCESS, result };
        }

        return { Status::NOT_FOUND, NULL };
    }

    std::string Command::get() {
        tr_debug("%s.get() is not implemented", this->command);
        return Command::ERROR_MESSAGE;
    }

    std::string Command::set(std::string argument) {
        tr_debug("%s.set() is not implemented", this->command);
        return Command::ERROR_MESSAGE;
    }

    std::string Command::action() {
        tr_debug("%s.action() is not implemented", this->command);
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
