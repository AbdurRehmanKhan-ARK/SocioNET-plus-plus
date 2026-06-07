# helper.cpp

## Component Name
Helper implementation

## Responsibilities
- Implement shared helper data and global state initialization.
- Implement password encryption and email lookup helpers.
- Define the shared suffix automaton, graph pointers, and helper mapping containers.

## Data Structures
- `std::unordered_map<string, string> UserHelper::email_to_username` — global email map.
- `std::vector<string> UserHelper::all_usernames` — all known usernames for search and suggestions.
- `Graph* UserHelper::friend_graph` — pointer to the global friend graph.
- `SuggestionGraph* UserHelper::suggestion_graph` — pointer to runtime suggestion graph.
- `SuffixAutomaton UserHelper::sam` — shared search automaton.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `SuffixAutomaton::SuffixAutomaton()` | none | `SuffixAutomaton` | Initializes the automaton root and reserve storage. |
| `static string toLower(const string &s)` | `s` | `string` | Returns lower-case copy of the string. |
| `void SuffixAutomaton::addUsername(const string &s, int usernameID)` | `s`, `usernameID` | `void` | Inserts a username into the automaton using case-insensitive characters. |
| `void SuffixAutomaton::extend(char c, int usernameID)` | `c`, `usernameID` | `void` | Extends the automaton with a single character transition. |
| `void SuffixAutomaton::finalize()` | none | `void` | Propagates username IDs through suffix links and deduplicates states. |
| `vector<int> SuffixAutomaton::search(const string &pattern)` | `pattern` | `vector<int>` | Returns matching username IDs for a substring query. |
| `string UserHelper::encryptPassword(const string &username, const string &email, const string &password)` | `username`, `email`, `password` | `string` | Generates a deterministic hashed password string. |
| `bool UserHelper::isEmailRegistered(const string &email) const` | `email` | `bool` | Checks the global email registration map. |
| `void UserHelper::registerEmail(const string &email, const string &username) const` | `email`, `username` | `void` | Registers a new email mapping. |

## Algorithmic Complexity
- `addUsername()` / `extend()` — Time: O(L), Space: O(L).
- `finalize()` — Time: O(S log S) due to sorting state vectors, Space: O(S).
- `search()` — Time: O(P), Space: O(1) aux.
- `encryptPassword()` — Time: O(L), Space: O(1) aux.
- `isEmailRegistered()` / `registerEmail()` — Time: O(1) average, Space: O(1).

## Dependencies
- `Helper.h`
- `Graph.h`
- Standard library: `<string>`, `<sstream>`, `<iomanip>`, `<functional>`, `<unordered_map>`, `<algorithm>`, `<cctype>`, `<numeric>`
