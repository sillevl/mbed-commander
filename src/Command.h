#pragma once
#include "mbed.h"
#include "string.h"
#include "vector"

namespace Commander {

    class Command {
        public: 
            Command(const char* command);
            Command(Command &command);
            bool execute(char* command);

            virtual std::string get();
            virtual std::string set(std::string argument);
            virtual std::string action();
        
            static const char ERROR_MESSAGE[];
            static const char SUCCESS_MESSAGE[];

        protected:
            std::vector<std::string> getArguments(std::string input, char _separator = ',');
            inline std::string& rtrim(std::string& text, std::string whitespaces = whitespaces);
            inline std::string& ltrim(std::string& text, std::string whitespaces = whitespaces);
            inline std::string& trim(std::string& text, std::string whitespaces = whitespaces);

        private:
            char command[MBED_CONF_COMMANDER_MAX_COMMAND_SIZE];
            static const std::string whitespaces;

    };
};

