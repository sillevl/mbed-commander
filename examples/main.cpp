#include "mbed.h"
#include "commander.h"

class HelloWorldCommand : public Commander::Command {
    public:
        HelloWorldCommand(const char* command) : Commander::Command(command) {

        };

        std::string get() override {
            return name;
        };

        std::string set(std::string argument) override {
            name = argument;
            return Commander::Command::SUCCESS_MESSAGE;
        };

    private:
        std::string name;
};

int main() {
    mbed::FileHandle* uart = mbed::mbed_file_handle(STDIN_FILENO);
    Commander::Commander commander(uart);

    commander.addCommand(new HelloWorldCommand("HELLO"));

    while(true) {
        commander.dispatch();
    }

    return 0;
}