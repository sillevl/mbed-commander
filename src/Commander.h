#pragma once 

#include "mbed.h"
#include "Command.h"
#include <vector>

namespace Commander {

    class Commander {
        public:
            Commander(
                mbed::FileHandle* inputFileHandle = mbed_file_handle(STDIN_FILENO), 
                mbed::FileHandle* outputFileHandle = mbed_file_handle(STDOUT_FILENO)
            );
            ~Commander();

            void dispatch();
            void addCommand(Command* command);
            void parse(char* commandString);
            void on_command(mbed::Callback<void()> cb);

        private:
            std::vector<Command*> commandList;
            mbed::FileHandle* inputFileHandle;
            mbed::FileHandle* outputFileHandle;
            char buffer[MBED_CONF_COMMANDER_UART_BUFFER_SIZE];
            int buffer_index = 0; 
            mbed::Callback<void()> onCommand;
    };
};
