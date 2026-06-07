// uiAuthentication.cpp
// Implementation of UI functions for user registration and sign-in
// Adapted to work with Authentication class

#include "../include/UIAuthentication.h"
#include <regex>
#include <algorithm>
using std::string;
using std::regex;
using std::getline;
using std::cout;
using std::cin;
using std::endl;

// Regex patterns for validation (matching backend)
const regex usernamePattern(R"(^(?!\d+$)(?!.*\.\.)(?!\.)(?!.*\.$)[A-Za-z0-9._]{3,30}$)");
const regex emailPattern(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
const regex passwordPattern(R"((?=.*\d)(?=.*[!@#$%^&*(),.?:{}|<>_\-+=~`])[A-Za-z\d!@#$%^&*(),.?:{}|<>_\-+=~`]{6,30})");

User registerUser(Authentication& auth) {
    string username, email, password;
    clearScreen();
    printBoxHeader("SocioNET++ Registration");
    printCenteredLine("Please Enter Your Primary Data | Username → Email → Password → Name");
    printBoxSeparator();

    // Username Input And Validation Loop
    while (true) {
        bool cap = true;
        string prompt = "║ 1. Enter Username: ";
        for (char c : prompt) {
            if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
            else { cout << c; cap = isspace(c); }
        }
        getline(cin, username);

        if (!regex_match(username, usernamePattern)) {
            clearScreen();
            printBoxHeader("SocioNET++ Registration");
            printCenteredLine("Warning: Username Has Following Limitations");
            printBoxSeparator();
            printCenteredLine("1. Cannot Be All Digits");
            printCenteredLine("2. No Consecutive Dots");
            printCenteredLine("3. Cannot Start\\End With Dot");
            printCenteredLine("4. 3-30 [Characters + . + _]");
            printBoxFooter();
            waitEnter();
            continue;
        }

        if (!auth.isUsernameAvailable(username)) {
            clearScreen();
            printBoxHeader("SocioNET++ Registration");
            printCenteredLine("Warning: Username Already Taken");
            printBoxFooter();
            waitEnter();
            continue;
        }
        break;
    }

    // Email Input And Validation Loop
    while (true) {
        bool cap = true;
        string prompt = "║ 2. Enter Email: ";
        for (char c : prompt) {
            if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
            else { cout << c; cap = isspace(c); }
        }
        getline(cin, email);

        if (!regex_match(email, emailPattern)) {
            clearScreen();
            printBoxHeader("SocioNET++ Registration");
            printPaddedLine("Username: ", username);
            printBoxSeparator();
            printCenteredLine("Warning: Email Has Following Limitations");
            printBoxSeparator();
            printCenteredLine("1. Exactly 1 '@' And '.'");
            printCenteredLine("2. No Space, Special Chars In Domain");
            printCenteredLine("3. At Least 2 Chars Post '.'");
            printCenteredLine("4. Allowed Special Ones (. , _)");
            printBoxFooter();
            waitEnter();
            continue;
        }

        if (!auth.isEmailAvailable(email)) {
            clearScreen();
            printBoxHeader("SocioNET++ Registration");
            printPaddedLine("Username: ", username);
            printBoxSeparator();
            printCenteredLine("Warning: Email Already Registered");
            printBoxFooter();
            waitEnter();
            continue;
        }
        break;
    }

    // Password Input And Validation Loop
    while (true) {
        bool cap = true;
        string prompt = "║ 3. Enter Password: ";
        for (char c : prompt) {
            if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
            else { cout << c; cap = isspace(c); }
        }
        getline(cin, password);

        if (!regex_match(password, passwordPattern)) {
            clearScreen();
            printBoxHeader("SocioNET++ Registration");
            printPaddedLine("Username: ", username);
            printPaddedLine("Email: ", email);
            printBoxSeparator();
            printCenteredLine("Warning: Password Has Following Limitations");
            printBoxSeparator();
            printCenteredLine("1. At Least 1 Digit");
            printCenteredLine("2. Length 6-30 Characters");
            printCenteredLine("3. At Least 1 Special Character");
            printBoxFooter();
            waitEnter();
            continue;
        }
        break;
    }

    // Register user using Authentication class
    string errorMsg;
    if (auth.signup(username, email, password, errorMsg)) {
        clearScreen();
        printBoxHeader("SocioNET++ Registration Successful");
        printPaddedLine("Username: ", username);
        printPaddedLine("Email: ", email);
        printBoxSeparator();
        printCenteredLine("Registration Successful For User: " + username);
        printBoxFooter();
        waitEnter();
        
        // Return the newly registered user by signing in
        User newUser = auth.signin(username, password, errorMsg);
        return newUser;
    } else {
        clearScreen();
        printBoxHeader("SocioNET++ Registration Failed");
        printCenteredLine("Error: " + errorMsg);
        printBoxFooter();
        waitEnter();
        return User(); // Return empty user on failure
    }
}

User signIn(Authentication& auth) {
    string username, password;
    clearScreen();
    printBoxHeader("SocioNET++ Sign In");

    // Step 1: Enter And Validate Username
    while (true) {
        bool cap = true;
        string prompt = "║ 1. Enter Username or Email: ";
        for (char c : prompt) {
            if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
            else { cout << c; cap = isspace(c); }
        }
        getline(cin, username);

        if (username.empty()) {
            clearScreen();
            printBoxHeader("SocioNET++ Sign In");
            printCenteredLine("Warning: Username Cannot Be Empty");
            printBoxFooter();
            waitEnter();
            continue;
        }
        break;
    }

    // Step 2: Enter Password
    bool cap = true;
    string prompt = "║ 2. Enter Password: ";
    for (char c : prompt) {
        if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
        else { cout << c; cap = isspace(c); }
    }
    getline(cin, password);

    // Attempt sign in
    string errorMsg;
    User currentUser = auth.signin(username, password, errorMsg);

    if (!currentUser.getUsername().empty()) {
        clearScreen();
        printBoxHeader("Sign In Successful!");
        printCenteredLine("Welcome Back, Dear " + currentUser.getUsername() + "!");
        printPaddedLine("Email: ", currentUser.getEmail());
        printBoxFooter();
        waitEnter();
        return currentUser;
    } else {
        // Handle wrong password with forgot password option
        string resetChoice;
        while (true) {
            clearScreen();
            printBoxHeader("SocioNET++ Sign In");
            printPaddedLine("Username: ", username);
            printBoxSeparator();
            printCenteredLine("Warning: " + errorMsg);
            printBoxSeparator();
            bool cap = true;
            string forgot = "Forgot Password? (Y/N): ";
            for (char c : forgot) {
                if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
                else { cout << c; cap = isspace(c); }
            }
            getline(cin, resetChoice);

            if (resetChoice == "y" || resetChoice == "Y") {
                // Password reset would need to be implemented in Authentication class
                clearScreen();
                printBoxHeader("Password Reset");
                printCenteredLine("Password Reset Feature Not Yet Implemented");
                printCenteredLine("Please Contact Administrator");
                printBoxFooter();
                waitEnter();
                break;
            }
            else if (resetChoice == "n" || resetChoice == "N") {
                break;
            }
        }
        return User(); // Return empty user on failed sign in
    }
}

