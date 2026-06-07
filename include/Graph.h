// Graph.h
// ============================================================================
// PURPOSE: Manages friend relationships using an undirected graph structure
//          Each vertex is a username, edges represent relationship status
//          Uses adjacency list representation with relation metadata
// ============================================================================

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
using std::string;
using std::vector;
using std::unordered_map;

// ============================================================================
// RELATION STATUS ENUM AND STRUCT
// ============================================================================

enum RelationStatus {
    REQUEST_SENT = 0,  // Friend request sent, awaiting response
    FRIEND = 1         // Confirmed friendship (bidirectional)
};

struct Relation {
    string toUser;                      // Username of the target user
    RelationStatus status;              // Status of the relationship
    
    // Constructor for convenience
    Relation(const string& user = "", RelationStatus s = REQUEST_SENT)
        : toUser(user), status(s) {}
};

// ============================================================================
// GRAPH CLASS: Manages friend relationships using adjacency list
// ============================================================================

class Graph {
private:
    // Adjacency list: maps username -> vector of relations (outgoing edges)
    // Example: adj["alice"] = [Relation("bob", FRIEND), Relation("charlie", REQUEST_SENT)]
    //          means alice has a friendship with bob and sent a request to charlie
    unordered_map<string, vector<Relation>> adj;

public:
    // ========================================================================
    // CONSTRUCTOR
    // ========================================================================
    Graph();
    
    // ========================================================================
    // USER MANAGEMENT
    // ========================================================================
    
    // Check if a user exists in the graph
    // Returns true if user has any relationships or has been added explicitly
    bool userExists(const string& username) const;
    
    // Add a new user to the graph (creates empty adjacency list)
    // If user already exists, does nothing
    void addUser(const string& username);
    
    // ========================================================================
    // FRIEND REQUEST OPERATIONS
    // ========================================================================
    
    // Send a friend request from one user to another
    // Creates edge: from → to with status REQUEST_SENT
    // Returns true if request sent successfully
    // Returns false if:
    //   - Users are already friends
    //   - Request already sent
    //   - User doesn't exist
    // NOTE: Does NOT create reverse edge until request is accepted
    bool sendFriendRequest(const string& from, const string& to);
    
    // Accept a friend request
    // Updates edge: from → to with status FRIEND
    // Creates new edge: to → from with status FRIEND (bidirectional friendship)
    // Returns true if request accepted successfully
    // Returns false if:
    //   - No request exists from 'from' to 'to'
    //   - They are already friends
    //   - User doesn't exist
    bool acceptFriendRequest(const string& from, const string& to);
    
    // ========================================================================
    // RELATIONSHIP QUERY OPERATIONS
    // ========================================================================
    
    // Get all confirmed friends of a user
    // Returns vector of usernames where relationship is FRIEND (bidirectional)
    // Time Complexity: O(n) where n is degree of user
    vector<string> getFriends(const string& user) const;
    
    // Get all users who have mutual friendships with both user1 and user2
    // Returns vector of usernames who are friends with both users
    // Time Complexity: O(n + m) where n, m are degrees of the two users
    vector<string> getMutualFriends(const string& user1, const string& user2) const;
    
    // Check if a specific friendship exists (bidirectional)
    // Returns true only if both users have FRIEND status edges to each other
    bool areFriends(const string& user1, const string& user2) const;
    
    // Check if user1 sent a request to user2
    // Returns true if edge user1 → user2 exists with status REQUEST_SENT
    bool hasSentRequest(const string& user1, const string& user2) const;
    
    // Check if user1 has received a request from user2
    // Equivalent to: user2 sent request to user1
    // Returns true if edge user2 → user1 exists with status REQUEST_SENT
    bool hasIncomingRequest(const string& user1, const string& user2) const;
    
    // Remove a pending friend request from 'from' -> 'to'
    // Returns true if a REQUEST_SENT edge existed and was removed
    bool removeFriendRequest(const string& from, const string& to);

