// graph.cpp
// ============================================================================
// Implementation of Graph class for managing friend relationships
// ============================================================================

#include "../include/Graph.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
#include <unordered_set>
#include <cctype>

#include "../include/FileManager.h"
#include "../include/Interest.h"
#include "../include/Helper.h"

using std::ifstream;
using std::ofstream;
using std::ios;
using std::cerr;
using std::endl;

// ============================================================================
// CONSTRUCTOR
// ============================================================================
Graph::Graph() {
    // Initialize empty adjacency list
    // Users are added dynamically as relationships are created
}

// ============================================================================
// USER MANAGEMENT METHODS
// ============================================================================

// ============================================================================
// FUNCTION: userExists
// ============================================================================
// Checks if a user exists in the graph
// A user exists if they have an entry in the adjacency list
// (either through sending/receiving requests or being friends)
// Time Complexity: O(1) average case (unordered_map lookup)
// ============================================================================
bool Graph::userExists(const string& username) const {
    return adj.find(username) != adj.end();
}

// ============================================================================
// FUNCTION: addUser
// ============================================================================
// Adds a new user to the graph with empty adjacency list
// If user already exists, does nothing
// Time Complexity: O(1) average case
// ============================================================================
void Graph::addUser(const string& username) {
    if (!userExists(username)) {
        adj[username] = vector<Relation>();  // Create empty adjacency list
    }
}

// ============================================================================
// FRIEND REQUEST OPERATIONS
// ============================================================================

// ============================================================================
// FUNCTION: sendFriendRequest
// ============================================================================
// Send a friend request from 'from' user to 'to' user
// Logic:
// 1. Check if users exist (if not, create them)
// 2. Check if already friends (return false)
// 3. Check if request already sent (return false)
// 4. Add edge: from → to with status REQUEST_SENT
// 5. Return true
//
// Time Complexity: O(d) where d is degree of 'from' user
// ============================================================================
bool Graph::sendFriendRequest(const string& from, const string& to) {
    // Ensure both users exist in the graph
    addUser(from);
    addUser(to);
    
    // Check if they are already friends
    if (areFriends(from, to)) {
        return false;  // Already friends, cannot send request
    }
    
    // Check if request already sent
    auto& fromRelations = adj[from];
    for (const Relation& rel : fromRelations) {
        if (rel.toUser == to) {
            return false;  // Request already sent or already friends
        }
    }
    
    // Add the friend request
    fromRelations.push_back(Relation(to, REQUEST_SENT));
    return true;
}

// ============================================================================
// FUNCTION: acceptFriendRequest
// ============================================================================
// Accept a friend request from 'from' user
// Logic:
// 1. Check if request exists (from → to with REQUEST_SENT)
// 2. Update edge to FRIEND status
// 3. Create reverse edge: to → from with FRIEND status
// 4. Return true
//
// Time Complexity: O(d) where d is degree of users
// ============================================================================
bool Graph::acceptFriendRequest(const string& from, const string& to) {
    // Check if request exists from 'from' to 'to'
    auto& fromRelations = adj[from];
    
    // Find the relationship
    auto it = std::find_if(fromRelations.begin(), fromRelations.end(),
                          [&to](const Relation& r) { return r.toUser == to; });
    
    if (it == fromRelations.end() || it->status != REQUEST_SENT) {
        return false;  // Request doesn't exist or already accepted
    }
    
    // Update the forward relationship to FRIEND
    it->status = FRIEND;
    
    // Add reverse relationship: to → from with FRIEND status
    // First ensure 'to' user exists
    if (!userExists(to)) {
        addUser(to);
    }
    
    auto& toRelations = adj[to];
    // Check if reverse relationship already exists (shouldn't happen if logic is correct)
    auto reverseIt = std::find_if(toRelations.begin(), toRelations.end(),
                                  [&from](const Relation& r) { return r.toUser == from; });
    
    if (reverseIt == toRelations.end()) {
        // Add reverse relationship
        toRelations.push_back(Relation(from, FRIEND));
    } else {
        // Reverse relationship exists, just update status to FRIEND
        reverseIt->status = FRIEND;
    }
    
    return true;
}

// ============================================================================
// RELATIONSHIP QUERY OPERATIONS
// ============================================================================

