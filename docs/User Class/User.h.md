# User.h

## Component Name
User

## Responsibilities
- Define the application user model, profile fields, and interest relationships.
- Expose accessors for profile metadata and interest collections.
- Declare serialization support for persistent user storage.
- Declare friend relationship helper methods using the global graph.

## Data Structures
- `std::string` fields for username, password, email, name, and profile metadata.
- `Date birth_date` — birth date object from `Helper.h`.
- Individual favorite interest objects: `BookInterest`, `MovieInterest`, `PlaceInterest`, `FoodInterest`, `SportInterest`.
- Category vectors: `read_books`, `travelled_places`, `tried_foods`, `attended_sports_events`, `watched_movies`.
- `InterestStack recent_posts` — stack of recent posts.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `User()` | none | `User` | Default constructor. |
| `User(const string &uname)` | `uname` | `User` | Constructs with a username. |
| `void setUsername(const string &u)` | `u` | `void` | Sets the username. |
| `string getUsername() const` | none | `string` | Returns the username. |
| `void setPassword(const string &p)` | `p` | `void` | Sets encrypted password. |
| `string getPassword() const` | none | `string` | Returns stored password string. |
| `void setName(const string &n)` | `n` | `void` | Sets display name. |
| `string getName() const` | none | `string` | Gets display name. |
| `void setEmail(const string &e)` | `e` | `void` | Sets email address. |
| `string getEmail() const` | none | `string` | Gets email address. |
| `void setBirthDate(const Date &d)` | `d` | `void` | Sets birth date. |
| `Date getBirthDate() const` | none | `Date` | Gets birth date. |
| `double profileCompletion() const` | none | `double` | Returns completion score for profile metadata. |
| `bool isFriend(const std::string& username) const` | `username` | `bool` | Delegates friendship check to global graph. |
| `bool hasSentRequest(const std::string& username) const` | `username` | `bool` | Checks sent friend request status. |
| `bool hasIncomingRequest(const std::string& username) const` | `username` | `bool` | Checks incoming request status. |
| `void serialize(ostream &os) const` | `os` | `void` | Serializes the user to a binary stream. |
| `void deserialize(istream &is)` | `is` | `void` | Deserializes a user from a binary stream. |

## Algorithmic Complexity
- Accessors and mutators — O(1).
- `profileCompletion()` — O(1).
- Friend helper methods — O(d) where d = degree in the global graph.
- Serialization and deserialization — O(F + N) where F = fixed field count and N = number of interest items.

## Dependencies
- `Helper.h`
- `Interest.h`
- Standard library: `<string>`, `<vector>`
