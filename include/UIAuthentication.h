// UIAuthentication.h
// UI functions for user registration and sign-in
// Adapted to work with Authentication class

#ifndef UIAUTHENTICATION_H
#define UIAUTHENTICATION_H

#include "Authentication.h"
#include "UIHelpers.h"
#include "User.h"
#include <string>

// Register a new user (returns User object if successful, empty User if failed)
User registerUser(Authentication& auth);

// Sign in existing user (returns User object if successful, empty User if failed)
User signIn(Authentication& auth);

#endif // UIAUTHENTICATION_H