// ============================================================================
// FUNCTION: getFriends
// ============================================================================
// Get all confirmed friends of a user
// A friend is someone where:
// 1. Edge user → friend exists with status FRIEND
// 2. Edge friend → user exists with status FRIEND (bidirectional)
// Returns vector of all confirmed friend usernames
//
// Time Complexity: O(n) where n is degree of user
// ============================================================================
vector<string> Graph::getFriends(const string& user) const {
    vector<string> friends;
    
    // Check if user exists
    if (!userExists(user)) {
        return friends;  // Return empty vector if user doesn't exist
    }
    
    // Get all outgoing relations from user
    const auto& userRelations = adj.at(user);
    
    // For each relation, check if it's a confirmed friendship
    for (const Relation& rel : userRelations) {
        if (rel.status == FRIEND) {
            // Verify bidirectional friendship exists
            if (userExists(rel.toUser)) {
                const auto& otherRelations = adj.at(rel.toUser);
                auto reverseIt = std::find_if(otherRelations.begin(), otherRelations.end(),
                                            [&user](const Relation& r) {
                                                return r.toUser == user && r.status == FRIEND;
                                            });
                if (reverseIt != otherRelations.end()) {
                    friends.push_back(rel.toUser);
                }
            }
        }
    }
    
    return friends;
}

// ============================================================================
// FUNCTION: getMutualFriends
// ============================================================================
// Get mutual friends between two users
// Mutual friends are users who appear in both getFriends(user1) and getFriends(user2)
// Uses set intersection logic
//
// Time Complexity: O(n + m) where n, m are degrees of the two users
// ============================================================================
vector<string> Graph::getMutualFriends(const string& user1, const string& user2) const {
    vector<string> mutualFriends;
    
    // Get friends of both users
    vector<string> friends1 = getFriends(user1);
    vector<string> friends2 = getFriends(user2);
    
    // Find intersection: users who are friends with both
    for (const string& friend1 : friends1) {
        // Check if this friend is also in friends2
        auto it = std::find(friends2.begin(), friends2.end(), friend1);
        if (it != friends2.end()) {
            mutualFriends.push_back(friend1);
        }
    }
    
    return mutualFriends;
}

// ============================================================================
// FUNCTION: areFriends
// ============================================================================
// Check if two users are confirmed friends
// Returns true only if both directions have FRIEND status
// Time Complexity: O(d) where d is degree of user1
// ============================================================================
bool Graph::areFriends(const string& user1, const string& user2) const {
    if (!userExists(user1) || !userExists(user2)) {
        return false;
    }
    
    const auto& user1Relations = adj.at(user1);
    auto it = std::find_if(user1Relations.begin(), user1Relations.end(),
                          [&user2](const Relation& r) {
                              return r.toUser == user2 && r.status == FRIEND;
                          });
    
    if (it == user1Relations.end()) {
        return false;  // No FRIEND edge from user1 to user2
    }
    
    // Check reverse direction
    const auto& user2Relations = adj.at(user2);
    auto reverseIt = std::find_if(user2Relations.begin(), user2Relations.end(),
                                  [&user1](const Relation& r) {
                                      return r.toUser == user1 && r.status == FRIEND;
                                  });
    
    return reverseIt != user2Relations.end();
}

// ============================================================================
// FUNCTION: hasSentRequest
// ============================================================================
// Check if user1 sent a friend request to user2
// Returns true if edge user1 → user2 exists with status REQUEST_SENT
// Time Complexity: O(d) where d is degree of user1
// ============================================================================
bool Graph::hasSentRequest(const string& user1, const string& user2) const {
    if (!userExists(user1)) {
        return false;
    }
    
    const auto& user1Relations = adj.at(user1);
    auto it = std::find_if(user1Relations.begin(), user1Relations.end(),
                          [&user2](const Relation& r) {
                              return r.toUser == user2 && r.status == REQUEST_SENT;
                          });
    
    return it != user1Relations.end();
}

// ============================================================================
// FUNCTION: hasIncomingRequest
// ============================================================================
// Check if user1 has received a request from user2
// Equivalent to: user2 sent request to user1
// Returns true if edge user2 → user1 exists with status REQUEST_SENT
// Time Complexity: O(d) where d is degree of user2
// ============================================================================
bool Graph::hasIncomingRequest(const string& user1, const string& user2) const {
    return hasSentRequest(user2, user1);
}

