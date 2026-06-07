// fileManager.cpp
// ============================================================================
// Implementation of FileManager class for handling user file operations
// ============================================================================

#include "../include/FileManager.h"
#include <fstream>      // For file input/output operations (ifstream, ofstream)
#include <algorithm>    // For STL algorithms (not currently used, but useful for future)
#include <cctype>       // For character classification functions (isalpha, tolower)
#include <iostream>     // For input/output streams (cout, cerr)

// ============================================================================
// PLATFORM-SPECIFIC DIRECTORY CREATION
// ============================================================================
// Different operating systems use different functions to create directories:
//
// WINDOWS (_WIN32):
//   - Uses <direct.h> header which provides _mkdir() function
//   - _mkdir() takes only the path (no permission mode needed)
//   - We create a macro to map mkdir() to _mkdir() for consistency
//
// UNIX/LINUX/MAC (POSIX systems):
//   - Uses <sys/stat.h> and <sys/types.h> headers
//   - Provides mkdir() function that takes path and permission mode
//   - Permission mode 0755 means: owner can read/write/execute, 
//     group and others can read/execute (common for directories)
//
// The #ifdef preprocessor directive checks if we're compiling on Windows
// and includes the appropriate headers and defines the appropriate function
// ============================================================================
#ifdef _WIN32
    // Windows-specific: Include header for directory creation
    #include <direct.h>
    // Create a macro so we can use mkdir() consistently on both platforms
    // This maps mkdir(path, mode) calls to Windows' _mkdir(path)
    #define mkdir(path, mode) _mkdir(path)
#else
    // Unix/Linux/Mac: Include headers for directory creation
    #include <sys/stat.h>   // Provides mkdir() function and stat structure
    #include <sys/types.h>  // Provides mode_t type for file permissions
#endif

// ============================================================================
// STATIC MEMBER INITIALIZATION
// ============================================================================
// These constants define the directory paths where files are stored
// They are static members, so they're shared across all FileManager instances
const string FileManager::DATA_DIR = "data/users/";   // Directory for user files
const string FileManager::GRAPH_DIR = "data/graphs/";  // Directory for graph files (future use)

// ============================================================================
// CONSTRUCTOR
// ============================================================================
FileManager::FileManager() {
    // Ensure data directories exist when FileManager is created
    // This prevents errors when trying to write files to non-existent directories
    // If directories already exist, these calls will fail silently (which is fine)
    
    #ifdef _WIN32
        // Windows: Use _mkdir() which only needs the path
        // Creates "data" directory in current working directory
        _mkdir("data");
        // Creates "data/users" subdirectory
        _mkdir("data/users");
        // Creates "data/graphs" subdirectory
        _mkdir("data/graphs");
    #else
        // Unix/Linux/Mac: Use mkdir() with permission mode
        // 0755 = rwxr-xr-x (owner: read/write/execute, others: read/execute)
        // Creates "data" directory with read/write/execute permissions
        mkdir("data", 0755);
        // Creates "data/users" subdirectory
        mkdir("data/users", 0755);
        // Creates "data/graphs" subdirectory
        mkdir("data/graphs", 0755);
    #endif
    // Note: If directories already exist, mkdir/_mkdir will return an error,
    // but we ignore it since the directory existing is what we want anyway
}

// ============================================================================
// HELPER FUNCTION: getUserFileName
// ============================================================================
// Converts a character to the corresponding filename
// Input: firstChar - the first alphabetic character of a username (or '?' for non-alphabetic)
// Output: Full path to the binary file where users with this starting character are stored
// Examples:
//   'a' or 'A' -> "data/users/users_a.bin"
//   'z' or 'Z' -> "data/users/users_z.bin"
//   '?' or any non-letter -> "data/users/users_other.bin"
string FileManager::getUserFileName(char firstChar) const {
    if (isalpha(firstChar)) {
        // If it's a letter, convert to lowercase and create filename
        char lowerChar = tolower(firstChar);
        // string(1, lowerChar) creates a string with one character
        return DATA_DIR + "users_" + string(1, lowerChar) + ".bin";
    } else {
        // Non-alphabetic characters go to the "other" file
        return DATA_DIR + "users_other.bin";
    }
}

