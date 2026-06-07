# Authentication.h

## Component Name
Authentication

## Responsibilities
- Define the authentication interface for user registration, sign-in, and account deletion.
- Expose validation and lookup helpers for username and email.
- Bridge frontend UI workflows and backend persistence logic.

## Data Structures
- `FileManager fileManager` — file-based user persistence handler.
- `UserHelper userHelper` — shared helper object containing email lookup and username search state.
- `std::string` references for username, email, password, and error messages.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `Authentication()` | none | `Authentication` | Constructs the authentication controller. |
| `bool initializeEmailHashmap()` | none | `bool` | Loads persistent email-to-username mappings and username cache at startup. |
| `User registerUserWithUI()` | none | `User` | Complete registration flow with UI, validation, and persistence. |
| `User signInWithUI()` | none | `User` | Complete sign-in flow with UI and credential verification. |
| `bool isUsernameAvailable(const string& username) const` | `username` | `bool` | Checks availability by querying persisted usernames. |
| `bool isEmailAvailable(const string& email) const` | `email` | `bool` | Checks whether the email is already registered in the shared map. |
| `string getUsernameByEmail(const string& email) const` | `email` | `string` | Returns associated username from email map. |
| `bool deleteUserAccount(const User& currentUser, const string& password, string& errorMsg) const` | `currentUser`, `password`, `errorMsg` | `bool` | Validates password and removes a user account. |

## Algorithmic Complexity
- `initializeEmailHashmap()` — Time: O(N), Space: O(N) where N = number of registered emails.
- `isUsernameAvailable()` — Time: O(log M) where M = users in the relevant user file.
- `isEmailAvailable()` — Time: O(1) average, Space: O(1).
- `getUsernameByEmail()` — Time: O(1) average, Space: O(1).
- `deleteUserAccount()` — Time: O(M) due to file update and delete operations.

## Dependencies
- `User.h`
- `FileManager.h`
- `Helper.h`
- `UIHelpers.h`
- Standard library: `<string>`