// ============================================================================
// FUNCTION: removeFriendRequest
// ============================================================================
// Remove a pending friend request from 'from' to 'to'
// Returns true if request existed and was removed; false otherwise
// Time Complexity: O(d) where d is degree of 'from'
// ============================================================================
bool Graph::removeFriendRequest(const string& from, const string& to) {
    if (!userExists(from)) return false;
    auto& fromRelations = adj[from];
    auto it = std::find_if(fromRelations.begin(), fromRelations.end(),
                          [&to](const Relation& r) { return r.toUser == to && r.status == REQUEST_SENT; });
    if (it == fromRelations.end()) return false;
    fromRelations.erase(it);
    return true;
}

// ============================================================================
// FUNCTION: removeUser
// ============================================================================
// Remove a user from the graph, including all incoming and outgoing relations
// Returns true if user existed and was removed, false otherwise
// Time Complexity: O(V + E) where V is number of users, E is number of relations
// ============================================================================
bool Graph::removeUser(const string& username) {
    if (!userExists(username)) {
        return false;
    }

    // Remove outgoing relations from the user
    adj.erase(username);

    // Remove incoming relations to the user from all remaining nodes
    for (auto& pair : adj) {
        auto& relations = pair.second;
        relations.erase(std::remove_if(relations.begin(), relations.end(),
            [&username](const Relation& r) {
                return r.toUser == username;
            }), relations.end());
    }

    return true;
}

// ============================================================================
// FILE I/O OPERATIONS
// ============================================================================

// ============================================================================
// FUNCTION: saveToBinaryFile
// ============================================================================
// Serialize the entire graph to a binary file
// Binary File Format:
//   [userCount (size_t)]
//   For each user:
//     [username_length (size_t)][username (string)]
//     [relation_count (size_t)]
//     For each relation:
//       [toUser_length (size_t)][toUser (string)]
//       [status (int)]
//
// Time Complexity: O(V + E) where V is vertices, E is edges
// ============================================================================
bool Graph::saveToBinaryFile(const string& filepath) const {
    // Open file in binary write mode
    ofstream file(filepath, ios::binary | ios::trunc);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filepath << " for writing." << endl;
        return false;
    }
    
    try {
        // Step 1: Write the number of users
        size_t userCount = adj.size();
        file.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
        
        // Step 2: For each user, write their adjacency list
        for (const auto& userPair : adj) {
            const string& username = userPair.first;
            const vector<Relation>& relations = userPair.second;
            
            // Write username
            size_t usernameLength = username.length();
            file.write(reinterpret_cast<const char*>(&usernameLength), sizeof(usernameLength));
            file.write(username.c_str(), usernameLength);
            
            // Write number of relations
            size_t relationCount = relations.size();
            file.write(reinterpret_cast<const char*>(&relationCount), sizeof(relationCount));
            
            // Write each relation
            for (const Relation& rel : relations) {
                // Write target username
                size_t toUserLength = rel.toUser.length();
                file.write(reinterpret_cast<const char*>(&toUserLength), sizeof(toUserLength));
                file.write(rel.toUser.c_str(), toUserLength);
                
                // Write status
                int status = static_cast<int>(rel.status);
                file.write(reinterpret_cast<const char*>(&status), sizeof(status));
            }
        }
        
        file.close();
        return true;
        
    } catch (const std::exception& e) {
        cerr << "Error writing to graph file: " << e.what() << endl;
        file.close();
        return false;
    }
}

