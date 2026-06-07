// UIUser.h
// UI functions for user profile, dashboard, and post management
// Adapted to work with backend User class

#ifndef UIUSER_H
#define UIUSER_H

#include "User.h"
#include "UIHelpers.h"
#include "UIInterest.h"
#include "Interest.h"
#include <string>

// Main dashboard menu
void dashboard(User& currentUser);

// View and edit user profile
void viewProfile(User& currentUser);
void editProfile(User& currentUser);

// Delete a signed-in user's profile after confirming password
bool deleteProfile(User& currentUser);

// Add a new interest/post
void addInterest(User& user);

// Search for other users (placeholder - requires FileManager/Graph)
void searchUser(User& currentUser);

// Friend-related functions (placeholders - require Graph implementation)
void viewFriends(const User& user);
void viewFriendRequests(User& currentUser);
void viewMyFriends(User& currentUser);
void viewMutualFriends(User& currentUser, User& otherUser);
void showFriendSuggestions(User& currentUser);
void viewOtherProfile(User& currentUser, User& otherUser);

#endif // UIUSER_H