// ============================================================================
// HELPER FUNCTION: getFirstAlphabeticChar
// ============================================================================
// Scans through username string to find the first alphabetic character
// This handles usernames like "123john", "__Alice", "007bond", etc.
// Input: username - the username string to scan
// Output: Lowercase version of first alphabetic character, or '\0' if none found
// Examples:
//   "123john" -> 'j'
//   "__Alice" -> 'a'
//   "007bond" -> 'b'
//   "123" -> '\0' (null character, meaning no letter found)
char FileManager::getFirstAlphabeticChar(const string& username) const {
    // Iterate through each character in the username
    for (char c : username) {
        if (isalpha(c)) {  // Check if character is a letter (a-z, A-Z)
            return tolower(c);  // Return lowercase version for consistency
        }
    }
    return '\0'; // No alphabetic character found - return null character
}

// ============================================================================
// FUNCTION: readUserFile
// ============================================================================
// Reads all User objects from a binary file
// Binary File Format:
//   [userCount (size_t)] [User1] [User2] ... [UserN]
//   Each User: [usernameLength (size_t)] [username (char array)]
//             [passwordLength (size_t)] [password (char array)]
//             [nameLength (size_t)] [name (char array)]
//             [emailLength (size_t)] [email (char array)]
// 
// Why binary format?
//   - More efficient than text (no parsing needed)
//   - Preserves exact data (no formatting issues)
//   - Faster read/write operations
// ============================================================================
vector<User> FileManager::readUserFile(const string& filename) const {
    vector<User> users;  // Vector to store all users from file
    // Open file in binary mode (ios::binary flag is crucial for binary I/O)
    ifstream file(filename, ios::binary);
    
    // Check if file opened successfully
    if (!file.is_open()) {
        // File doesn't exist yet (first time running, no users registered)
        // Return empty vector - this is not an error, just means no users yet
        return users;
    }
    
    // STEP 1: Read the number of users stored in this file
    // This tells us how many User objects to read
    size_t userCount = 0;
    // reinterpret_cast converts the address of userCount to char* so we can read binary data
    // sizeof(userCount) tells us how many bytes to read (typically 8 bytes for size_t)
    file.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
    
    // STEP 2: Read each user from the file
    for (size_t i = 0; i < userCount; ++i) {
        User user;  // Create and fully deserialize the User object from the stream
        user.deserialize(file);
        // Add the complete user to our vector
        users.push_back(user);
    }
    
    // Close the file when done reading
    file.close();
    return users;  // Return vector of all users from this file
}

// ============================================================================
// FUNCTION: writeUserFile
// ============================================================================
// Writes all User objects to a binary file
// Uses the same binary format as readUserFile (see comments above)
// ios::trunc means "truncate" - if file exists, it will be completely overwritten
// ============================================================================
bool FileManager::writeUserFile(const string& filename, const vector<User>& users) const {
    // Open file in binary mode for writing
    // ios::trunc ensures file is cleared if it already exists (overwrite mode)
    ofstream file(filename, ios::binary | ios::trunc);
    
    // Check if file opened successfully
    if (!file.is_open()) {
        return false;  // Failed to open file (permissions issue, disk full, etc.)
    }
    
    // STEP 1: Write the number of users first
    // This allows readUserFile to know how many users to read
    size_t userCount = users.size();
    file.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
    
    // STEP 2: Write each user to the file
    for (const User& user : users) {
        // Delegate full serialization to User
        user.serialize(file);
    }
    
    // Close file and flush any buffered data to disk
    file.close();
    return true;  // Successfully wrote all users to file
}