// ============================================================================
// FUNCTION: loadFromBinaryFile
// ============================================================================
// Deserialize the entire graph from a binary file
// Restores exact adjacency list structure from saved state
// If file doesn't exist, returns true with empty graph (normal for first run)
//
// Time Complexity: O(V + E) where V is vertices, E is edges
// ============================================================================
bool Graph::loadFromBinaryFile(const string& filepath) {
    // Clear existing graph
    clear();
    
    // Open file in binary read mode
    ifstream file(filepath, ios::binary);
    
    if (!file.is_open()) {
        // File doesn't exist - this is normal on first run
        return true;
    }
    
    try {
        // Step 1: Read the number of users
        size_t userCount = 0;
        file.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
        
        // Step 2: For each user, read their adjacency list
        for (size_t i = 0; i < userCount; ++i) {
            // Read username
            size_t usernameLength = 0;
            file.read(reinterpret_cast<char*>(&usernameLength), sizeof(usernameLength));
            
            string username(usernameLength, '\0');
            file.read(&username[0], usernameLength);
            
            // Create user entry if it doesn't exist
            addUser(username);
            
            // Read number of relations
            size_t relationCount = 0;
            file.read(reinterpret_cast<char*>(&relationCount), sizeof(relationCount));
            
            // Read each relation
            for (size_t j = 0; j < relationCount; ++j) {
                // Read target username
                size_t toUserLength = 0;
                file.read(reinterpret_cast<char*>(&toUserLength), sizeof(toUserLength));
                
                string toUser(toUserLength, '\0');
                file.read(&toUser[0], toUserLength);
                
                // Read status
                int status = 0;
                file.read(reinterpret_cast<char*>(&status), sizeof(status));
                
                // Add relation
                RelationStatus relStatus = static_cast<RelationStatus>(status);
                adj[username].push_back(Relation(toUser, relStatus));
            }
        }
        
        file.close();
        return true;
        
    } catch (const std::exception& e) {
        cerr << "Error reading from graph file: " << e.what() << endl;
        file.close();
        return false;
    }
}

// ============================================================================
// SUGGESTION GRAPH IMPLEMENTATION (RUNTIME ONLY)
// ============================================================================
// The SuggestionGraph is an ephemeral, randomly-connected graph used to
// traverse a sample of the user-space when generating friend suggestions.
// It intentionally does not persist to disk and is rebuilt each time the
// suggestions are requested.
// ============================================================================

SuggestionGraph::SuggestionGraph(const vector<string>& allUsernames) {
    adj.clear();
    for (const string& u : allUsernames) {
        adj[u] = vector<string>();
    }
}

void SuggestionGraph::generateRandomConnections() {
    // Random device + mersenne twister
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> degDist(2, 4);

    // Collect nodes
    vector<string> nodes;
    nodes.reserve(adj.size());
    for (const auto& p : adj) nodes.push_back(p.first);

    // Shuffle nodes to randomize pairing
    std::shuffle(nodes.begin(), nodes.end(), gen);

    // For each node, create deg random undirected edges to other nodes
    for (const string& u : nodes) {
        int deg = degDist(gen);
        std::uniform_int_distribution<size_t> idxDist(0, nodes.size() - 1);
        int attempts = 0;
        while (static_cast<int>(adj[u].size()) < deg && attempts < deg * 6) {
            size_t idx = idxDist(gen);
            const string& v = nodes[idx];
            if (v == u) { attempts++; continue; }
            // avoid duplicate
            auto &listU = adj[u];
            if (std::find(listU.begin(), listU.end(), v) != listU.end()) { attempts++; continue; }
            // add undirected edge
            listU.push_back(v);
            adj[v].push_back(u);
            attempts++;
        }
    }
}


// ============================================================================
// FUNCTION: bfsSample
// ============================================================================
// BFS sampling starting from `start`. Returns up to `limit` unique
// reachable usernames (excluding the start), following graph edges.
// Excludes any username present in the `exclude` set.
// Time Complexity: O(V + E) for traversal
// Traversal done using standard BFS with queue because it is an adjacency list
// ============================================================================
vector<string> SuggestionGraph::bfsSample(const string& start, const vector<string>& exclude, size_t limit) const {
    vector<string> result;
    if (adj.find(start) == adj.end()) return result;
    std::unordered_set<string> excluded;
    for (const auto &e : exclude) excluded.insert(e);

    std::unordered_set<string> visited;
    std::queue<string> q;
    visited.insert(start);
    q.push(start);

    while (!q.empty() && result.size() < limit) {
        string cur = q.front(); q.pop();
        auto it = adj.find(cur);
        if (it == adj.end()) continue;
        for (const string& nb : it->second) {
            if (visited.find(nb) != visited.end()) continue;
            visited.insert(nb);
            q.push(nb);
            if (nb == start) continue;
            if (excluded.find(nb) != excluded.end()) continue;
            result.push_back(nb);
            if (result.size() >= limit) break;
        }
    }

    return result;
}

// Helper type used for scoring pairs
struct _ScorePair { string username; int score; };

// Normalize string: remove spaces and lowercase for fair comparisons
static string normalizeString(const string &s) {
    string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == ' ') continue;
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return out;
}

