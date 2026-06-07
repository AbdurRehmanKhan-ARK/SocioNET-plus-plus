// Authentication.h
// Handles user registration and sign-in functionality
// Serves as bridge between frontend UI and backend logic

#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include "User.h"
#include "FileManager.h"
#include "Helper.h"
#include "UIHelpers.h"
using std::string;

class Authentication {
private:
    FileManager fileManager;
    UserHelper userHelper;
    
    // Helper function to trim username and find first alphabetic character
    char getFirstAlphabeticChar(const string& username) const;
    
    // Validate email format (basic validation)
    bool isValidEmail(const string& email) const;
    
    // Validate password strength (basic validation)
    bool isValidPassword(const string& password) const;

    // Validate username format (basic validation)
    bool isValidUsername(const string& username) const;
    
    public:
    // Backend registration logic (called by registerUserWithUI)
    bool signup(const string& username, const string& email, 
                const string& password, string& errorMsg) const;
    
    // Backend signin logic (called by signInWithUI)
    User signin(const string& username, const string& password, 
                string& errorMsg) const;

public:
    Authentication();
    
    // Load email-to-username hashmap and username vector from binary file
    // This should be called once at program startup before any user operations
    // Loads from: data/graphs/email_to_username.bin
    // Returns true if load successful or file doesn't exist yet, false on error
    bool initializeEmailHashmap();
    
    // ========================================================================
    // PUBLIC UI-INTEGRATED METHODS (Bridge between Frontend and Backend)
    // ========================================================================
    
    // Complete registration flow with UI
    // Handles all UI interactions, validation, and user creation
    // Returns User object if successful, User with empty username if failed
    // Manages encrypted passwords and email tracking
    User registerUserWithUI();
    
    // Complete sign-in flow with UI
    // Handles all UI interactions, validation, and authentication
    // Returns User object if successful, User with empty username if failed
    // Supports login by username or email
    User signInWithUI();
    
    // ========================================================================
    // VALIDATION AND HELPER METHODS (Used by UI methods)
    // ========================================================================
    
    // Check if username is available
    bool isUsernameAvailable(const string& username) const;
    
    // Check if email is available
    bool isEmailAvailable(const string& email) const;

    // Get username by email (Help in login by either username or email)
    string getUsernameByEmail(const string& email) const;

    // Delete the currently signed in user account after password confirmation
    // Returns true if deletion succeeded, false otherwise. errorMsg contains details.
    bool deleteUserAccount(const User& currentUser, const string& password, string& errorMsg) const;
};

#endif // AUTHENTICATION_H

