<div align="center">

<h1>SocioNET++ 🌐</h1>

<p><em>A high-performance, console-based social networking engine built on Graph Theory</em></p>

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey?style=for-the-badge)
![Build](https://img.shields.io/badge/Build-GCC%2015%20%7C%20MSYS2-brightgreen?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active%20Development-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

<br/>

> **SocioNET++** transforms classical data structures into a fully functional social ecosystem —
> modelling real-world human relationships through directed and undirected graph edges,
> persistent binary storage, and intelligent friend-suggestion algorithms.

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Core Features](#-core-features)
- [System Architecture](#-system-architecture)
- [Data Structures & Algorithms](#-data-structures--algorithms)
- [Prerequisites](#-prerequisites)
- [Getting Started](#-getting-started)
- [Project Structure](#-project-structure)
- [Module Reference](#-module-reference)
- [Roadmap](#-roadmap)
- [Contributors](#-contributors)

---

## 🔍 Overview

SocioNET++ is a semester project developed at **FAST-NUCES** for the Data Structures & Algorithms course. It simulates a console-based social media platform where users can register, build profiles, manage friendships, post interests, and discover new connections — all persisted across sessions using a custom binary file storage system.

The entire backend is built from scratch in **C++17** with no external libraries, demonstrating practical application of graphs, hash maps, binary search, suffix automata, and file I/O.

---

## 🌟 Core Features

| Module | Feature | Description |
|--------|---------|-------------|
| 👤 **User System** | Rich Profiling | Usernames, bio, interests, favorite books/movies/places/food/sports |
| 🔐 **Authentication** | Secure Login | Username or email login, encrypted passwords via custom hash |
| 🤝 **Friend Graph** | Relationship Engine | Send/accept/reject requests, bidirectional FRIEND edges |
| 🔎 **Search** | Substring Search | Username search powered by a **Suffix Automaton** (O(n) lookup) |
| 💡 **Suggestions** | Smart Recommendations | BFS-based friend suggestions scored by interest similarity |
| 📦 **Persistence** | Binary Storage | 27 sorted binary files (A–Z + other) with O(log n) binary search |
| 📮 **Interest Posts** | Activity Feed | Stack-based recent posts using `std::variant` for polymorphic types |
| 🗺️ **Mutual Friends** | Social Insights | BFS traversal to find shared connections between any two users |

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

## 🧠 Data Structures & Algorithms

| Structure | Used In | Purpose |
|-----------|---------|---------|
| **Adjacency List** (`unordered_map`) | `Graph.h` | O(1) average-case friend lookup |
| **Binary Search** | `FileManager.h` | O(log n) user retrieval from sorted files |
| **Suffix Automaton** | `Helper.h` | O(n) username substring search |
| **BFS** | `Graph.h` | Mutual friend discovery & suggestion sampling |
| **std::variant** | `Interest.h` | Type-safe polymorphic interest storage |
| **Hash Map** | `Helper.h` | O(1) email-to-username reverse lookup |
| **Stack (vector-backed)** | `Interest.h` | Recent posts / activity feed |
| **Insertion Sort** | `FileManager.h` | Maintaining sorted user files on insert |

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

> **Why `sources.txt`?** Instead of listing 15+ source files manually every compile, `sources.txt` acts as a build manifest — ensuring every developer compiles the exact same file set.

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
│   ├── Graph.h               # Friend graph + suggestion graph
│   ├── Helper.h              # Date, UserHelper, SuffixAutomaton
│   ├── FileManager.h         # Binary file I/O layer
│   ├── Authentication.h      # Registration & login logic
│   ├── UIUser.h              # Dashboard, profile UI
│   ├── UIInterest.h          # Interest/post display UI
│   ├── UIAuthentication.h    # Login/register UI
│   └── UIHelpers.h           # Box drawing, formatting utilities
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
<summary><b>Graph.h — Relationship Engine</b></summary>

- `sendFriendRequest(from, to)` — Creates a directed `REQUEST_SENT` edge
- `acceptFriendRequest(from, to)` — Upgrades to bidirectional `FRIEND` edges
- `getFriends(user)` — Returns confirmed friends via adjacency list traversal
- `getMutualFriends(u1, u2)` — BFS-based intersection of two friend sets
- `SuggestionGraph::suggestFor(...)` — Hybrid BFS + similarity scoring for recommendations
- Persisted via `saveToBinaryFile()` / `loadFromBinaryFile()`

</details>

<details>
<summary><b>FileManager.h — Storage Layer</b></summary>

- 27 binary files partition users by first alphabetic character of username
- All files maintain sorted order — enables O(log n) binary search
- `insertUser()` — finds insertion point, shifts, writes back
- `getUserByUsername()` — determines file → binary search → returns User
- Email hashmap stored separately in `data/graphs/email_to_username.bin`

</details>

<details>
<summary><b>Interest.h — Post System</b></summary>

- 5 interest types: `BookInterest`, `MovieInterest`, `PlaceInterest`, `FoodInterest`, `SportInterest`
- All stored as `InterestVariant` (`std::variant`) — no inheritance overhead
- `InterestStack` — vector-backed stack for recent posts, fully serializable

</details>

<details>
<summary><b>Helper.h — Utilities</b></summary>

- `SuffixAutomaton` — O(n) substring search across all usernames
- `UserHelper` — static email hashmap, username vector, graph pointers
- `Date` — lightweight POD date type used by Interest and User

</details>

---

## 🌱 Roadmap

- [x] User registration & authentication with encryption
- [x] Friend request system (send / accept / reject)
- [x] Interest posts with variant-based polymorphism
- [x] Binary file persistence across sessions
- [x] BFS-based mutual friends
- [x] Suffix Automaton username search
- [x] Friend suggestion algorithm
- [ ] GUI migration using **SFML** or **Qt**
- [ ] PageRank implementation for influencer detection
- [ ] SQL database backend (replace binary files)
- [ ] Unit test suite (Google Test)

---

## 👥 Contributors

| Name | Role |
|------|------|
| **Abdur Rehman Khan** | Backend, Graph Engine, File System, Auth |
| **Syed Sufyan** | Frontend UI, Project Architecture, Integration |

---

<div align="center">

**Developed with ❤️ at FAST-NUCES Karachi**

*Data Structures & Algorithms — Semester 4*

</div>
