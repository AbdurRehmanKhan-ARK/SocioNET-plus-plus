# graph.cpp

## Component Name
Graph implementation

## Responsibilities
- Implement friend relationship graph operations.
- Persist and restore graph state to binary storage.
- Build and query runtime suggestion graphs for friend recommendations.
- Score similarity between users and generate suggestion results.

## Data Structures
- `unordered_map<string, vector<Relation>> adj` — adjacency list for friend relationships.
- `unordered_map<string, vector<string>> adj` in `SuggestionGraph` — runtime undirected suggestion graph.
- `std::queue<string>` and `unordered_set<string>` for BFS traversal.
- Local scoring helpers and `vector<string>` containers for reasons and candidates.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `Graph::Graph()` | none | `Graph` | Constructs an empty friendship graph. |
| `bool userExists(const string& username) const` | `username` | `bool` | Checks if a vertex exists. |
| `void addUser(const string& username)` | `username` | `void` | Adds a vertex with no relations. |
| `bool sendFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Adds a directed request edge if no existing link exists. |
| `bool acceptFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Converts a pending request to a bidirectional friendship. |
| `vector<string> getFriends(const string& user) const` | `user` | `vector<string>` | Returns confirmed mutual friends. |
| `vector<string> getMutualFriends(const string& user1, const string& user2) const` | `user1`, `user2` | `vector<string>` | Returns users who are friends with both. |
| `bool areFriends(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Verifies two-way confirmed friendship. |
| `bool hasSentRequest(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Checks for a pending outgoing request. |
| `bool hasIncomingRequest(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Equivalent to checking the reverse outgoing request. |
| `bool removeFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Removes a pending request edge. |
| `bool removeUser(const string& username)` | `username` | `bool` | Removes a user and all incident relations. |
| `bool saveToBinaryFile(const string& filepath) const` | `filepath` | `bool` | Serializes graph adjacency to disk. |
| `bool loadFromBinaryFile(const string& filepath)` | `filepath` | `bool` | Deserializes graph state from disk. |
| `SuggestionGraph::SuggestionGraph(const vector<string>& allUsernames)` | `allUsernames` | `SuggestionGraph` | Builds a temporary runtime graph with all usernames. |
| `void generateRandomConnections()` | none | `void` | Creates random undirected connections among suggestion nodes. |
| `vector<string> bfsSample(const string& start, const vector<string>& exclude, size_t limit) const` | `start`, `exclude`, `limit` | `vector<string>` | Performs BFS sampling while excluding a set of users. |
| `vector<pair<string, string>> suggestFor(...) const` | `currentUser`, `allUsernames`, `relationshipGraph`, `topK` | `vector<pair<string,string>>` | Scores candidate users and returns top suggestions. |
| `vector<pair<string, string>> getSuggestedFriendsWithReasons(...)` | `currentUser`, `allUsernames`, `relationshipGraph`, `topK` | `vector<pair<string,string>>` | High-level wrapper for generating suggestions. |
| `void initializeSuggestionGraph()` | none | `void` | Rebuilds the runtime suggestion graph from current usernames. |

## Algorithmic Complexity
- `userExists()` / `addUser()` — Time: O(1) average.
- `sendFriendRequest()` / `acceptFriendRequest()` — Time: O(d) for degree scan.
- `getFriends()` — Time: O(d^2) worst-case because of mutual edge verification, Space: O(f). |
- `getMutualFriends()` — Time: O(d1 * d2), Space: O(min(d1, d2)).
- `removeUser()` — Time: O(V + E), Space: O(1).
- `saveToBinaryFile()` / `loadFromBinaryFile()` — Time: O(V + E), Space: O(1).
- `generateRandomConnections()` — Time: O(V + E), Space: O(V + E).
- `bfsSample()` — Time: O(V + E), Space: O(V).
- `suggestFor()` — Time: O(S log S) where S = sampled candidate count / score evaluations, Space: O(S).
- `initializeSuggestionGraph()` — Time: O(V + E), Space: O(V + E).

## Dependencies
- `Graph.h`
- `FileManager.h`
- `Interest.h`
- `Helper.h`
- Standard library: `<fstream>`, `<algorithm>`, `<iostream>`, `<random>`, `<queue>`, `<unordered_set>`, `<cctype>`
