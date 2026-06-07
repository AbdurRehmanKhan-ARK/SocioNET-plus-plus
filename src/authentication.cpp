// authentication.cpp
// Implementation of Authentication class for user registration and sign-in
// Serves as bridge between frontend UI and backend logic

#include "../include/Authentication.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <iostream>
#include <iomanip>
#include <exception>
using namespace std;

Authentication::Authentication() {
    // Constructor
}

// ============================================================================
// FUNCTION: initializeEmailHashmap
// ============================================================================
// Loads email-to-username hashmap from binary file (data/graphs/email_to_username.bin)
// and populates the all_usernames vector with all usernames
// This should be called once at program startup
// Returns true if successful (or file doesn't exist yet), false on error
bool Authentication::initializeEmailHashmap() {
    try {
        // Load email-to-username hashmap from binary file
        UserHelper::email_to_username = fileManager.loadEmailHashmap();
        
        // Load username vector from the hashmap
        UserHelper::all_usernames = fileManager.loadUsernameVector();
        
        return true;  // Successfully loaded
    } catch (const std::exception& e) {
        // In case of any error during loading, return false
        return false;
    }
}

char Authentication::getFirstAlphabeticChar(const string& username) const {
    for (char c : username) {
        if (isalpha(c)) {
            return tolower(c);
        }
    }
    return '\0'; // No alphabetic character found
}

bool Authentication::isValidEmail(const string& email) const {
    // Basic email validation using regex
    // Email should be in the format of name@domain.com
    regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailPattern);
}

bool Authentication::isValidPassword(const string& password) const {
    // Password validation using regex
    // At least 6 characters, at least one digit, at least one special character
    regex passwordPattern(R"((?=.*\d)(?=.*[!@#$%^&*(),.?:{}|<>_\-+=~`])[A-Za-z\d!@#$%^&*(),.?:{}|<>_\-+=~`]{6,30})");
    return regex_match(password, passwordPattern);
}

bool Authentication::isValidUsername(const string& username) const {
    // Username validation using regex
    // No digits only, no consecutive dots, no starting or ending dots
    regex usernamePattern(R"(^(?!\d+$)(?!.*\.\.)(?!\.)(?!.*\.$)[A-Za-z0-9._]{3,30}$)");
    return regex_match(username, usernamePattern);
}

bool Authentication::isUsernameAvailable(const string& username) const {
    return !fileManager.usernameExists(username);
}

bool Authentication::isEmailAvailable(const string& email) const {
    return !userHelper.isEmailRegistered(email);
}

// Get username by email (Help in login by either username or email)
string Authentication::getUsernameByEmail(const string& email) const {
    auto it = UserHelper::email_to_username.find(email);
    if (it != UserHelper::email_to_username.end()) {
        return it->second; // Key found, return its value
    } else {
        return ""; // Key not found
    }
}

// ============================================================================
// BACKEND LOGIC METHODS (Private - called by UI methods)
// ============================================================================

bool Authentication::signup(const string& username, const string& email, 
                           const string& password, string& errorMsg) const {
    // Validate input
    if (username.empty()) {
        errorMsg = "Username cannot be empty.";
        return false;
    }
    
    if (email.empty()) {
        errorMsg = "Email cannot be empty.";
        return false;
    }
    
    if (password.empty()) {
        errorMsg = "Password cannot be empty.";
        return false;
    }
    
    // Validate email format
    if (!isValidEmail(email)) {
        errorMsg = "Invalid email format.";
        return false;
    }
    
    // Validate password strength
    if (!isValidPassword(password)) {
        errorMsg = "Password must be at least 6 characters long.";
        return false;
    }
    
    // Check username uniqueness (using binary search)
    if (!isUsernameAvailable(username)) {
        errorMsg = "The username is taken.";
        return false;
    }
    
    // Check email uniqueness (O(1) search by hashmap)
    if (!isEmailAvailable(email)) {
        errorMsg = "Email is already registered.";
        return false;
    }
    
    // Create new user
    User newUser;
    newUser.setUsername(username);
    newUser.setEmail(email);
    
    // Encrypt password
    // Use UserHelper to encrypt password and no need for userhelper object because encryptPassword is static
    string encryptedPassword = UserHelper::encryptPassword(username, email, password);
    newUser.setPassword(encryptedPassword);
    
    // Insert user into file (maintains sorted order)
    if (!fileManager.insertUser(newUser)) {
        errorMsg = "Failed to save user. Please try again.";
        return false;
    }

    // Track email in helper hashmap for future duplicate checks
    userHelper.registerEmail(email, username);
    
    // Add username to the all_usernames vector for graph operations
    UserHelper::all_usernames.push_back(username);

    // Rebuild from scratch because Suffix Automaton doesn't support dynamic insertions
    // and the finalization fixes the structure for searching so we need to rebuild it all again
    UserHelper::sam = SuffixAutomaton();
    for (size_t i = 0; i < UserHelper::all_usernames.size(); i++) {
        UserHelper::sam.addUsername(UserHelper::all_usernames[i], static_cast<int>(i));
    }
    UserHelper::sam.finalize();

    
    // Save the updated email hashmap to binary file for persistence
    // This ensures the hashmap is saved immediately when a new user registers
    if (!fileManager.saveEmailHashmap(UserHelper::email_to_username)) {
        // Log error but don't fail registration if hashmap save fails
        // (the user is already registered, but the email hashmap persistence failed)
        std::cerr << "Warning: Failed to save email hashmap to file." << std::endl;
    }
    
    // Add the new user as an isolated vertex to the friend graph (if initialized)
    if (UserHelper::friend_graph != nullptr) {
        UserHelper::friend_graph->addUser(username);
        // Persist the friend graph so that the new vertex exists on disk
        if (!fileManager.saveFriendGraphToFile(*UserHelper::friend_graph)) {
            std::cerr << "Warning: Failed to save friend graph to file after registration." << std::endl;
        }
    }
    
    return true;
}

