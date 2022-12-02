#include "Commander.h"
#include "mbed_trace.h"

#define TRACE_GROUP "COMMANDER"
namespace Commander {

    static const char LF_S = '\n';
    static const char CR_S = '\r';
    static const char ESC = 0x1B;
    static const char DEL = 0x7F;
    static const char BS  = 0x08;
    static const char ETX = 0x03; //ctrl+c (End of text)
    static const char ETB = 0x17;  
    static const char TAB = 0x09;
    static const char CAN = 0x18; // ctrl+x (cancel)

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

            if(received == ETX || received == CAN) {   
                char buf[] = { 'E', 'S', 'C', CR_S, LF_S };
                outputFileHandle->write(buf, sizeof(buf));
                break; 
            }

            outputFileHandle->write(&received, 1);

            if(received == BS || received == DEL) {
                buffer[--buffer_index] = '\0';
                break;
            }

            buffer[buffer_index] = received;
            buffer[buffer_index+1] = '\0';
            buffer_index++;

            if(received != '\r' && received != '\n') {
                continue;
            }

            outputFileHandle->write(&LF_S, 1);

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
            
            success = true;
            FILE* output = fdopen(outputFileHandle, "r+");
            fprintf(output, "%s\n", response.message.c_str());
            if(this->onCommand) { this->onCommand(); };
            break;
        }

        if(!success) {
            tr_debug("command did not match any registered commands");
            FILE* output = fdopen(outputFileHandle, "r+");
            fprintf(output, "%s\n", Command::ERROR_MESSAGE);
        }
    }

    void Commander::on_command(mbed::Callback<void()> cb) {
        this->onCommand = cb;
    }
};