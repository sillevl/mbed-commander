#include "Commander.h"
#include "mbed_trace.h"

#define TRACE_GROUP "COMMANDER"
namespace Commander {

    Commander::Commander(mbed::FileHandle* inputFileHandle, mbed::FileHandle* outputFileHandle) {
        this->inputFileHandle = inputFileHandle;
        this->outputFileHandle = outputFileHandle;
    }

    Commander::~Commander() {
        for (Command* command : commandList) {
            delete command;
        }
    }

    void Commander::dispatch() {
        while(inputFileHandle->readable()) {
            char received;
            inputFileHandle->read(&received, 1);
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
        tr_debug("A command is added to the Commander");
        commandList.push_back(command);
    }

    void Commander::parse(char* commandString) {
        bool success = false;

        for (Command* command : commandList) {
            Response response = command->execute(commandString);
            if(response.status != SUCCESS) { continue; }
                
            FILE* output = fdopen(outputFileHandle, "r+");
            fprintf(output, "%s\n", response.message.c_str());
            break;
        }

        if(!success) {
            tr_debug("command did not match any registered commands");
            FILE* output = fdopen(outputFileHandle, "r+");
            fprintf(output, "%s\n", Command::ERROR_MESSAGE);
        }
    }

};