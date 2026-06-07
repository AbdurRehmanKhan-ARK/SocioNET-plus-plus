# uiUser.cpp

## Component Name
UIUser implementation

## Responsibilities
- Implement user dashboard navigation and profile workflows.
- Support profile editing, post creation, account deletion, and friend interactions.
- Perform search, friend request handling, suggestion display, and profile browsing.

## Data Structures
- `User` references for current and target profiles.
- `std::vector<string>` for friend lists, mutual friends, and search results.
- `InterestVariant` objects for new posts.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `void addInterest(User& user)` | `user` | `void` | Collects post details, updates user interests, and persists the user. |
| `void editProfile(User& currentUser)` | `currentUser` | `void` | Allows editing of profile fields and saves changes. |
| `bool deleteProfile(User& currentUser)` | `currentUser` | `bool` | Confirms and deletes the signed-in user account. |
| `void viewProfile(User& currentUser)` | `currentUser` | `void` | Displays current profile details and action menu. |
| `void dashboard(User& currentUser)` | `currentUser` | `void` | Presents the main user dashboard. |
| `void searchUser(User& currentUser)` | `currentUser` | `void` | Performs username substring search and views selected profiles. |
| `void viewFriends(const User& user)` | `user` | `void` | Placeholder for friend list display. |
| `void viewFriendRequests(User& currentUser)` | `currentUser` | `void` | Lists pending requests and enables accept/decline actions. |
| `void viewMyFriends(User& currentUser)` | `currentUser` | `void` | Lists current friends and allows visiting their profiles. |
| `void viewMutualFriends(User& currentUser, User& otherUser)` | `currentUser`, `otherUser` | `void` | Displays mutual friends between users. |
| `void showFriendSuggestions(User& currentUser)` | `currentUser` | `void` | Lists recommendation results and allows profile visit. |
| `void viewOtherProfile(User& currentUser, User& otherUser)` | `currentUser`, `otherUser` | `void` | Displays another user's profile and friend actions. |

## Algorithmic Complexity
- UI interaction loops are O(1) per selection.
- `addInterest()` — O(1) for stack push plus `FileManager::updateUser` cost O(N).
- `searchUser()` — O(P + R) where P = search pattern length and R = result count.
- `viewFriendRequests()` / `viewMyFriends()` — O(V + E) scanning the graph adjacency list.
- `showFriendSuggestions()` — dominated by suggestion pipeline O(S log S) where S = sampled candidates.

## Dependencies
- `UIUser.h`
- `Authentication.h`
- `FileManager.h`
- `Helper.h`
- `Graph.h`
- `UIHelpers.h`
- `UIInterest.h`
- `Interest.h`
