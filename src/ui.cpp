#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>  // For usleep and close

// Placeholder for server log messages
std::vector<std::string> serverLog;

// Clear the screen and reset cursor
void clearScreen() {
    std::cout << "\033[2J\033[H";
}

// Draws the server UI with logs and command prompt
void drawUI() {
    clearScreen();

    // Server Header
    std::cout << "+-------------- ███    ██ ███████ ████████ ████████ ██     ██ ███████ ██████  ██   ██ ███████ ██████  ███████  --------------+\n";
    std::cout << "+-------------- ████   ██ ██         ██       ██    ██     ██ ██      ██   ██ ██  ██  ██      ██   ██ ██       --------------+\n";
    std::cout << "+-------------- ██ ██  ██ █████      ██       ██    ██  █  ██ █████   ██████  █████   █████   ██████  ███████  --------------+\n";
    std::cout << "+-------------- ██  ██ ██ ██         ██       ██    ██ ███ ██ ██      ██   ██ ██  ██  ██      ██   ██      ██  --------------+\n";
    std::cout << "+-------------- ██   ████ ███████    ██       ██     ███ ███  ███████ ██   ██ ██   ██ ███████ ██   ██ ███████  --------------+\n";

    std::cout << "| Logs:                                               |\n";
    int logDisplayLimit = 10;
    for (unsigned int i = std::max(0, (int)serverLog.size() - logDisplayLimit); i < serverLog.size(); ++i) {
        std::cout << "| " << serverLog[i];
        std::cout << std::string(50 - serverLog[i].size(), ' ') << "|\n";
    }
    
    std::cout << "+-----------------------------------------------------+\n";
    std::cout << "| > Command: ";
    std::cout.flush();
}

void logMessage(const std::string& message) {
    serverLog.push_back(message);
}

void serverLoop() {
    // Simulate server activity
    while (true) {
        logMessage("Server running...");
        usleep(5000000); // 5 seconds delay for example purposes
    }
}

void processCommand(const std::string& command) {
    if (command == "reboot") {
        logMessage("Server is rebooting...");
        // Simulate reboot operation here
    } else if (command == "clear") {
        serverLog.clear();
        logMessage("Logs cleared.");
    } else {
        logMessage("Unknown command: " + command);
    }
}

int main() {

    std::string command;
    while (true) {
        drawUI();
        std::getline(std::cin, command);
        if (command == "/exit") break;  // Exit command
        processCommand(command);        // Process the command
    }

    std::cout << "\nAdmin console exiting...\n";
    return 0;
}
