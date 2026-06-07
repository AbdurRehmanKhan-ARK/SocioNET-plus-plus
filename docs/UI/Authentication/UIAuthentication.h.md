# UIAuthentication.h

## Component Name
UIAuthentication

## Responsibilities
- Declare the UI entry points for user registration and sign-in.
- Provide a simple frontend wrapper around the `Authentication` backend.

## Data Structures
- `User` return values for completed registration and sign-in actions.
- `Authentication& auth` parameter to access backend authentication logic.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `User registerUser(Authentication& auth)` | `auth` | `User` | Prompts the user for registration data and calls backend signup. |
| `User signIn(Authentication& auth)` | `auth` | `User` | Prompts the user for credentials and calls backend signin. |

## Algorithmic Complexity
- Both functions are UI workflows dominated by user input handling and backend calls.
- `registerUser()` and `signIn()` are effectively O(1) in application logic, with backend validation calls adding their own cost.

## Dependencies
- `Authentication.h`
- `UIHelpers.h`
- `User.h`
- Standard library: `<string>`
