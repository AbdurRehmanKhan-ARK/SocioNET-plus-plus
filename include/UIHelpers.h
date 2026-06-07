// UIHelpers.h
// Basic UI helper functions for drawing boxes and formatting output
// Adapted from the static UI file to work with the backend

#ifndef UIHELPERS_H
#define UIHELPERS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using std::string;
using std::cout;

const int BOX_WIDTH = 80; // Fixed width for all UI boxes

// Clear the console screen
void clearScreen();

// Print the top border and title of a UI box
void printBoxHeader(const string& rawTitle);

// Print a horizontal separator line inside the box
void printBoxSeparator();

// Print the bottom border of the UI box
void printBoxFooter();

// Print a label-value pair with proper left alignment and right padding
void printPaddedLine(const string& rawLabel, const string& value = "");

// Print a centered text line inside the box
void printCenteredLine(const string& rawText);

// Pause execution until user presses Enter
void waitEnter();

#endif // UIHELPERS_H

