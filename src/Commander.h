#pragma once 

#include "mbed.h"
#include "Command.h"
#include <vector>

namespace Commander {

    class Commander {
        public:
            Commander(mbed::FileHandle* uart);
            ~Commander();

            void dispatch();
            void addCommand(Command* command);
            void parse(char* commandString);

        private:
            std::vector<Command*> commandList;
            mbed::FileHandle* uart;
            char buffer[MBED_CONF_COMMANDER_UART_BUFFER_SIZE];
            int buffer_index = 0; 
    };
};
