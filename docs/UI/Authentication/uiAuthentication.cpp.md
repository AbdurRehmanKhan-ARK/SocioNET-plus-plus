# uiAuthentication.cpp

## Component Name
UI authentication implementation

## Responsibilities
- Implement the interactive registration and sign-in screens.
- Validate user input before passing it to the authentication backend.
- Display success, failure, and reset prompts.

## Data Structures
- `std::regex` patterns for username, email, and password validation.
- Local strings for username, email, password, and UI prompts.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `User registerUser(Authentication& auth)` | `auth` | `User` | Executes the registration flow, validates input, and saves a new user. |
| `User signIn(Authentication& auth)` | `auth` | `User` | Executes sign-in flow and returns the authenticated user. |

## Algorithmic Complexity
- Input validation loops are O(L) per entry due to regex checks.
- Backend calls to `auth.signup()` and `auth.signin()` dominate actual user creation and lookup cost.
- UI formatting operations are O(1) per printed line.

## Dependencies
- `UIAuthentication.h`
- `Authentication.h`
- `UIHelpers.h`
- Standard library: `<regex>`, `<algorithm>`
