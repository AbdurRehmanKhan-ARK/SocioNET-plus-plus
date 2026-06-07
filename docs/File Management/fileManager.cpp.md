# fileManager.cpp

## Component Name
FileManager implementation

## Responsibilities
- Implement binary serialization and deserialization for users and graph state.
- Manage directory creation for data persistence.
- Provide helper functions to locate bucket files and process sorted user files.

## Data Structures
- `std::unordered_map<string, string>` for loaded email hash maps.
- `std::vector<User>` for loaded file contents.
- Platform-specific directory handling via `_mkdir` on Windows and `mkdir` on POSIX.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `FileManager::FileManager()` | none | `FileManager` | Creates `data/`, `data/users/`, and `data/graphs/` directories if missing. |
| `string getUserFileName(char firstChar) const` | `firstChar` | `string` | Maps a username initial to the storage filename. |
| `char getFirstAlphabeticChar(const string& username) const` | `username` | `char` | Extracts the first alphabetic character from a username. |
| `vector<User> readUserFile(const string& filename) const` | `filename` | `vector<User>` | Reads binary user file and deserializes each `User`. |
| `bool writeUserFile(const string& filename, const vector<User>& users) const` | `filename`, `users` | `bool` | Writes user count and serialized records to disk. |
| `int binarySearchUsername(const vector<User>& users, const string& username) const` | `users`, `username` | `int` | Performs sorted binary search on users. |
| `bool usernameExists(const string& username) const` | `username` | `bool` | Loads the correct bucket and searches for a username. |
| `bool insertUser(const User& user) const` | `user` | `bool` | Inserts a new user into sorted file and rewrites that file. |
| `User getUserByUsername(const string& username) const` | `username` | `User` | Loads a single user by searching the appropriate file. |
| `bool updateUser(const User& updatedUser) const` | `updatedUser` | `bool` | Replaces an existing user in its bucket and rewrites the file. |
| `bool deleteUser(const string& username) const` | `username` | `bool` | Removes a user from storage and rewrites the file. |
| `bool saveEmailHashmap(const unordered_map<string, string>& emailMap) const` | `emailMap` | `bool` | Serializes the email lookup map into a binary file. |
| `unordered_map<string, string> loadEmailHashmap() const` | none | `unordered_map<string, string>` | Deserializes the stored email lookup map. |
| `vector<string> loadUsernameVector() const` | none | `vector<string>` | Returns all usernames extracted from the email map. |
| `bool loadFriendGraphFromFile(Graph& graph) const` | `graph` | `bool` | Delegates graph load from `friend_graph.bin`. |
| `bool saveFriendGraphToFile(const Graph& graph) const` | `graph` | `bool` | Delegates graph save to `friend_graph.bin`. |

## Algorithmic Complexity
- `getUserFileName()` / `getFirstAlphabeticChar()` — Time: O(L), Space: O(1).
- `readUserFile()` / `writeUserFile()` — Time: O(N) and Space: O(N) for N users in a bucket.
- `insertUser()` / `updateUser()` / `deleteUser()` — Time: O(N) due to vector insertion/deletion and rewrite, Space: O(N).
- `saveEmailHashmap()` / `loadEmailHashmap()` — Time/Space: O(E).
- `loadUsernameVector()` — Time/Space: O(E).

## Dependencies
- `FileManager.h`
- `User.h`
- `Graph.h`
- Standard library: `<fstream>`, `<algorithm>`, `<cctype>`, `<iostream>`