// ============================================================================
// FUNCTION: binarySearchUsername
// ============================================================================
// Performs binary search on a sorted vector of users to find a username
// Time Complexity: O(log n) - very efficient for large datasets
// 
// HOW BINARY SEARCH WORKS:
// 1. Start with the entire sorted array
// 2. Compare target with middle element
// 3. If equal, found! If target is smaller, search left half. If larger, search right half
// 4. Repeat until found or search space is empty
// 
// PRECONDITION: users vector MUST be sorted alphabetically by username
// ============================================================================
int FileManager::binarySearchUsername(const vector<User>& users, const string& username) const {
    // Initialize search boundaries
    int left = 0;                           // Left boundary (start of array)
    int right = users.size() - 1;           // Right boundary (end of array)
    
    // Continue searching while there's a valid search space
    while (left <= right) {
        // Calculate middle index (using this formula prevents integer overflow)
        // (left + right) / 2 could overflow for very large arrays
        int mid = left + (right - left) / 2;
        string midUsername = users[mid].getUsername();
        
        // Compare target username with middle element
        if (midUsername == username) {
            return mid;  // Found! Return the index
        } else if (midUsername < username) {
            // Target is in the right half (username comes after midUsername alphabetically)
            left = mid + 1;  // Move left boundary to right of middle
        } else {
            // Target is in the left half (username comes before midUsername alphabetically)
            right = mid - 1;  // Move right boundary to left of middle
        }
    }
    
    return -1;  // Not found - username doesn't exist in this file
}

// ============================================================================
// FUNCTION: usernameExists
// ============================================================================
// Checks if a username already exists in the system
// Uses efficient binary search on the appropriate file
// Time Complexity: O(log n) where n is number of users in that specific file
// ============================================================================
bool FileManager::usernameExists(const string& username) const {
    // Step 1: Find the first alphabetic character in username
    // This determines which file to search
    char firstChar = getFirstAlphabeticChar(username);
    
    // Step 2: If no alphabetic character found, use 'other' file
    if (firstChar == '\0') {
        firstChar = '?';  // '?' is used as marker for non-alphabetic usernames
    }
    
    // Step 3: Get the filename for this character
    // Example: 'a' -> "data/users/users_a.bin"
    string filename = getUserFileName(firstChar);
    
    // Step 4: Read all users from that file
    vector<User> users = readUserFile(filename);
    
    // Step 5: Perform binary search to check if username exists
    // Returns true if index is not -1 (meaning username was found)
    return binarySearchUsername(users, username) != -1;
}

// ============================================================================
// FUNCTION: insertUser
// ============================================================================
// Inserts a new user into the appropriate file while maintaining sorted order
// This is crucial for binary search to work correctly
// Uses insertion sort approach: finds correct position, then inserts
// Time Complexity: O(n) for finding position + O(n) for insertion = O(n)
// ============================================================================
bool FileManager::insertUser(const User& user) const {
    string username = user.getUsername();
    
    // Step 1: Determine which file this user belongs to
    char firstChar = getFirstAlphabeticChar(username);
    if (firstChar == '\0') {
        firstChar = '?';  // Non-alphabetic usernames go to 'other' file
    }
    
    // Step 2: Get the filename and load existing users from that file
    string filename = getUserFileName(firstChar);
    vector<User> users = readUserFile(filename);
    
    // Step 3: Check if username already exists (prevent duplicates)
    if (binarySearchUsername(users, username) != -1) {
        return false;  // Username already exists - cannot insert duplicate
    }
    
    // Step 4: Find the correct insertion position to maintain alphabetical order
    // We scan through the sorted vector until we find where this username should go
    int insertPos = 0;
    for (size_t i = 0; i < users.size(); ++i) {
        // If current username is less than new username, new one goes after it
        if (users[i].getUsername() < username) {
            insertPos = i + 1;  // Position is after current element
        } else {
            // Found a username that's >= new username, so insert before it
            break;  // Stop searching - found the right position
        }
    }
    // After loop, insertPos points to where new user should be inserted
    
    // Step 5: Insert user at the correct position
    // This maintains the sorted order, allowing binary search to work
    users.insert(users.begin() + insertPos, user);
    
    // Step 6: Write the updated vector back to file
    // This overwrites the old file with the new sorted list including the new user
    return writeUserFile(filename, users);
}

