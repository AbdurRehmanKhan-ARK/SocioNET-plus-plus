# authentication.cpp

## Component Name
Authentication implementation

## Responsibilities
- Implement email/password validation and sign-in/signup workflows.
- Persist new users and update shared lookup state.
- Coordinate registration with friend graph and username search index rebuild.

## Data Structures
- `std::regex` for username, email, and password validation.
- `UserHelper::email_to_username` — shared `std::unordered_map<string, string>`.
- `UserHelper::all_usernames` — shared vector of usernames used for search and suggestions.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `Authentication::Authentication()` | none | `Authentication` | Default constructor. |
| `bool initializeEmailHashmap()` | none | `bool` | Loads email map and username vector from binary storage. |
| `char getFirstAlphabeticChar(const string& username) const` | `username` | `char` | Returns first alphabetic character in a username or '\0'. |
| `bool isValidEmail(const string& email) const` | `email` | `bool` | Validates email format with regex. |
| `bool isValidPassword(const string& password) const` | `password` | `bool` | Validates password complexity with regex. |
| `bool isValidUsername(const string& username) const` | `username` | `bool` | Validates username rules with regex. |
| `bool signup(const string& username, const string& email, const string& password, string& errorMsg) const` | `username`, `email`, `password`, `errorMsg` | `bool` | Validates user data, creates new user, and persists state. |
| `User signin(const string& username, const string& password, string& errorMsg) const` | `username`, `password`, `errorMsg` | `User` | Loads user record and verifies encrypted credentials. |

## Algorithmic Complexity
- `signup()` — Time: O(M + U) where M = user file size for insertion and U = number of registered usernames for automaton rebuild; Space: O(U).
- `signin()` — Time: O(log M) for user lookup, plus O(1) for validation; Space: O(1).
- Validation methods — Time: O(L) for input length L, Space: O(1).

## Dependencies
- `Authentication.h`
- `FileManager.h`
- `User.h`
- `Helper.h`
- `UIHelpers.h`
- Standard library: `<algorithm>`, `<cctype>`, `<regex>`, `<iostream>`, `<iomanip>`, `<exception>`
