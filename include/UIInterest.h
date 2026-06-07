// UIInterest.h
// UI functions for displaying interests/posts
// Adapted to work with variant-based Interest system

#ifndef UIINTEREST_H
#define UIINTEREST_H

#include "User.h"
#include "Interest.h"
#include "UIHelpers.h"
#include <vector>


// Display interests in a dynamic grid layout (3 columns)
void printDynamicGrid(const User& user);

// Display a single interest/post in detailed format
void printInterestDetail(const InterestVariant& interest);

// Display all interests from InterestStack
void viewInterests(User& user);

// Display interests filtered by type
void viewInterestsByType(User& user, const string& typeChoice);

// Display favorite interests
void viewFavoriteInterests(const User& user);

// Helper function to get interest type name from variant
string getInterestTypeName(const InterestVariant& interest);

// Helper function to check if an interest is a favorite
bool isFavoriteInterest(const InterestVariant& interest, const User& user);

#endif // UIINTEREST_H

