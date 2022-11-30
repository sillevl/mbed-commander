# mbed-commander
mbed library for working with commands over serial or other interfaces.

## Commands

Commander supports tye following types of commands:

* `get`
* `set`
* `action`

You can escape commands using the `escape` key. The command will be ignored and you will be presented with a clear input to start a new command.

### Get command

Use these commands to get values back. This can be done by sending just the command string.

```text
TEMPERATURE
```

### Set command

Use these command to send values to the device. Set commands are the same as get commands, but use an `=` sign to followed by the argument value.

```text
COLOR=blue
```

### Action command

Actions are a type of command that neither set or get anything. They can be used to execute actions on the device.

```text
RESET
```

## Usage

Implement your own _Commands_ by extending the `Command` class. The constructor takes the command string to be used by the commands.

Override the `get`, `set` or `action` method to implement the desired command types.

```cpp
class HelloWorldCommand : public Commander::Command {
    public:
        HelloWorldCommand(const char* command) : Commander::Command(command) { };
        std::string get() override;
        std::string set(std::string argument) override;
        std::string action() override;
};
```

Next, add the command instances to an `Commander` by using the `addCommand()` method.

```cpp
Commander commander;
commander.addCommand(new HelloWorldCommand("HELLO"));
```

All that is left is from time to time call the `dispatch()` method in order to parse commands in the receive buffer.

```cpp
while(true) {
    commander.dispatch();
}
```

### input and output

Using the `Commander` constructor, the input and/or output FileHandles are configurable.

* Default `STDIN` and `STDOUT`:

    ```cpp
    Commander commander;
    ```

* Custom input and output:
  
    ```cpp
    BufferedSerial serial(USBTX, USBRX);
    Commander commander(&serial, &serial);
    ```

* Mixed input and output

    ```cpp
    BufferedSerial input(USBTX, USBRX);
    BufferedSerial output(D0, D1);
    Commander commander(&input, &output);
    ```

* Custom input, standard output

    ```cpp
    BufferedSerial input(USBTX, USBRX);
    Commander commander(&input);
    ```

## Example

First define a new `Command` class.

```cpp
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
```

```cpp
#include "mbed.h"
#include "commander.h"
#include "HelloWorldCommand.h"

using Commander;

int main() {
    mbed::FileHandle* uart = mbed::mbed_file_handle(STDIN_FILENO);
    Commander commander(uart);

    commander.addCommand(new HelloWorldCommand("HELLO"));

    while(true) {
        commander.dispatch();
    }

    return 0;
}
```

Now you can send the _set_ and _get_ commands using `HELLO`. `HELLO` will get the name stored by the HelloWorldCommand instance. `HELLO=` enables you to set a value in the HelloWorldCommand instance.

```text
HELLO=John Doe
OK
HELLO
John Doe
```

## Helpers

The command class provides some helpers in order to parse arguments.

* `getArguments(string input, char _separator = ',')`: Returns an vector containing the split parts of a string, separated by a separator character.