User Authentication::signin(const string& username, const string& password, 
                           string& errorMsg) const {
    // Validate input
    if (username.empty()) {
        errorMsg = "Username cannot be empty.";
        return User();
    }
    
    if (password.empty()) {
        errorMsg = "Password cannot be empty.";
        return User();
    }

    // First check if the given username variable is a username or email
    // The regex in the function is used to check if the username entered is actually a valid email
    string emailUsername = username;
    if(isValidEmail(username)){
        emailUsername = getUsernameByEmail(username);
        if (emailUsername.empty()) {
            errorMsg = "Invalid username or password.";
            return User();
        }
    }
    
    // Get user by username
    User user = fileManager.getUserByUsername(emailUsername);
    
    // Check if user exists
    if (user.getUsername().empty()) {
        errorMsg = "Invalid username or password.";
        return User();
    }
    
    // Verify password
    string encryptedPassword = UserHelper::encryptPassword(
        user.getUsername(), 
        user.getEmail(), 
        password
    );
    
    if (user.getPassword() != encryptedPassword) {
        errorMsg = "Invalid username or password.";
        return User();
    }
    
    // Sign-in successful
    return user;
}

bool Authentication::deleteUserAccount(const User& currentUser, const string& password, string& errorMsg) const {
    if (currentUser.getUsername().empty()) {
        errorMsg = "No signed-in user available.";
        return false;
    }

    if (password.empty()) {
        errorMsg = "Password cannot be empty.";
        return false;
    }

    string encryptedPassword = UserHelper::encryptPassword(
        currentUser.getUsername(),
        currentUser.getEmail(),
        password
    );

    if (encryptedPassword != currentUser.getPassword()) {
        errorMsg = "Password is incorrect.";
        return false;
    }

    // Delete from user storage file
    if (!fileManager.deleteUser(currentUser.getUsername())) {
        errorMsg = "Failed to delete user from storage.";
        return false;
    }

    // Remove email mapping and save hashmap
    auto it = UserHelper::email_to_username.find(currentUser.getEmail());
    if (it != UserHelper::email_to_username.end()) {
        UserHelper::email_to_username.erase(it);
    } else {
        // Fallback: remove any mapping to this username if present
        for (auto iter = UserHelper::email_to_username.begin(); iter != UserHelper::email_to_username.end(); ) {
            if (iter->second == currentUser.getUsername()) {
                iter = UserHelper::email_to_username.erase(iter);
            } else {
                ++iter;
            }
        }
    }
    if (!fileManager.saveEmailHashmap(UserHelper::email_to_username)) {
        std::cerr << "Warning: Failed to persist email hashmap after deletion." << std::endl;
    }

    // Remove username from all_usernames list and rebuild suffix automaton
    UserHelper::all_usernames.erase(
        std::remove(UserHelper::all_usernames.begin(), UserHelper::all_usernames.end(), currentUser.getUsername()),
        UserHelper::all_usernames.end()
    );
    UserHelper::sam = SuffixAutomaton();
    for (size_t i = 0; i < UserHelper::all_usernames.size(); ++i) {
        UserHelper::sam.addUsername(UserHelper::all_usernames[i], static_cast<int>(i));
    }
    UserHelper::sam.finalize();

    // Remove from friend graph and save
    if (UserHelper::friend_graph != nullptr) {
        UserHelper::friend_graph->removeUser(currentUser.getUsername());
        if (!fileManager.saveFriendGraphToFile(*UserHelper::friend_graph)) {
            std::cerr << "Warning: Failed to persist friend graph after deletion." << std::endl;
        }
    }

    return true;
}

