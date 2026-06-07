# user.cpp

## Component Name
User implementation

## Responsibilities
- Implement serialization and deserialization for persistent user storage.
- Implement helper methods that delegate friend relationship queries to the global graph.

## Data Structures
- `std::string` fields for user identity and profile values.
- `Date birth_date` for birthdate persistence.
- Interest vectors and `InterestStack` for user content.
- Local helper lambdas for binary read/write operations.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `User::User()` | none | `User` | Default constructor initializing empty fields. |
| `User::User(const std::string &uname)` | `uname` | `User` | Constructs a user with a username. |
| `void serialize(std::ostream &os) const` | `os` | `void` | Serializes the entire user profile to binary format. |
| `bool isFriend(const std::string& other) const` | `other` | `bool` | Delegates confirmed friendship check to the global graph. |
| `bool hasSentRequest(const std::string& other) const` | `other` | `bool` | Delegates outgoing request check. |
| `bool hasIncomingRequest(const std::string& other) const` | `other` | `bool` | Delegates incoming request check. |
| `void deserialize(std::istream &is)` | `is` | `void` | Restores a user object from binary data. |

## Algorithmic Complexity
- `serialize()` / `deserialize()` — Time: O(F + N) where F is fixed field count and N is total interest items; Space: O(1) auxiliary.
- Friend helper methods — Time: O(d) where d = relationship degree in the global graph.

## Dependencies
- `User.h`
- `Graph.h`
- `Interest.h`
- `Helper.h`
- Standard library: `<iostream>`, `<string>`, `<vector>`
