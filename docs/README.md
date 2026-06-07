# SocioNET++ Documentation Index

This documentation index serves as the Table of Contents for the `docs/` folder. Each entry links to a dedicated Markdown file that documents a specific header or source module in the SocioNET++ project.

## Documentation Structure

- [Core Initialization & Entry](#core-initialization--entry)
- [Authentication & User Persistence](#authentication--user-persistence)
- [Friend Graph & Suggestion Engine](#friend-graph--suggestion-engine)
- [User Model & Helpers](#user-model--helpers)
- [Interest System](#interest-system)
- [UI Modules](#ui-modules)

---

## Core Initialization & Entry

- [main.cpp](./Main%20File/main.cpp.md)
  - Satellite View: Program entry point and main CLI loop.
  - Initializes runtime state, loads persisted graphs, and dispatches registration/sign-in flows.

---

## Authentication & User Persistence

- [Authentication.h](./Authentication/Authentication.h.md)
  - Satellite View: Authentication interface for validation, signup, signin, and account removal.
  - Bridges frontend UI and backend persistence logic.

- [authentication.cpp](./Authentication/authentication.cpp.md)
  - Satellite View: Implements validation rules, encrypted password handling, and registration flow.
  - Adds new users to storage and updates the shared search index and graph state.

- [FileManager.h](./File%20Management/FileManager.h.md)
  - Satellite View: Header-only definition of file storage operations and binary file layout.
  - Manages bucketed username storage and persistent graph file paths.

- [fileManager.cpp](./File%20Management/fileManager.cpp.md)
  - Satellite View: Implementation of user file reads/writes, sorted binary search, and graph persistence.
  - Handles email hash map serialization and friend graph file I/O.

---

## Friend Graph & Suggestion Engine

- [Graph.h](./Graph/Graph.h.md)
  - Satellite View: Friend relationship graph API and runtime suggestion graph declarations.
  - Defines adjacency list, relation status, and graph serialization interfaces.

- [graph.cpp](./Graph/graph.cpp.md)
  - Satellite View: Implements friendship requests, relationship queries, graph persistence, and friend suggestion generation.
  - Contains the randomized runtime suggestion graph and similarity scoring pipeline.

---

## User Model & Helpers

- [Helper.h](./Helper%20Class/Helper.h.md)
  - Satellite View: Shared utility declarations, suffix automaton search state, and global helper containers.
  - Defines the `Date` type, suffix automaton, and `UserHelper` shared runtime state.

- [helper.cpp](./Helper%20Class/helper.cpp.md)
  - Satellite View: Implements global helper state and password hash generation.
  - Initializes shared username search and email lookup helpers.

- [User.h](./User%20Class/User.h.md)
  - Satellite View: User domain model and serialization contract.
  - Declares profile fields, interest collections, and graph helper accessors.

- [user.cpp](./User%20Class/user.cpp.md)
  - Satellite View: Implements binary serialization for user persistence.
  - Delegates friend checks to the global graph and restores user objects from disk.

---

## Interest System

- [Interest.h](./User%20Posts%20Class/Interest.h.md)
  - Satellite View: Declares interest types, variant storage, and the recent-post stack.
  - Provides a flexible data model for books, movies, places, food, and sports.

- [interest.cpp](./User%20Posts%20Class/interest.cpp.md)
  - Satellite View: Implements interest serialization and stack operations.
  - Supports persistence of mixed interest variants and recent post history.

---

## UI Modules

- [UIAuthentication.h](./UI/Authentication/UIAuthentication.h.md)
  - Satellite View: Declares the registration and sign-in UI wrappers.
  - Connects console prompts to backend authentication.

- [uiAuthentication.cpp](./UI/Authentication/uiAuthentication.cpp.md)
  - Satellite View: Implements registration/sign-in input validation and interactive flows.
  - Displays success/error states and account recovery prompts.

- [UIHelpers.h](./UI/Helper%20Classes/UIHelpers.h.md)
  - Satellite View: Declares text UI layout helpers and box formatting functions.
  - Standardizes console output for all UI modules.

- [uiHelpers.cpp](./UI/Helper%20Classes/uiHelpers.cpp.md)
  - Satellite View: Implements console box rendering, padded line layout, and screen clearing.
  - Provides reusable UI components across the application.

- [UIInterest.h](./UI/User%20Posts/UIInterest.h.md)
  - Satellite View: Declares interest display helpers and filtering functions.
  - Connects `InterestVariant` data to UI rendering.

- [uiInterest.cpp](./UI/User%20Posts/uiInterest.cpp.md)
  - Satellite View: Implements dynamic grids, interest detail cards, and filtered post browsing.
  - Handles favorite and category-specific interest views.

- [UIUser.h](./UI/User/UIUser.h.md)
  - Satellite View: Declares dashboard, profile, search, and friend interaction workflows.
  - Defines the user-facing navigation surface.

- [uiUser.cpp](./UI/User/uiUser.cpp.md)
  - Satellite View: Implements the main dashboard, profile editing, posting, searching, and friend actions.
  - Manages user actions, friend request handling, and profile browsing.

---

## How to Use This Documentation

1. Start with [main.cpp](./Main%20File/main.cpp.md) to understand application initialization.
2. Review the authentication and persistence modules to learn how users are stored and validated.
3. Read the graph modules for friendship semantics and recommendation logic.
4. Explore the user model and interest modules to understand data structures and persistence.
5. Use the UI module docs when tracing actual runtime behavior and menu flows.

---

## Notes

- Each document offers a focused overview and linkable reference for a specific project component.
- For a complete API snapshot, follow the grouped sections above.
