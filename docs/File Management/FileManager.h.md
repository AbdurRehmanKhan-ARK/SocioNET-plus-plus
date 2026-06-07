# FileManager.h

## Component Name
FileManager

## Responsibilities
- Define file-based persistence for user records, email lookup, and friend-graph storage.
- Provide lookup, insertion, update, and deletion operations for user records.
- Expose serialization interfaces for graph-related binary files.

## Data Structures
- `static const string DATA_DIR` — base user data directory.
- `static const string GRAPH_DIR` — base graph storage directory.
- `std::string` file paths and filenames.
- `std::vector<User>` for in-memory user batches.
- `std::unordered_map<std::string, std::string>` for email-to-username mapping.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `FileManager()` | none | `FileManager` | Ensures user and graph directories exist. |
| `vector<User> readUserFile(const string& filename) const` | `filename` | `vector<User>` | Reads a sorted binary user file and returns users. |
| `bool writeUserFile(const string& filename, const vector<User>& users) const` | `filename`, `users` | `bool` | Writes a user vector to binary file. |
| `int binarySearchUsername(const vector<User>& users, const string& username) const` | `users`, `username` | `int` | Sorted binary search for a username. |
| `bool usernameExists(const string& username) const` | `username` | `bool` | Determines if a username exists in the appropriate file. |
| `bool insertUser(const User& user) const` | `user` | `bool` | Inserts a new user in sorted order into the correct bucket file. |
| `User getUserByUsername(const string& username) const` | `username` | `User` | Loads the user record by username. |
| `bool updateUser(const User& updatedUser) const` | `updatedUser` | `bool` | Rewrites a user record within its bucket file. |
| `bool deleteUser(const string& username) const` | `username` | `bool` | Removes a username from storage and rewrites the file. |
| `bool saveEmailHashmap(const unordered_map<string, string>& emailMap) const` | `emailMap` | `bool` | Saves email lookup state in binary form. |
| `unordered_map<string, string> loadEmailHashmap() const` | none | `unordered_map<string, string>` | Loads email lookup map from binary file. |
| `vector<string> loadUsernameVector() const` | none | `vector<string>` | Extracts usernames from loaded email lookup state. |
| `bool loadFriendGraphFromFile(Graph& graph) const` | `graph` | `bool` | Loads persisted friend graph into memory. |
| `bool saveFriendGraphToFile(const Graph& graph) const` | `graph` | `bool` | Persists the graph adjacency state to disk. |

## Algorithmic Complexity
- `binarySearchUsername()` — Time: O(log N), Space: O(1).
- `usernameExists()` — Time: O(N) file read + O(log N) search; Space: O(N) for loaded file.
- `insertUser()` — Time: O(N) insertion into sorted vector and rewrite; Space: O(N).
- `updateUser()` / `deleteUser()` — Time: O(N) due to file rewrite; Space: O(N).
- `saveEmailHashmap()` / `loadEmailHashmap()` — Time: O(E), Space: O(E) where E = number of email entries.
- `loadUsernameVector()` — Time: O(E), Space: O(E).

## Dependencies
- `User.h`
- `Graph.h`
- Standard library: `<string>`, `<vector>`, `<unordered_map>`
