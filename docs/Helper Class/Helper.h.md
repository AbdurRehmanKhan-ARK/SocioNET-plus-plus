# Helper.h

## Component Name
Helper utilities

## Responsibilities
- Define shared utility types and search infrastructure used across the project.
- Declare the suffix automaton for username substring search.
- Provide lightweight user-related helpers and global shared state.

## Data Structures
- `struct SAMState` — suffix automaton state with link, length, transitions, and username IDs.
- `vector<SAMState> st` — state container for the suffix automaton.
- `unordered_map<char, int> next` — transitions from a SAM state.
- `vector<int> username_ids` — list of matched usernames for each state.
- `class Date` — simple POD date type with day/month/year.
- `class UserHelper` — static global helper containing:
  - `email_to_username` map
  - `all_usernames` vector
  - pointers to global `Graph` and `SuggestionGraph`
  - shared `SuffixAutomaton` instance

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `SuffixAutomaton()` | none | `SuffixAutomaton` | Constructs an empty suffix automaton. |
| `void addUsername(const std::string &s, int usernameID)` | `s`, `usernameID` | `void` | Inserts a username string into the automaton. |
| `void extend(char c, int usernameID)` | `c`, `usernameID` | `void` | Extends the automaton with a character. |
| `void finalize()` | none | `void` | Collates username IDs through suffix links. |
| `std::vector<int> search(const std::string &pattern)` | `pattern` | `vector<int>` | Searches for a substring in all registered usernames. |
| `Date(int d = 0, int m = 0, int y = 0)` | `d`, `m`, `y` | `Date` | Constructs a date object. |
| `std::string toString() const` | none | `std::string` | Formats the date as `dd/mm/yyyy`. |
| `static std::string encryptPassword(const std::string &username, const std::string &email, const std::string &password)` | `username`, `email`, `password` | `std::string` | Creates a deterministic password hash string. |
| `bool isEmailRegistered(const std::string &email) const` | `email` | `bool` | Checks email registration in the shared map. |
| `void registerEmail(const std::string &email, const std::string &username) const` | `email`, `username` | `void` | Inserts email-to-username mapping into shared state. |

## Algorithmic Complexity
- `addUsername()` / `extend()` — Time: O(L), Space: O(L) for a username of length L.
- `finalize()` — Time: O(S log S) dominated by sort and unique operations across states, Space: O(S) where S = number of states.
- `search()` — Time: O(P) where P = pattern length, Space: O(1) auxiliary.
- `encryptPassword()` — Time: O(L), Space: O(1) auxiliary.
- `isEmailRegistered()` / `registerEmail()` — Time: O(1) average, Space: O(1).

## Dependencies
- `Graph` and `SuggestionGraph` forward declarations
- Standard library: `<iostream>`, `<string>`, `<vector>`, `<functional>`, `<unordered_map>`