// Compare two InterestVariant objects in a type-aware way.
// Appends short human-readable reasons into the `reasons` vector and
// returns an integer score representing the similarity contribution.
// ------------------------------------------------------------
//   Compare two InterestVariant values & accumulate similarity
// ------------------------------------------------------------
static int compareInterestVariants(const InterestVariant& A,
                                   const InterestVariant& B,
                                   vector<string>& reasons)
{
    // Type must match
    if (A.index() != B.index()) return 0;

    int score = 0;

    std::visit([&](auto&& aObj) {
        using T = std::decay_t<decltype(aObj)>;

        const auto& bObj = std::get<T>(B);

        // A helper lambda to score string attributes
        auto strMatch = [&](const string& X, const string& Y, int pts, string why) {
            if (!X.empty() && !Y.empty() && normalizeString(X) == normalizeString(Y)) {
                score += pts;
                reasons.push_back(why);
            }
        };

        // A helper lambda to score numeric closeness (year, rating, etc.)
        auto numClose = [&](int X, int Y, int maxPts, int tolerance, string why) {
            if (abs(X - Y) <= tolerance) {
                score += maxPts;
                reasons.push_back(why);
            }
        };

        // -------- BOOK ----------
        if constexpr (std::is_same_v<T, BookInterest>)
        {
            strMatch(aObj.getTitle(),  bObj.getTitle(),  5, "Read Same Book");
            strMatch(aObj.getAuthor(), bObj.getAuthor(), 1,  "");
            strMatch(aObj.getGenre(),  bObj.getGenre(),   1, "");
            numClose(aObj.getYear(),   bObj.getYear(),    5, 2, "");
            strMatch(aObj.getLanguage(), bObj.getLanguage(), 1, "");
            strMatch(aObj.getPublisher(), bObj.getPublisher(), 1, "");
        }

        // -------- MOVIE ----------
        if constexpr (std::is_same_v<T, MovieInterest>)
        {
            strMatch(aObj.getTitle(),       bObj.getTitle(),     5, "Watched Same Movie");
            strMatch(aObj.getGenre(),       bObj.getGenre(),      1, "");
            strMatch(aObj.getDirector(),    bObj.getDirector(),   1, "");
            strMatch(aObj.getLanguage(),    bObj.getLanguage(),   1, "");
            strMatch(aObj.getProduction(),  bObj.getProduction(), 1, "");
            numClose(aObj.getYear(),        bObj.getYear(),       2, 2, "");
        }

        // -------- PLACE ----------
        if constexpr (std::is_same_v<T, PlaceInterest>)
        {
            strMatch(aObj.getName(),      bObj.getName(),     5, "Visited same place");
            strMatch(aObj.getCountry(),   bObj.getCountry(),   4, "");
            strMatch(aObj.getCity(),      bObj.getCity(),      2, "");
            strMatch(aObj.getType(),      bObj.getType(),      1, "");
            strMatch(aObj.getActivities(), bObj.getActivities(), 1, "");
            strMatch(aObj.getBestSeason(), bObj.getBestSeason(), 1, "");
            // Compare visit dates (same day/month)
            strMatch(aObj.getVisitDate().toString(), bObj.getVisitDate().toString(), 4, "");
        }

        // -------- FOOD ----------
        if constexpr (std::is_same_v<T, FoodInterest>)
        {
            strMatch(aObj.getDish(),     bObj.getDish(),      5, "Ate Same Food");
            strMatch(aObj.getCuisine(),  bObj.getCuisine(),    2, "");
            strMatch(aObj.getPlace(),    bObj.getPlace(),      1, "");
            strMatch(aObj.getIngredients(), bObj.getIngredients(), 1, "");
            strMatch(aObj.getMeal(),     bObj.getMeal(),       1, "");
        }

        // -------- SPORT ----------
        if constexpr (std::is_same_v<T, SportInterest>)
        {
            strMatch(aObj.getSport(),    bObj.getSport(),      5, "Watches Same sport");
            strMatch(aObj.getLeague(),   bObj.getLeague(),      2, "");

            strMatch(aObj.getTeam1(),     bObj.getTeam1(),        1, "");
            strMatch(aObj.getTeam2(),     bObj.getTeam2(),        1, "");
            strMatch(aObj.getTeam1(),     bObj.getTeam2(),        1, "");
            strMatch(aObj.getTeam2(),     bObj.getTeam1(),        1, "");

            strMatch(aObj.getEventDate().toString(), bObj.getEventDate().toString(), 4, "");
            strMatch(aObj.getVenue(),     bObj.getVenue(),        4, "");
        }

    }, A);

    return score;
}

