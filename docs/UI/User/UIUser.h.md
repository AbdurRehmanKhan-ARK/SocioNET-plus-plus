# UIUser.h

## Component Name
UIUser

## Responsibilities
- Declare dashboard and profile UI workflows.
- Provide functions for editing profiles, posting interests, searching users, and friend actions.
- Support navigation between profile, friend suggestions, and activity views.

## Data Structures
- `User` references for current and viewed profiles.
- `std::string` menu choices and prompts.
- `std::vector<string>` placeholders for friend lists.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `void dashboard(User& currentUser)` | `currentUser` | `void` | Displays the main user dashboard and routes actions. |
| `void viewProfile(User& currentUser)` | `currentUser` | `void` | Shows detailed profile data and profile actions. |
| `void editProfile(User& currentUser)` | `currentUser` | `void` | Edits profile fields and persists updates. |
| `bool deleteProfile(User& currentUser)` | `currentUser` | `bool` | Deletes the signed-in account after confirmation. |
| `void addInterest(User& user)` | `user` | `void` | Creates a new interest post and updates user record. |
| `void searchUser(User& currentUser)` | `currentUser` | `void` | Searches for users using substring matching. |
| `void viewFriends(const User& user)` | `user` | `void` | Placeholder for friend view functionality. |
| `void viewFriendRequests(User& currentUser)` | `currentUser` | `void` | Handles incoming friend request approvals and declines. |
| `void viewMyFriends(User& currentUser)` | `currentUser` | `void` | Lists confirmed friends and optionally visits their profiles. |
| `void viewMutualFriends(User& currentUser, User& otherUser)` | `currentUser`, `otherUser` | `void` | Displays mutual friends between two users. |
| `void showFriendSuggestions(User& currentUser)` | `currentUser` | `void` | Presents algorithmic friend suggestions and navigation. |
| `void viewOtherProfile(User& currentUser, User& otherUser)` | `currentUser`, `otherUser` | `void` | Views another user’s public profile with friend actions. |

## Algorithmic Complexity
- UI menu navigation is O(1) per action.
- Friend list and suggestion views are dominated by backend graph and suggestion costs.
- `viewFriendRequests()` and `viewMyFriends()` may be O(V + E) when scanning the graph adjacency list.

## Dependencies
- `User.h`
- `UIHelpers.h`
- `UIInterest.h`
- `Interest.h`
- `Authentication.h`
- `FileManager.h`
- `Graph.h`