    // Remove a user from the graph, including all incoming and outgoing relations
    // Returns true if user existed and was removed, false otherwise
    bool removeUser(const string& username);
    // ========================================================================
    // FILE I/O OPERATIONS
    // ========================================================================
    
    // Serialize the entire graph to a binary file
    // File format:
    //   [userCount]
    //   For each user:
    //     [username_length][username]
    //     [relation_count]
    //     For each relation:
    //       [toUser_length][toUser]
    //       [status (int)]
    // Returns true if successful, false on file error
    bool saveToBinaryFile(const string& filepath) const;
    
    // Deserialize the entire graph from a binary file
    // Restores the exact adjacency list from saved state
    // Returns true if successful, false on file error or missing file
    bool loadFromBinaryFile(const string& filepath);
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    
    // Get size of adjacency list (number of users with relationships)
    size_t size() const { return adj.size(); }
    
    // Clear all relationships
    void clear() { adj.clear(); }
    
    // Get reference to adjacency list (for advanced operations)
    const unordered_map<string, vector<Relation>>& getAdjacencyList() const {
        return adj;
    }
};

// ============================================================================
// SUGGESTION GRAPH (RUNTIME ONLY)
// ============================================================================
// Lightweight in-memory graph used to generate random reachable samples among
// all usernames for friend suggestion purposes. This graph is NOT persisted.
// ============================================================================
class SuggestionGraph {
private:
    // adjacency list for the temporary graph (undirected)
    unordered_map<string, vector<string>> adj;
public:
    // Construct from a list of all usernames
    SuggestionGraph(const vector<string>& allUsernames);

    // Generate random connections between nodes. Each node will have a
    // randomized degree in [2,4]. This function mutates internal adj.
    // Time Complexity: O(n + m) where n = |V|, m = total edges created
    void generateRandomConnections();

    // BFS sampling starting from `start`. Returns up to `limit` unique
    // reachable usernames (excluding the start), following graph edges.
    // Excludes any username present in the `exclude` set.
    // Time Complexity: O(V + E) for traversal
    vector<string> bfsSample(const string& start, const vector<string>& exclude, size_t limit = 40) const;

    // Suggest friends for `currentUser` using the hybrid algorithm described
    // in the project notes. This runs entirely in-memory and returns up to
    // `topK` usernames sorted by descending similarity score.
    // Uses `relationshipGraph` to filter out existing friends / requests.
    vector<std::pair<string, string>> suggestFor(const string& currentUser,
                               const vector<string>& allUsernames,
                               const Graph* relationshipGraph,
                               size_t topK = 5) const;
};

// Compute a simple integer similarity score between two users. Higher is
// better. This inspects personal fields and recent posts. Returns 0..100
// approximately. Time Complexity: O(P) where P is total posts inspected.
int computeSimilarity(const string& userA, const string& userB);

// ============================================================================
// HIGH-LEVEL SUGGESTION API
// ============================================================================
// Wrapper function that encapsulates the entire suggestion pipeline:
// - Builds SuggestionGraph
// - Generates random connections
// - Performs BFS sampling
// - Scores and ranks candidates
// Returns vector of pairs: (username, reason_string) for the top suggestions
// Time Complexity: O(n + m + P*log(P)) where n=users, m=edges, P=posts
vector<std::pair<string, string>> getSuggestedFriendsWithReasons(
    const string& currentUser,
    const vector<string>& allUsernames,
    const Graph* relationshipGraph,
    size_t topK = 5
);

// ============================================================================
// BACKEND INITIALIZATION FUNCTION
// ============================================================================
// Initialize the suggestion graph from scratch using all_usernames.
// This function builds a fresh random graph each time and caches it.
// Call this before retrieving suggestions to ensure fresh recommendations.
// Safe to call multiple times (rebuilds the graph each call).
void initializeSuggestionGraph();

#endif // GRAPH_H