// Compute Intesrests arrays
// ------------------------------------------------------------
//    Efficient comparison of two interest arrays (O n)
// ------------------------------------------------------------
template<typename T>
static int compareInterestArray(const vector<T>& A,
                                const vector<T>& B,
                                vector<string>& reasons)
{
    auto makeKey = [&](const T& item) -> string {
        if constexpr (std::is_same_v<T, BookInterest>)
            return normalizeString(item.getTitle());
        if constexpr (std::is_same_v<T, MovieInterest>)
            return normalizeString(item.getTitle());
        if constexpr (std::is_same_v<T, PlaceInterest>)
            return normalizeString(item.getName());
        if constexpr (std::is_same_v<T, FoodInterest>)
            return normalizeString(item.getDish());
        if constexpr (std::is_same_v<T, SportInterest>)
            return normalizeString(item.getSport());
        return "";
    };

    unordered_map<string, vector<const T*>> lookup;
    lookup.reserve(B.size());

    for (const auto& b : B)
        lookup[makeKey(b)].push_back(&b);

    int score = 0;

    for (const auto& a : A)
    {
        string key = makeKey(a);
        if (key.empty()) continue;

        if (lookup.count(key))
        {
            for (const T* b : lookup[key])
            {
                InterestVariant va = a;
                InterestVariant vb = *b;
                score += compareInterestVariants(va, vb, reasons);
            }
        }
    }

    return score;
}


// Compare personal info
// ------------------------------------------------------------
//    Efficient comparison of personal info fields TC: O(1)
// ------------------------------------------------------------
static int comparePersonalInfo(const User& A, const User& B,
                               vector<string>& reasons)
{
    int score = 0;

    auto strEq = [&](const string& x, const string& y, int pts, string reason){
        if (!x.empty() && !y.empty() && normalizeString(x) == normalizeString(y)) {
            score += pts;
            reasons.push_back(reason);
        }
    };

    // Birthdate (DD/MM/YYYY)
    if (A.getBirthDate().toString() == B.getBirthDate().toString()) {
        score += 10;
        reasons.push_back("Same Birthdate");
    }

    strEq(A.getOccupation(), B.getOccupation(), 5, "Same Profession");
    strEq(A.getCity(),       B.getCity(),         4, "Same City");
    strEq(A.getCountry(),    B.getCountry(),      3, "");
    strEq(A.getGender(),     B.getGender(),       2, "Same Gender");

    return score;
}

// ===========================================================================
// FUNCTION: compareFavouriteInterests
// ===========================================================================
// This function compares the favorite interests of two users
// and accumulates similarity scores and reasons.
// This is separate from the interest arrays to give extra weight to favorites.
// TC: O(1)
static int compareFavouriteInterests(const User& A, const User& B,
                                 vector<string>& reasons)
{
    int score = 0;

    auto favBookA = A.getFavoriteBook();
    auto favBookB = B.getFavoriteBook();
    if (!favBookA.getTitle().empty() && !favBookB.getTitle().empty()) {
        if(normalizeString(favBookA.getTitle()) == normalizeString(favBookB.getTitle())
            && normalizeString(favBookA.getAuthor()) == normalizeString(favBookB.getAuthor())) {
            score += 10;
            reasons.push_back("Same Favorite Book");
        }
    }

    auto favMovieA = A.getFavoriteMovie();
    auto favMovieB = B.getFavoriteMovie();
    if (!favMovieA.getTitle().empty() && !favMovieB.getTitle().empty()) {
        if(normalizeString(favMovieA.getTitle()) == normalizeString(favMovieB.getTitle())
            && normalizeString(favMovieA.getDirector()) == normalizeString(favMovieB.getDirector())) {
            score += 10;
            reasons.push_back("Same Favorite Movie");
        }
    }

    auto favPlaceA = A.getFavoritePlace();
    auto favPlaceB = B.getFavoritePlace();
    if (!favPlaceA.getName().empty() && !favPlaceB.getName().empty()) {
        if(normalizeString(favPlaceA.getName()) == normalizeString(favPlaceB.getName())
            && normalizeString(favPlaceA.getCountry()) == normalizeString(favPlaceB.getCountry())
            && normalizeString(favPlaceA.getCity()) == normalizeString(favPlaceB.getCity())) {
            score += 10;
            reasons.push_back("Same Favorite Place");
        }
    }

    auto favFoodA = A.getFavoriteFood();
    auto favFoodB = B.getFavoriteFood();
    if (!favFoodA.getDish().empty() && !favFoodB.getDish().empty()) {
        if(normalizeString(favFoodA.getDish()) == normalizeString(favFoodB.getDish())
            && normalizeString(favFoodA.getCuisine()) == normalizeString(favFoodB.getCuisine())) {
            score += 10;
            reasons.push_back("Same Favorite Food");
        }
    }

    auto favSportA = A.getFavoriteSport();
    auto favSportB = B.getFavoriteSport();
    if (!favSportA.getSport().empty() && !favSportB.getSport().empty()) {
        if(normalizeString(favSportA.getSport()) == normalizeString(favSportB.getSport())
            && normalizeString(favSportA.getLeague()) == normalizeString(favSportB.getLeague())) {
            score += 10;
            reasons.push_back("Same Favorite Sport and League");
        }
    }

    return score;
}