// ============================================================================
// FUNCTION: getUserByUsername
// ============================================================================
// Retrieves a User object by their username
// Uses binary search for efficient lookup (O(log n))
// Returns User with empty username if not found (check with user.getUsername().empty())
// ============================================================================
User FileManager::getUserByUsername(const string& username) const {
    // Step 1: Determine which file contains this user
    char firstChar = getFirstAlphabeticChar(username);
    if (firstChar == '\0') {
        firstChar = '?';  // Non-alphabetic -> 'other' file
    }
    
    // Step 2: Get filename and load users from that file
    string filename = getUserFileName(firstChar);
    vector<User> users = readUserFile(filename);
    
    // Step 3: Perform binary search to find the user
    int index = binarySearchUsername(users, username);
    if (index != -1) {
        return users[index];  // Found! Return the User object
    }
    
    // Step 4: Not found - return empty User object
    // Caller can check if username is empty to determine if user was found
    return User();
}

// ============================================================================
// FUNCTION: updateUser
// ============================================================================
// Updates an existing user in the file system
// Finds the user, replaces it with updated data, and saves back to file
// Time Complexity: O(log n) for search + O(n) for write = O(n)
// ============================================================================
bool FileManager::updateUser(const User& updatedUser) const {
    string username = updatedUser.getUsername();
    
    // Step 1: Determine which file contains this user
    char firstChar = getFirstAlphabeticChar(username);
    if (firstChar == '\0') {
        firstChar = '?';  // Non-alphabetic usernames go to 'other' file
    }
    
    // Step 2: Get the filename and load existing users from that file
    string filename = getUserFileName(firstChar);
    vector<User> users = readUserFile(filename);
    
    // Step 3: Find the user using binary search
    int index = binarySearchUsername(users, username);
    if (index == -1) {
        return false;  // User not found - cannot update
    }
    
    // Step 4: Replace the user at the found index with updated data
    users[index] = updatedUser;
    
    // Step 5: Write the updated vector back to file
    return writeUserFile(filename, users);
}

// ============================================================================
// FUNCTION: deleteUser
// Deletes a user from the appropriate sorted user file.
// Maintains sorted order for remaining users.
// ============================================================================
bool FileManager::deleteUser(const string& username) const {
    char firstChar = getFirstAlphabeticChar(username);
    if (firstChar == '\0') {
        firstChar = '?';
    }

    string filename = getUserFileName(firstChar);
    vector<User> users = readUserFile(filename);
    int index = binarySearchUsername(users, username);
    if (index == -1) {
        return false; // User not found
    }

    users.erase(users.begin() + index);
    return writeUserFile(filename, users);
}

// ============================================================================
// FUNCTION: saveEmailHashmap
// ============================================================================
// Saves email-to-username hashmap to a binary file in data/graphs/
// Binary File Format:
//   [mapSize (size_t)] 
//   [email1_length (size_t)][email1 (char array)]
//   [username1_length (size_t)][username1 (char array)]
//   [email2_length (size_t)][email2 (char array)]
//   [username2_length (size_t)][username2 (char array)]
//   ...
// ============================================================================
bool FileManager::saveEmailHashmap(const unordered_map<string, string>& emailMap) const {
    // Construct the full path to the email hashmap file
    string hashMapFile = GRAPH_DIR + "email_to_username.bin";
    
    // Open file in binary mode for writing
    // ios::trunc ensures file is cleared if it already exists (overwrite mode)
    ofstream file(hashMapFile, ios::binary | ios::trunc);
    
    // Check if file opened successfully
    if (!file.is_open()) {
        return false;  // Failed to open file
    }
    
    // STEP 1: Write the size of the hashmap (number of entries)
    size_t mapSize = emailMap.size();
    file.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    
    // STEP 2: Write each email-username pair
    for (const auto& pair : emailMap) {
        const string& email = pair.first;
        const string& username = pair.second;
        
        // Write email length
        size_t emailLength = email.length();
        file.write(reinterpret_cast<const char*>(&emailLength), sizeof(emailLength));
        
        // Write email data
        file.write(email.c_str(), emailLength);
        
        // Write username length
        size_t usernameLength = username.length();
        file.write(reinterpret_cast<const char*>(&usernameLength), sizeof(usernameLength));
        
        // Write username data
        file.write(username.c_str(), usernameLength);
    }
    
    // Close file and flush any buffered data to disk
    file.close();
    return true;  // Successfully saved hashmap
}

