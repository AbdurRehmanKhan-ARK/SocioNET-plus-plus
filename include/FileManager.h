// FileManager.h
// ============================================================================
// PURPOSE: Handles all file operations for user storage and retrieval
//          This class manages 27 binary files (26 for letters A-Z, 1 for other characters)
//          Users are stored sorted alphabetically by username for efficient binary search
// ============================================================================

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "User.h"
#include "Graph.h"
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::unordered_map;

class FileManager {
private:
    // Directory paths where user files are stored
    static const string DATA_DIR;           // "data/users/" - stores all user files
    static const string GRAPH_DIR;          // "data/graphs/" - stores graph structure (for future use)
    
    // Helper function to get filename based on first character of username
    // Example: 'a' or 'A' -> "data/users/users_a.bin"
    //          'z' or 'Z' -> "data/users/users_z.bin"
    //          non-alphabetic -> "data/users/users_other.bin"
    string getUserFileName(char firstChar) const;
    
    // Helper function to extract first alphabetic character from username
    // Scans through username string until it finds the first letter (case-insensitive)
    // Returns lowercase version of the first alphabetic character found
    // Returns '\0' (null character) if no alphabetic character exists
    // Example: "123john" -> 'j', "___Alice" -> 'a', "123" -> '\0'
    char getFirstAlphabeticChar(const string& username) const;

public:
    // Constructor: Initializes FileManager and ensures data directories exist
    FileManager();
    
    // Read all users from a specific binary file
    // Returns vector of User objects (empty if file doesn't exist or is empty)
    // File format: [userCount][user1][user2]...[userN]
    // Each user: [usernameLength][username][passwordLength][password][nameLength][name][emailLength][email]
    vector<User> readUserFile(const string& filename) const;
    
    // Write users to a specific binary file (overwrites existing file)
    // Maintains the same binary format as readUserFile
    // Returns true if successful, false if file cannot be opened
    bool writeUserFile(const string& filename, const vector<User>& users) const;
    
    // Binary search for username in sorted vector (O(log n) time complexity)
    // PRECONDITION: users vector must be sorted alphabetically by username
    // Returns index if found, -1 if not found
    // Uses iterative binary search algorithm
    int binarySearchUsername(const vector<User>& users, const string& username) const;
    
    // Check if username exists in the system
    // 1. Extracts first alphabetic character from username
    // 2. Determines which file to search (a-z or other)
    // 3. Loads that file and performs binary search
    // Returns true if username exists, false otherwise
    // Time Complexity: O(log n) where n is number of users in that file
    bool usernameExists(const string& username) const;
    
    // Insert user into appropriate file maintaining sorted order
    // 1. Determines correct file based on username's first alphabetic character
    // 2. Checks if username already exists (prevents duplicates)
    // 3. Finds correct insertion position to maintain alphabetical order
    // 4. Inserts user at that position
    // 5. Writes updated vector back to file
    // Returns true if insertion successful, false if username already exists or write fails
    // Uses insertion sort approach: finds position, then inserts (O(n) for finding position)
    bool insertUser(const User& user) const;
    
    // Get user by username
    // 1. Determines which file contains the user
    // 2. Performs binary search on that file
    // 3. Returns User object if found
    // Returns User with empty username if not found (can check with user.getUsername().empty())
    User getUserByUsername(const string& username) const;
    
    // Update an existing user in the file
    // 1. Determines which file contains the user
    // 2. Finds the user using binary search
    // 3. Replaces the user with updated data
    // 4. Writes updated vector back to file
    // Returns true if update successful, false if user not found or write fails
    bool updateUser(const User& updatedUser) const;

    // Delete an existing user from the user file
    // 1. Determines which file contains the user
    // 2. Removes the user from the sorted list
    // 3. Writes updated list back to file
    // Returns true if deletion successful, false if user not found or write fails
    bool deleteUser(const string& username) const;
    
    // Save email-to-username hashmap to binary file
    // File location: data/graphs/email_to_username.bin
    // Format: [mapSize] [email1_length][email1] [username1_length][username1] ...
    // Returns true if successful, false if file cannot be opened
    bool saveEmailHashmap(const std::unordered_map<std::string, std::string>& emailMap) const;
    
    // Load email-to-username hashmap from binary file
    // Returns a hashmap of email->username pairs loaded from file
    // If file doesn't exist, returns empty hashmap
    std::unordered_map<std::string, std::string> loadEmailHashmap() const;
    
    // Load email-to-username hashmap and extract all usernames into a vector
    // Returns vector of all usernames from the email hashmap
    // File location: data/graphs/email_to_username.bin
    std::vector<std::string> loadUsernameVector() const;
    
    // ========================================================================
    // FRIEND GRAPH I/O OPERATIONS
    // ========================================================================
    
    // Load friend graph from binary file
    // Populates the provided Graph object with all relationships from file
    // File location: data/graphs/friend_graph.bin
    // Returns true if successful, false if file doesn't exist or read fails
    // NOTE: On first run, file may not exist - this returns true with empty graph
    bool loadFriendGraphFromFile(Graph& graph) const;
    
    // Save friend graph to binary file
    // Serializes all relationships from the Graph object to binary file
    // File location: data/graphs/friend_graph.bin
    // Returns true if successful, false if file cannot be opened
    bool saveFriendGraphToFile(const Graph& graph) const;
};

#endif // FILEMANAGER_H