// ============================================================================
// FUNCTION: computeSimilarityDetailed
// ============================================================================
// Compute similarity score between two users with detailed reasons.
// Returns a pair of (score [0-100], reason string)
// Compute similarity helper (internal): returns integer score and a simple match reason
// Time Complexity: O(n) where n is total number of interests
// ============================================================================
std::pair<int, std::string> computeSimilarityDetailed(const string& userA, const string& userB)
{
    FileManager fman;
    User A = fman.getUserByUsername(userA);
    User B = fman.getUserByUsername(userB);

    vector<string> reasons;

    // --------------------------------------------------------
    // 1. INTEREST SCORE (max 60)
    // --------------------------------------------------------
    int interestScore = 0;

    interestScore += compareInterestArray(A.getReadBooks(),  B.getReadBooks(),  reasons);
    interestScore += compareInterestArray(A.getWatchedMovies(), B.getWatchedMovies(), reasons);
    interestScore += compareInterestArray(A.getTravelledPlaces(), B.getTravelledPlaces(), reasons);
    interestScore += compareInterestArray(A.getTriedFoods(),  B.getTriedFoods(),  reasons);
    interestScore += compareInterestArray(A.getAttendedSportsEvents(), B.getAttendedSportsEvents(), reasons);
    interestScore += compareFavouriteInterests(A, B, reasons);

    // Cap interest portion at 60
    interestScore = std::min(60, interestScore);

    // --------------------------------------------------------
    // 2. PERSONAL INFO SCORE (max 40)
    // --------------------------------------------------------
    int personalScore = comparePersonalInfo(A, B, reasons);
    personalScore = std::min(40, personalScore);

    // --------------------------------------------------------
    // 3. FINAL TOTAL SCORE (0—100)
    // --------------------------------------------------------
    int finalScore = interestScore + personalScore;

    // --------------------------------------------------------
    // 4. Reason string
    // --------------------------------------------------------
    string reasonStr;
    for (auto& r : reasons) {
        reasonStr += "- " + r + " " ;//"";
    }

    return { finalScore, reasonStr };
}


