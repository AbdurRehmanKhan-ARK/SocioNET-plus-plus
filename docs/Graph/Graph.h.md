# Graph.h

## Component Name
Graph

## Responsibilities
- Define the friend relationship graph and suggestion graph APIs.
- Declare relation metadata and runtime suggestion structures.
- Expose serialization and traversal operations for friend relationships.

## Data Structures
- `enum RelationStatus` — request or confirmed friendship.
- `struct Relation` — stores target username and relationship status.
- `unordered_map<string, vector<Relation>> adj` — adjacency list for the friend graph.
- `unordered_map<string, vector<string>> adj` in `SuggestionGraph` — temporary in-memory graph for friend suggestion sampling.
- `vector<string>` for BFS sampling and username lists.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `Graph()` | none | `Graph` | Constructs an empty graph. |
| `bool userExists(const string& username) const` | `username` | `bool` | Checks if a user node is present. |
| `void addUser(const string& username)` | `username` | `void` | Adds a user vertex with empty relations. |
| `bool sendFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Adds a directed request edge if not already connected. |
| `bool acceptFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Converts a request into a bidirectional friendship. |
| `vector<string> getFriends(const string& user) const` | `user` | `vector<string>` | Returns confirmed bidirectional friends. |
| `vector<string> getMutualFriends(const string& user1, const string& user2) const` | `user1`, `user2` | `vector<string>` | Returns users who are friends with both users. |
| `bool areFriends(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Verifies mutual confirmed friendship. |
| `bool hasSentRequest(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Checks if a pending request exists. |
| `bool hasIncomingRequest(const string& user1, const string& user2) const` | `user1`, `user2` | `bool` | Checks if `user1` has request from `user2`. |
| `bool removeFriendRequest(const string& from, const string& to)` | `from`, `to` | `bool` | Removes a pending request. |
| `bool removeUser(const string& username)` | `username` | `bool` | Deletes the node and all incident relations. |
| `bool saveToBinaryFile(const string& filepath) const` | `filepath` | `bool` | Serializes adjacency map to disk. |
| `bool loadFromBinaryFile(const string& filepath)` | `filepath` | `bool` | Deserializes graph state from disk. |
| `size_t size() const` | none | `size_t` | Returns the number of vertices. |
| `void clear()` | none | `void` | Clears the graph. |
| `const unordered_map<string, vector<Relation>>& getAdjacencyList() const` | none | adjacency reference | Returns the internal adjacency list. |
| `SuggestionGraph(const vector<string>& allUsernames)` | `allUsernames` | `SuggestionGraph` | Constructs runtime friend suggestion graph. |
| `void generateRandomConnections()` | none | `void` | Builds random undirected suggestion edges. |
| `vector<string> bfsSample(const string& start, const vector<string>& exclude, size_t limit = 40) const` | `start`, `exclude`, `limit` | `vector<string>` | Performs BFS sampling on the suggestion graph. |
| `vector<pair<string, string>> suggestFor(const string& currentUser, const vector<string>& allUsernames, const Graph* relationshipGraph, size_t topK = 5) const` | `currentUser`, `allUsernames`, `relationshipGraph`, `topK` | `vector<pair<string,string>>` | Generates scored friend recommendations. |
| `int computeSimilarity(const string& userA, const string& userB)` | `userA`, `userB` | `int` | Declares a similarity scorer. |
| `vector<pair<string, string>> getSuggestedFriendsWithReasons(...)` | `currentUser`, `allUsernames`, `relationshipGraph`, `topK` | `vector<pair<string,string>>` | Declares a high-level suggestion pipeline. |
| `void initializeSuggestionGraph()` | none | `void` | Rebuilds the runtime suggestion graph. |

## Algorithmic Complexity
- `userExists()` / `addUser()` — Time: O(1) average, Space: O(1).
- `sendFriendRequest()` / `acceptFriendRequest()` — Time: O(d) where d = degree of involved user.
- `getFriends()` — Time: O(d^2) worst-case due to verification of mutual edges, Space: O(f) for friends list.
- `getMutualFriends()` — Time: O(d1 * d2), Space: O(min(d1,d2)).
- `areFriends()` / `hasSentRequest()` / `hasIncomingRequest()` — Time: O(d).
- `removeUser()` — Time: O(V + E), Space: O(1).
- `saveToBinaryFile()` / `loadFromBinaryFile()` — Time: O(V + E), Space: O(1) auxiliary.
- `generateRandomConnections()` — Time: O(V + E), Space: O(V + E).
- `bfsSample()` — Time: O(V + E), Space: O(V).
- `suggestFor()` — Time: O(P log P) where P = sampled candidate count, Space: O(P).
- `initializeSuggestionGraph()` — Time: O(V + E), Space: O(V + E).

## Dependencies
- `Helper.h`
- `string`, `vector`, `unordered_map`
- `std::pair` for suggestions
