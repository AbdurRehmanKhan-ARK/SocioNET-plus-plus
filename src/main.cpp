#include "../include/Authentication.h"
#include "../include/UIHelpers.h"
#include "../include/UIUser.h"
#include <windows.h>
#include <iostream>

int main() {
    // This line sets the console output code page to UTF-8 (65001)
    // Purpose: Enables printing of box-drawing characters (╔, ═, ║, etc.) correctly
    SetConsoleOutputCP(65001);

    string choice; // Stores user's menu selection (1, 2, 3)
    Authentication auth;  // Authentication object to handle user registration and sign-in

    // Initialize email-to-username hashmap from persistent binary file
    // This loads all previous user registrations into memory
    // The username vector is also populated for graph operations
    if (!auth.initializeEmailHashmap()) {
        // Log warning if initialization fails, but continue (first run or no data yet)
        std::cerr << "Warning: Failed to load email hashmap on startup." << std::endl;
    }

    // Initialize and load friend graph
    UserHelper::friend_graph = new Graph();
    FileManager fm; // reuse file manager to load graph and usernames
    if (!fm.loadFriendGraphFromFile(*UserHelper::friend_graph)) {
        // Log warning if loading graph fails, but continue (first run or no data yet)
        std::cerr << "Warning: Failed to load friend graph from file." << std::endl;
    }
    // Ensure all registered users exist as vertices in graph
    UserHelper::all_usernames = fm.loadUsernameVector();
    for (const auto& uname : UserHelper::all_usernames) {
        UserHelper::friend_graph->addUser(uname);
    }

    // Build Suffix Automaton for username substring search
    UserHelper::sam = SuffixAutomaton();
    for (size_t i = 0; i < UserHelper::all_usernames.size(); i++) {
        UserHelper::sam.addUsername(UserHelper::all_usernames[i], static_cast<int>(i));
    }
    UserHelper::sam.finalize();
    

    User currentUser;  // Currently signed-in user (empty if none)
    // Main program loop: keeps running until user selects "3. Exit"
    while (true) {
        clearScreen();  // Clears console for a fresh, clean UI

        printBoxHeader("Welcome To SocioNET++");  // Prints boxed title
        printPaddedLine("1. Register");           // Option 1: Create new account
        printPaddedLine("2. Sign In");            // Option 2: Log in to existing account
        printPaddedLine("3. Exit");               // Option 3: Quit the program
        printBoxFooter();                         // Closes the UI box

        cout << "\n";  // Adds spacing before prompt
		cout << "Enter Your Choice (1-3): ";    // Prompt for Main Choice
        std::getline(std::cin, choice);  // Read full line input (safe for spaces or accidental text)

        if (choice == "1") {
            currentUser = auth.registerUserWithUI();  // Call registration function
            // After registration, loop returns to main menu
        }

        else if (choice == "2") {
            currentUser = auth.signInWithUI();  // Try to log in, returns pointer to user or nullptr
            if (!currentUser.getUsername().empty()) {  // If login successful (pointer is valid)
                dashboard(currentUser);  // Open user's personal dashboard
            }
            // If login failed (wrong password, etc.), loop continues to main menu
        }
         else if (choice == "3") {
            clearScreen();  // Clear screen for goodbye message
            printBoxHeader("Thank You For Using SocioNET++");  // Final message
            printBoxFooter();  // Close box
            break;  // Exit the while(true) loop → end program
        } 
        else {
            clearScreen();  // Fresh screen for error
            printBoxHeader("Invalid Choice");  // Error title
            printCenteredLine("Please Enter Correct Choice");  // Error message
            printBoxFooter();  // Close box
            waitEnter();  // Wait for user to press Enter before continuing
        }
	}

    // Cleanup allocated global graph
    if (UserHelper::friend_graph != nullptr) {
        delete UserHelper::friend_graph;
        UserHelper::friend_graph = nullptr;
    }

    // We should also cleanup suggestion graph if allocated
    if (UserHelper::suggestion_graph != nullptr) {
        delete UserHelper::suggestion_graph;
        UserHelper::suggestion_graph = nullptr;
    }

    return 0;
}