// ============================================================================
// PUBLIC UI-INTEGRATED METHODS (Bridge between Frontend and Backend)
// ============================================================================

User Authentication::registerUserWithUI() {
    string username, email, password;
    clearScreen();
    printBoxHeader("SocioNET++ Registration");
    printCenteredLine("Please Enter Your Primary Data | Username → Email → Password");
    printBoxSeparator();

    // Regex patterns for validation
    const regex usernamePattern(R"(^(?!\d+$)(?!.*\.\.)(?!\.)(?!.*\.$)[A-Za-z0-9._]{3,30}$)");
    const regex emailPattern(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
    const regex passwordPattern(R"((?=.*\d)(?=.*[!@#$%^&*(),.?:{}|<>_\-+=~`])[A-Za-z\d!@#$%^&*(),.?:{}|<>_\-+=~`]{6,30})");

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

        if (!isUsernameAvailable(username)) {
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

        if (!isEmailAvailable(email)) {
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

    // Register user using backend signup method
    string errorMsg;
    if (signup(username, email, password, errorMsg)) {
        clearScreen();
        printBoxHeader("SocioNET++ Registration Successful");
        printPaddedLine("Username: ", username);
        printPaddedLine("Email: ", email);
        printBoxSeparator();
        printCenteredLine("Registration Successful For User: " + username);
        printBoxFooter();
        waitEnter();
        
        // Return the newly registered user by signing in
        // Password is already encrypted and saved, so we use the original password for signin
        User newUser = signin(username, password, errorMsg);
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

User Authentication::signInWithUI() {
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

    // Attempt sign in using backend signin method
    string errorMsg;
    User currentUser = signin(username, password, errorMsg);

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
                // Password reset functionality
                clearScreen();
                printBoxHeader("Password Reset");
                
                // Get user by username/email to verify identity
                string emailUsername = username;
                if (isValidEmail(username)) {
                    emailUsername = getUsernameByEmail(username);
                    if (emailUsername.empty()) {
                        printCenteredLine("Email Not Found In System");
                        printBoxFooter();
                        waitEnter();
                        break;
                    }
                }
                
                User userToReset = fileManager.getUserByUsername(emailUsername);
                if (userToReset.getUsername().empty()) {
                    printCenteredLine("User Not Found");
                    printBoxFooter();
                    waitEnter();
                    break;
                }
                
                // Verify email for password reset
                printPaddedLine("Username: ", userToReset.getUsername());
                printBoxSeparator();
                printCenteredLine("Enter Your Registered Email To Reset Password");
                printBoxSeparator();
                string verifyEmail;
                cout << "║ Email: ";
                getline(cin, verifyEmail);
                
                if (verifyEmail != userToReset.getEmail()) {
                    clearScreen();
                    printBoxHeader("Password Reset Failed");
                    printCenteredLine("Email Does Not Match Registered Email");
                    printBoxFooter();
                    waitEnter();
                    break;
                }
                
                // Get new password
                const regex passwordPattern(R"((?=.*\d)(?=.*[!@#$%^&*(),.?:{}|<>_\-+=~`])[A-Za-z\d!@#$%^&*(),.?:{}|<>_\-+=~`]{6,30})");
                string newPassword;
                while (true) {
                    clearScreen();
                    printBoxHeader("Password Reset");
                    printPaddedLine("Username: ", userToReset.getUsername());
                    printPaddedLine("Email: ", verifyEmail);
                    printBoxSeparator();
                    bool cap = true;
                    string passPrompt = "Enter New Password (6-30 chars, 1 digit, 1 special): ";
                    for (char c : passPrompt) {
                        if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
                        else { cout << c; cap = isspace(c); }
                    }
                    getline(cin, newPassword);
                    
                    if (!regex_match(newPassword, passwordPattern)) {
                        clearScreen();
                        printBoxHeader("Password Reset");
                        printPaddedLine("Username: ", userToReset.getUsername());
                        printPaddedLine("Email: ", verifyEmail);
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
                
                // Encrypt and update password
                string encryptedPassword = UserHelper::encryptPassword(
                    userToReset.getUsername(), 
                    userToReset.getEmail(), 
                    newPassword
                );
                userToReset.setPassword(encryptedPassword);
                
                // Save updated user to file
                if (fileManager.updateUser(userToReset)) {
                    clearScreen();
                    printBoxHeader("Password Reset Successful");
                    printCenteredLine("Password Updated For User: " + userToReset.getUsername());
                    printBoxFooter();
                    waitEnter();
                } else {
                    clearScreen();
                    printBoxHeader("Password Reset Failed");
                    printCenteredLine("Failed to Save New Password. Please Try Again.");
                    printBoxFooter();
                    waitEnter();
                }
                break;
            }
            else if (resetChoice == "n" || resetChoice == "N") {
                break;
            }
        }
        return User(); // Return empty user on failed sign in
    }
}

