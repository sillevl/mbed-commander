#include "Commander.h"

namespace Commander {

    Commander::Commander(mbed::FileHandle* uart) {
        this->uart = uart;
    }

    Commander::~Commander() {
        for (Command* command : commandList) {
            delete command;
        }
    }

    void Commander::dispatch() {
        while(uart->readable()) {
            char received;
            uart->read(&received, 1);
            buffer[buffer_index] = received;
            buffer[buffer_index+1] = '\0';
            buffer_index++;

            if(received != '\r' && received != '\n') {
                continue;
            }

            buffer_index = 0;
            buffer[strcspn(buffer, "\r\n")] = 0; // clear newline or carriage return characters
            parse(buffer);
        }
    }

    void Commander::addCommand(Command* command) {
        commandList.push_back(command);
    }

    void Commander::parse(char* commandString) {
        bool success = false;

        for (Command* command : commandList) {
            success |= command->execute(commandString);
        }

        if(!success) {
            printf("%s\n", Command::ERROR_MESSAGE);
        }
    }

};