// ============================================================================
// FUNCTION: loadEmailHashmap
// ============================================================================
// Loads email-to-username hashmap from binary file
// Uses the same binary format as saveEmailHashmap
// Returns empty hashmap if file doesn't exist
// Time Complexity: O(n) where n is number of email entries
// ============================================================================
unordered_map<string, string> FileManager::loadEmailHashmap() const {
    unordered_map<string, string> emailMap;
    
    // Construct the full path to the email hashmap file
    string hashMapFile = GRAPH_DIR + "email_to_username.bin";
    
    // Open file in binary mode for reading
    ifstream file(hashMapFile, ios::binary);
    
    // Check if file opened successfully
    if (!file.is_open()) {
        // File doesn't exist yet (first time running or no users registered yet)
        return emailMap;  // Return empty hashmap
    }
    
    // STEP 1: Read the number of entries in the hashmap
    size_t mapSize = 0;
    file.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    
    // STEP 2: Read each email-username pair
    for (size_t i = 0; i < mapSize; ++i) {
        // Read email length
        size_t emailLength = 0;
        file.read(reinterpret_cast<char*>(&emailLength), sizeof(emailLength));
        
        // Read email data
        string email(emailLength, '\0');
        file.read(&email[0], emailLength);
        
        // Read username length
        size_t usernameLength = 0;
        file.read(reinterpret_cast<char*>(&usernameLength), sizeof(usernameLength));
        
        // Read username data
        string username(usernameLength, '\0');
        file.read(&username[0], usernameLength);
        
        // Insert into hashmap
        emailMap[email] = username;
    }
    
    // Close file
    file.close();
    return emailMap;  // Return populated hashmap
}

// ============================================================================
// FUNCTION: loadUsernameVector
// ============================================================================
// Loads the email-to-username hashmap from file and extracts all usernames
// into a vector. This is useful for graph operations that need all usernames.
// Returns vector of all unique usernames from the email hashmap
// Time Complexity: O(n) where n is number of email entries
// ============================================================================
vector<string> FileManager::loadUsernameVector() const {
    vector<string> usernames;
    
    // Load the email hashmap from file
    unordered_map<string, string> emailMap = loadEmailHashmap();
    
    // Extract all usernames (values) from the hashmap
    // Reserve space if map is non-empty to avoid unnecessary reallocations
    if (!emailMap.empty()) {
        usernames.reserve(emailMap.size());
    }
    
    // Iterate through hashmap and collect all usernames
    for (const auto& pair : emailMap) {
        usernames.push_back(pair.second);
    }
    
    return usernames;  // Return vector of all usernames
}

// ============================================================================
// FUNCTION: loadFriendGraphFromFile
// ============================================================================
// Loads the friend graph from a binary file
// Delegates to Graph::loadFromBinaryFile for actual deserialization
// File location: data/graphs/friend_graph.bin
// Returns true if successful or file doesn't exist (first run), false on error
// ============================================================================
bool FileManager::loadFriendGraphFromFile(Graph& graph) const {
    string graphFile = GRAPH_DIR + "friend_graph.bin";
    return graph.loadFromBinaryFile(graphFile);
}

// ============================================================================
// FUNCTION: saveFriendGraphToFile
// ============================================================================
// Saves the friend graph to a binary file
// Delegates to Graph::saveToBinaryFile for actual serialization
// File location: data/graphs/friend_graph.bin
// Returns true if successful, false if file cannot be opened
// ============================================================================
bool FileManager::saveFriendGraphToFile(const Graph& graph) const {
    string graphFile = GRAPH_DIR + "friend_graph.bin";
    return graph.saveToBinaryFile(graphFile);
}