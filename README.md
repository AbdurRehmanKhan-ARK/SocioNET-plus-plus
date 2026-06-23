<div align="center">

<h1>SocioNET++ 🌐</h1>

<p><em>A high-performance, console-based social networking engine built on Graph Theory</em></p>

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey?style=for-the-badge)
![Build](https://img.shields.io/badge/Build-GCC%2015%20%7C%20MSYS2-brightgreen?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active%20Development-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

<br/>

> **SocioNET++** transforms classical data structures into a fully functional social ecosystem -
> modelling real-world human relationships through directed and undirected graph edges,
> persistent binary storage, and intelligent friend-suggestion algorithms.

</div>

---

## 📋 Table of Contents

* [Overview](#-overview)
* [Core Features](#-core-features)
* [System Architecture](#-system-architecture)
* [How It's Actually Built - Backend Deep Dive](#-how-its-actually-built--backend-deep-dive)
* [Prerequisites](#-prerequisites)
* [Getting Started](#-getting-started)
* [Project Structure](#-project-structure)
* [Module Reference](#-module-reference)
* [Roadmap](#-roadmap)
* [Contributors](#-contributors)

---

## 🔍 Overview

SocioNET++ is a semester project developed at **FAST-NUCES** for the Data Structures & Algorithms course. It simulates a console-based social media platform where users can register, build profiles, manage friendships, post interests, and discover new connections - all persisted across sessions using a custom binary file storage system.

The entire backend is built from scratch in **C++17** with no external libraries, demonstrating practical application of graphs, hash maps, binary search, suffix automata, and file I/O.

---

## 🌟 Core Features

| Module | Feature | Description |
|--------|---------|-------------|
| 👤 **User System** | Rich Profiling | Usernames, bio, interests, favorite books/movies/places/food/sports |
| 🔐 **Authentication** | Secure Login | Username or email login, encrypted passwords via custom hash |
| 🤝 **Friend Graph** | Relationship Engine | Send/accept/reject requests, bidirectional FRIEND edges |
| 🔎 **Search** | Substring Search | Username search powered by a **Suffix Automaton** (O(n) lookup) |
| 💡 **Suggestions** | Smart Recommendations | BFS-sampled candidates scored by a weighted interest/personal-info similarity model |
| 📦 **Persistence** | Binary Storage | 27 sorted binary files (A–Z + other) with O(log n) binary search |
| 📮 **Interest Posts** | Activity Feed | Stack-based recent posts using `std::variant` for polymorphic types |
| 🗺️ **Mutual Friends** | Social Insights | Set-intersection over adjacency lists to find shared connections |

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     main.cpp (Entry Point)               │
└──────────────────────────┬──────────────────────────────┘
                           │
          ┌────────────────┼────────────────┐
          ▼                ▼                ▼
   ┌─────────────┐  ┌─────────────┐  ┌──────────────┐
   │    UI Layer │  │  Auth Layer │  │  Graph Layer │
   │  UIUser.h   │  │  Authenti-  │  │   Graph.h    │
   │  UIInterest │  │  cation.h   │  │  (Friend +   │
   │  UIAuth.h   │  │             │  │  Suggestion) │
   │  UIHelpers  │  └──────┬──────┘  └──────┬───────┘
   └──────┬──────┘         │                │
          │         ┌──────▼──────┐         │
          └────────►│  User.h     │◄────────┘
                    │  Interest.h │
                    │  Helper.h   │
                    └──────┬──────┘
                           │
                    ┌──────▼──────┐
                    │ FileManager │
                    │  .h / .cpp  │
                    └──────┬──────┘
                           │
              ┌────────────┴────────────┐
              ▼                         ▼
     data/users/users_a.bin    data/graphs/friend_graph.bin
     data/users/users_b.bin    data/graphs/email_to_username.bin
     ...  (27 files total)
```

---

## 🧠 How It's Actually Built - Backend Deep Dive

This isn't just a features list - here's what's actually happening under the hood, module by module.

### 1. Friend Graph - Directed Edges That Become Bidirectional

`Graph` stores relationships as an `unordered_map<string, vector<Relation>>` adjacency list, where each `Relation` carries a status: `REQUEST_SENT` or `FRIEND`.

A friend request starts as a **one-way directed edge**:

```cpp
bool Graph::sendFriendRequest(const string& from, const string& to) {
    addUser(from);
    addUser(to);
    if (areFriends(from, to)) return false;

    auto& fromRelations = adj[from];
    for (const Relation& rel : fromRelations)
        if (rel.toUser == to) return false;  // already requested/friends

    fromRelations.push_back(Relation(to, REQUEST_SENT));
    return true;
}
```

When accepted, the engine **upgrades the forward edge** to `FRIEND` and **manually creates the reverse edge** - this is what turns a directed request into an undirected friendship:

```cpp
bool Graph::acceptFriendRequest(const string& from, const string& to) {
    auto it = std::find_if(adj[from].begin(), adj[from].end(),
                  [&to](const Relation& r){ return r.toUser == to; });
    if (it == adj[from].end() || it->status != REQUEST_SENT) return false;

    it->status = FRIEND;                              // forward edge
    adj[to].push_back(Relation(from, FRIEND));         // reverse edge
    return true;
}
```

`getFriends()` then only counts an edge as a real friendship if **both directions are confirmed** - protecting against half-finished state if a file write ever gets interrupted.

### 2. Mutual Friends - Set Intersection Over Adjacency Lists

No fancy traversal needed here - just pull both friend lists and intersect them:

```cpp
vector<string> Graph::getMutualFriends(const string& user1, const string& user2) const {
    vector<string> friends1 = getFriends(user1);
    vector<string> friends2 = getFriends(user2);
    vector<string> mutualFriends;

    for (const string& f : friends1)
        if (std::find(friends2.begin(), friends2.end(), f) != friends2.end())
            mutualFriends.push_back(f);

    return mutualFriends;
}
```
**Complexity:** O(n·m) for the two lists involved - fine here since friend lists are small per user, but the comment in code correctly flags it as O(n+m) territory if friends2 were hashed instead of linearly scanned.

### 3. Friend Suggestions - BFS Sampling + Weighted Similarity Scoring

This is the most interesting part of the codebase. Suggestions aren't random - they go through a 3-stage pipeline:

**Stage 1 - Exclude the obvious:** current friends, pending requests (either direction), and the user themself are all removed before any scoring happens.

**Stage 2 - BFS-sample 40 candidates** from a separate `SuggestionGraph` (built with randomized connections, kept apart from the real friend graph so suggestions stay independent of who's already friends).

**Stage 3 - Score every surviving candidate** out of 100, combining two weighted components:

```cpp
int interestScore = 0;
interestScore += compareInterestArray(A.getReadBooks(), B.getReadBooks(), reasons);
interestScore += compareInterestArray(A.getWatchedMovies(), B.getWatchedMovies(), reasons);
interestScore += compareInterestArray(A.getTravelledPlaces(), B.getTravelledPlaces(), reasons);
interestScore += compareInterestArray(A.getTriedFoods(), B.getTriedFoods(), reasons);
interestScore += compareInterestArray(A.getAttendedSportsEvents(), B.getAttendedSportsEvents(), reasons);
interestScore += compareFavouriteInterests(A, B, reasons);
interestScore = std::min(60, interestScore);   // capped at 60

int personalScore = std::min(40, comparePersonalInfo(A, B, reasons));

int finalScore = interestScore + personalScore;  // out of 100
```

Only candidates scoring **85+** make it through, and the system returns the score *and* a human-readable reason string (e.g. "Same Favorite Sport and League") for every suggestion - so the UI can explain *why* someone was recommended, not just that they were.

### 4. Persistence - 27-Way Sharded Binary Files + Manual Binary Search

Instead of one giant file, users are **sharded by the first alphabetic character of their username** into 27 files (`users_a.bin` … `users_z.bin`, `users_other.bin`):

```cpp
string FileManager::getUserFileName(char firstChar) const {
    if (isalpha(firstChar))
        return DATA_DIR + "users_" + string(1, tolower(firstChar)) + ".bin";
    return DATA_DIR + "users_other.bin";
}
```

Each shard is kept **strictly sorted alphabetically**, which is what makes binary search valid:

```cpp
int FileManager::binarySearchUsername(const vector<User>& users, const string& username) const {
    int left = 0, right = users.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        string midUsername = users[mid].getUsername();
        if (midUsername == username) return mid;
        else if (midUsername < username) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

New users aren't just appended - `insertUser()` scans for the correct alphabetical slot and inserts in place (an **insertion-sort-style** placement) so the shard stays sorted and every future lookup stays O(log n):

```cpp
int insertPos = 0;
for (size_t i = 0; i < users.size(); ++i) {
    if (users[i].getUsername() < username) insertPos = i + 1;
    else break;
}
users.insert(users.begin() + insertPos, user);
```

A separate `email_to_username.bin` file backs an in-memory `unordered_map` for O(1) email→username reverse lookups during login.

### 5. Username Search - A Real Suffix Automaton

`Helper.h`/`helper.cpp` builds a genuine **Suffix Automaton (SAM)**, not a naive substring scan. Every username is lower-cased and fed character-by-character through `extend()`, which implements the classic SAM construction (new state, suffix-link walking, clone-on-split):

```cpp
void SuffixAutomaton::extend(char c, int usernameID) {
    int cur = st.size();
    st.push_back(SAMState());
    st[cur].len = st[last].len + 1;

    int p = last;
    while (p != -1 && !st[p].next.count(c)) { st[p].next[c] = cur; p = st[p].link; }

    if (p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) st[cur].link = q;
        else {
            int clone = st.size();
            st.push_back(st[q]);
            st[clone].len = st[p].len + 1;
            while (p != -1 && st[p].next[c] == q) { st[p].next[c] = clone; p = st[p].link; }
            st[q].link = clone; st[cur].link = clone;
        }
    }
    last = cur;
    st[cur].username_ids.push_back(usernameID);
}
```

After all usernames are inserted, `finalize()` propagates each state's username IDs **up the suffix-link tree** so that searching any substring resolves to every username containing it. The payoff: searching for a substring is just a **character-by-character walk through the automaton's transition table** - O(pattern length), independent of how many usernames exist:

```cpp
vector<int> SuffixAutomaton::search(const string &pattern) {
    int v = 0;
    for (char c : toLower(pattern)) {
        if (!st[v].next.count(c)) return {};
        v = st[v].next[c];
    }
    return st[v].username_ids;
}
```

### 6. Interest Posts - `std::variant` Instead of Inheritance

Rather than a class hierarchy with virtual functions, 5 interest types (`BookInterest`, `MovieInterest`, `PlaceInterest`, `FoodInterest`, `SportInterest`) are unified through `InterestVariant = std::variant<...>`. Printing or serializing dispatches through `std::visit` with `if constexpr`, resolved entirely at compile time - zero virtual-call overhead:

```cpp
std::visit([](const auto &interest) {
    using T = std::decay_t<decltype(interest)>;
    if constexpr (std::is_same_v<T, BookInterest>)  std::cout << "Top Interest: A Book\n";
    else if constexpr (std::is_same_v<T, MovieInterest>) std::cout << "Top Interest: A Movie\n";
    // ...
}, stack.back());
```

For binary serialization, each variant is written with a 1-byte type tag followed by its own `serialize()` payload, then reconstructed on load with a `switch` over that tag - letting heterogeneous post types share a single stack and a single binary format.

### 7. Password Security - Double-Mixed Hash

Passwords aren't stored in plaintext or with a single hash pass. `encryptPassword()` combines username + email + password into one string, hashes it with `std::hash`, then **re-hashes the result mixed with the original password again** before storing both hashes as hex:

```cpp
string UserHelper::encryptPassword(const string &username, const string &email, const string &password) {
    string combined = username + "#" + email + "#" + password;
    size_t hashValue = hash<string>{}(combined);
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hashValue;
    size_t secondHash = hash<string>{}(std::to_string(hashValue) + password);
    ss << hex << setw(16) << setfill('0') << secondHash;
    return ss.str();
}
```

---

## 🧠 Data Structures & Algorithms

| Structure | Used In | Purpose |
|-----------|---------|---------|
| **Adjacency List** (`unordered_map`) | `Graph.h` | O(1) average-case friend lookup |
| **Binary Search** | `FileManager.h` | O(log n) user retrieval from sorted shards |
| **Suffix Automaton** | `Helper.h` | O(pattern length) username substring search |
| **BFS Sampling** | `Graph.h` | Candidate pool generation for friend suggestions |
| **Weighted Similarity Scoring** | `Graph.h` | Ranks BFS candidates out of 100 (60 interest + 40 personal) |
| **std::variant + std::visit** | `Interest.h` | Type-safe polymorphic interest storage, no vtables |
| **Hash Map** | `Helper.h` | O(1) email-to-username reverse lookup |
| **Stack (vector-backed)** | `Interest.h` | Recent posts / activity feed |
| **Insertion-style Placement** | `FileManager.h` | Maintaining sorted user shards on insert |

---

## 🛠 Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| **GCC / G++** | 13+ (C++17 required) | Compiler |
| **MSYS2 UCRT64** | Latest | Recommended toolchain on Windows |
| **VS Code** | Any | IDE with C/C++ extension |
| **Git** | Any | Version control |

> ⚠️ **GCC 6.x will NOT work.** `<variant>` and several C++17 features require GCC 8+.
> Install via MSYS2: `pacman -S mingw-w64-ucrt-x86_64-gcc`

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/AbdurRehmanKhan-ARK/SocioNet-Plus-Plus.git
cd SocioNet-Plus-Plus
```

### 2. Compile

This project uses a `sources.txt` manifest for clean multi-file compilation:

```bash
g++ -std=c++17 -g @sources.txt -Iinclude -o SocioNET.exe
```

> **Why `sources.txt`?** Instead of listing 15+ source files manually every compile, `sources.txt` acts as a build manifest - ensuring every developer compiles the exact same file set.

### 3. Run

```bash
./SocioNET.exe
```

### One-Click Debug (VS Code)

The `.vscode/` folder contains pre-configured `tasks.json` and `launch.json`. Press `F5` to build and debug instantly.

---

## 📂 Project Structure

```
SocioNet-Plus-Plus/
│
├── include/                  # All header files (.h)
│   ├── User.h                # Core user model
│   ├── Interest.h            # Polymorphic interest types (variant-based)
│   ├── Graph.h                # Friend graph + suggestion graph
│   ├── Helper.h               # Date, UserHelper, SuffixAutomaton
│   ├── FileManager.h          # Binary file I/O layer
│   ├── Authentication.h       # Registration & login logic
│   ├── UIUser.h                # Dashboard, profile UI
│   ├── UIInterest.h            # Interest/post display UI
│   ├── UIAuthentication.h      # Login/register UI
│   └── UIHelpers.h             # Box drawing, formatting utilities
│
├── src/                      # All implementation files (.cpp)
│   ├── main.cpp
│   ├── user.cpp
│   ├── interest.cpp
│   ├── graph.cpp
│   ├── helper.cpp
│   ├── fileManager.cpp
│   ├── authentication.cpp
│   ├── uiUser.cpp
│   ├── uiInterest.cpp
│   ├── uiAuthentication.cpp
│   └── uiHelpers.cpp
│
├── docs/                     # Technical documentation per module
│   ├── Authentication/
│   ├── Graph/
│   ├── File Management/
│   ├── Helper Class/
│   └── User Class/
│
├── data/                     # Auto-generated at runtime (gitignored)
│   ├── users/                # 27 sorted binary user files
│   └── graphs/               # friend_graph.bin, email_to_username.bin
│
├── .vscode/                  # VS Code config (tasks, launch, IntelliSense)
├── sources.txt               # Build manifest
├── .gitignore
└── README.md
```

---

## 📚 Module Reference

<details>
<summary><b>Graph.h - Relationship Engine</b></summary>

* `sendFriendRequest(from, to)` - Creates a directed `REQUEST_SENT` edge
* `acceptFriendRequest(from, to)` - Upgrades to bidirectional `FRIEND` edges
* `getFriends(user)` - Returns confirmed friends via adjacency list traversal, verifying both directions
* `getMutualFriends(u1, u2)` - Intersection of two friend-list vectors
* `SuggestionGraph::suggestFor(...)` - BFS sampling + weighted similarity scoring for recommendations
* `computeSimilarityDetailed(userA, userB)` - Scores a pair out of 100 (60 interests + 40 personal info) with human-readable match reasons
* Persisted via `saveToBinaryFile()` / `loadFromBinaryFile()`

</details>

<details>
<summary><b>FileManager.h - Storage Layer</b></summary>

* 27 binary files partition users by first alphabetic character of username
* All files maintain sorted order - enables O(log n) `binarySearchUsername()`
* `insertUser()` - finds insertion point via linear scan, shifts, writes back
* `getUserByUsername()` - determines shard, binary search, returns User
* `saveEmailHashmap()` / `loadEmailHashmap()` - flat binary serialization of the email→username map
* Email hashmap stored separately in `data/graphs/email_to_username.bin`

</details>

<details>
<summary><b>Interest.h - Post System</b></summary>

* 5 interest types: `BookInterest`, `MovieInterest`, `PlaceInterest`, `FoodInterest`, `SportInterest`
* All stored as `InterestVariant` (`std::variant`) - no inheritance overhead
* `InterestStack` - vector-backed stack for recent posts, fully serializable with a per-element type tag

</details>

<details>
<summary><b>Helper.h - Utilities</b></summary>

* `SuffixAutomaton` - real SAM construction (`extend`, `finalize`, `search`) for substring search across all usernames
* `UserHelper` - static email hashmap, username vector, graph pointers, password hashing
* `Date` - lightweight POD date type used by Interest and User

</details>

---

## 🌱 Roadmap

* [x] User registration & authentication with double-mixed hash encryption
* [x] Friend request system (send / accept / reject)
* [x] Interest posts with variant-based polymorphism
* [x] Binary file persistence across sessions
* [x] Mutual friends via adjacency-list intersection
* [x] Suffix Automaton username search
* [x] BFS-sampled, similarity-scored friend suggestions
* [ ] GUI migration using **SFML** or **Qt**
* [ ] PageRank implementation for influencer detection
* [ ] SQL database backend (replace binary files)
* [ ] Unit test suite (Google Test)

---

## 👥 Contributors

| Name | Role |
|------|------|
| **Syed Sufyan** | Backend Architecture, Graph Engine, File System, Suffix Automaton |
| **Abdur Rehman Khan** | Console UI Design, Box Rendering System, Graphs' Logic Integration, Authentication |

---

---

<div align="center">

**Developed with ❤️ at FAST-NUCES Karachi**

*Data Structures & Algorithms - Semester 3*

---

### ⚡ No algorithm was harmed in the making of this project.
*Every lookup is O(log n). Every search is O(pattern length). Every suggestion is scored out of 100.*
*We do not negotiate with O(n²).*

Happy connecting! 🌐

</div>
