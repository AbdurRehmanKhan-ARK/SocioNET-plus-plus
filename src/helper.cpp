#include "../include/Helper.h"
#include "../include/Graph.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <numeric>
using std::string;
using std::stringstream;
using std::unordered_map;
using std::hash;
using std::vector;
using std::hex;
using std::setw;
using std::setfill;

// ============================================================================
// SUFFIX AUTOMATON IMPLEMENTATION
// ============================================================================

SuffixAutomaton::SuffixAutomaton() {
    st.reserve(200000);
    st.push_back(SAMState());  // root state
    last = 0;
    totalUsernames = 0;
}

// Helper: convert string to lowercase
static string toLower(const string &s) {
    string result = s;
    for (char &c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}

void SuffixAutomaton::addUsername(const string &s, int usernameID) {
    // Convert to lowercase for case-insensitive search
    string lowerS = toLower(s);
    last = 0;  // reset to root for every username

    for (char c : lowerS) {
        extend(c, usernameID);
    }

    totalUsernames++;
}

void SuffixAutomaton::extend(char c, int usernameID) {
    int cur = st.size();
    st.push_back(SAMState());
    st[cur].len = st[last].len + 1;

    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }

    if (p == -1) {
        st[cur].link = 0;
    }
    else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        }
        else {
            int clone = st.size();
            st.push_back(st[q]);
            st[clone].len = st[p].len + 1;

            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }

            st[q].link = clone;
            st[cur].link = clone;
        }
    }

    last = cur;
    st[cur].username_ids.push_back(usernameID);
}

void SuffixAutomaton::finalize() {
    // Propagate username IDs up suffix links
    vector<int> order(st.size());
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return st[a].len > st[b].len;
    });

    for (int v : order) {
        if (st[v].link != -1) {
            auto &parent = st[st[v].link].username_ids;
            auto &child = st[v].username_ids;
            parent.insert(parent.end(), child.begin(), child.end());
        }
    }

    // Sort and unique each state's username_ids
    for (auto &state : st) {
        std::sort(state.username_ids.begin(), state.username_ids.end());
        state.username_ids.erase(std::unique(state.username_ids.begin(), state.username_ids.end()), state.username_ids.end());
    }
}

vector<int> SuffixAutomaton::search(const string &pattern) {
    // Convert pattern to lowercase for case-insensitive search
    string lowerPattern = toLower(pattern);
    int v = 0;

    for (char c : lowerPattern) {
        if (!st[v].next.count(c)) {
            return {};  // no such substring
        }
        v = st[v].next[c];
    }

    return st[v].username_ids;
}

// helper.cpp
// No implementations required at the moment because Date is a simple class
// fully defined in the header. This file is kept so you can add helper
// function implementations in the future.

unordered_map<string, string> UserHelper::email_to_username;
vector<string> UserHelper::all_usernames;  // Vector of all usernames for graph operations
Graph* UserHelper::friend_graph = nullptr;  // Global friend relationship graph (initialized in main)
SuggestionGraph* UserHelper::suggestion_graph = nullptr;  // Runtime suggestion graph (rebuilt on each viewProfile)
SuffixAutomaton UserHelper::sam; // Suffix Automaton Object initialized for username substring search

string UserHelper::encryptPassword(const string &username, const string &email, const string &password) {
    // Combine username, email, and password
    string combined = username + "#" + email + "#" + password;

    // Use std::hash to create a unique hash value
    size_t hashValue = hash<string>{}(combined);

    // Convert hash to hexadecimal string
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hashValue;

    // Optionally do a secondary mix to make it less predictable
    size_t secondHash = hash<string>{}(std::to_string(hashValue) + password);
    ss << hex << setw(16) << setfill('0') << secondHash;

    return ss.str();
}

bool UserHelper::isEmailRegistered(const string &email) const {
	return email_to_username.find(email) != email_to_username.end();
}

void UserHelper::registerEmail(const string &email, const string &username) const {
	email_to_username[email] = username;
}