// ============================================================================
// FUNCTION: suggestFor
// ============================================================================
// Suggest friends for the current user based on similarity scoring.
// Excludes current friends, pending requests, and mutual friends.
// Uses BFS sampling from the SuggestionGraph to find candidates.
// Returns topK suggestions with reasons.
// Time Complexity: O(n log n) where n is number of sampled candidates
vector<std::pair<string, string>> SuggestionGraph::suggestFor(const string& currentUser,
                                           const vector<string>& allUsernames,
                                           const Graph* relationshipGraph,
                                           size_t topK) const {
    // Build exclude set efficiently
    std::unordered_set<string> excludeSet;
    excludeSet.reserve(128);
    excludeSet.insert(currentUser);

    if (relationshipGraph != nullptr) {
        // exclude direct friends
        auto friends = relationshipGraph->getFriends(currentUser);
        for (const auto& f : friends) excludeSet.insert(f);

        // exclude pending requests (both directions)
        for (const auto& u : allUsernames) {
            if (relationshipGraph->hasSentRequest(currentUser, u) || relationshipGraph->hasSentRequest(u, currentUser)) {
                excludeSet.insert(u);
            }
        }
        // NOTE: do not pre-scan mutual friends for ALL users (expensive).
        // We'll check mutuals per-candidate below when evaluating eligibility.
    }

    // BFS sample from currentUser (skip excludes) -> keep existing bfsSample API
    // Build vector exclude for bfsSample if it needs it
    vector<string> excludeVec;
    excludeVec.reserve(excludeSet.size());
    for (const auto &s : excludeSet) excludeVec.push_back(s);

    vector<string> sampled = bfsSample(currentUser, excludeVec, 40);

    // Score each sampled candidate
    struct CandidateScore { string username; string reason; int score; };
    vector<CandidateScore> scored;
    scored.reserve(sampled.size());

    for (const string& cand : sampled) {
        // double-check eligibility quickly with set O(1)
        if (excludeSet.count(cand)) continue;
        if (relationshipGraph != nullptr) {
            // quick checks using relationshipGraph
            if (relationshipGraph->areFriends(currentUser, cand)) continue;
            if (relationshipGraph->hasSentRequest(currentUser, cand) || relationshipGraph->hasSentRequest(cand, currentUser)) continue;
            auto mutual = relationshipGraph->getMutualFriends(currentUser, cand);
            if (!mutual.empty()) continue; // exclude mutual friends
        }

        // compute similarity (cheap scalar)
        std::pair<int, std::string> detail = computeSimilarityDetailed(currentUser, cand);
        int score = detail.first;
        std::string reason = detail.second;

        // filter low-scoring candidates (use realistic threshold)
        if (score < 85) continue; // <-- change this from 85 to 35 during viva to demonstrate more results

        scored.push_back({cand, reason, score});
    }

    // Sort descending by score
    std::sort(scored.begin(), scored.end(), [](const CandidateScore& a, const CandidateScore& b){
        return a.score > b.score;
    });

    // Take topK usernames
    vector<std::pair<string, string>> result;
    result.reserve(std::min(topK, scored.size()));
    for (size_t i = 0; i < scored.size() && result.size() < topK; ++i) {
        result.push_back({scored[i].username, scored[i].reason});
    }

    return result;
}

// ============================================================================
// HIGH-LEVEL WRAPPER: getSuggestedFriendsWithReasons
// ============================================================================
// Encapsulates entire suggestion pipeline and returns results with reasons.
// This is the primary function to call from the UI layer.
// Time Complexity: O(n + m) where n=users, m=random edges created
// ============================================================================
vector<std::pair<string, string>> getSuggestedFriendsWithReasons(
    const string& currentUser,
    const vector<string>& allUsernames,
    const Graph* relationshipGraph,
    size_t topK) {

    vector<std::pair<string, string>> suggestions;
    if (allUsernames.empty()) return suggestions;

    // Ensure the runtime suggestion graph is built & shared (rebuilt per viewProfile)
    // This guarantees that UserHelper::suggestion_graph exists and is fresh.
    initializeSuggestionGraph();
    SuggestionGraph* sg = UserHelper::suggestion_graph;
    if (sg == nullptr) return suggestions; // nothing to do

    // Get raw suggestions (usernames) from shared runtime graph
    suggestions = sg->suggestFor(currentUser, allUsernames, relationshipGraph, topK);
    return suggestions;
}

// ============================================================================
// BACKEND INITIALIZATION: initializeSuggestionGraph
// ============================================================================
// Rebuilds the suggestion graph from scratch using UserHelper::all_usernames.
// This is called whenever the user views their suggested friends to refresh suggestions.
// Time Complexity: O(n + m) where n=users, m=random edges created
// ============================================================================
void initializeSuggestionGraph() {
    if (UserHelper::suggestion_graph != nullptr) {
        delete UserHelper::suggestion_graph;
        UserHelper::suggestion_graph = nullptr;
    }

    if (UserHelper::all_usernames.empty()) return;

    UserHelper::suggestion_graph = new SuggestionGraph(UserHelper::all_usernames);
    UserHelper::suggestion_graph->generateRandomConnections();
}