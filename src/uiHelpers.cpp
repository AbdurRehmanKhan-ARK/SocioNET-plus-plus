// uiHelpers.cpp
// Implementation of basic UI helper functions for drawing boxes and formatting

#include "../include/UIHelpers.h"
#include <algorithm>
#include <cctype>
#include <windows.h>
using std::string;
using std::max;
using std::cin;
using std::numeric_limits;
using std::streamsize;
using std::endl;

void clearScreen() {
    system("cls");
}

void printBoxHeader(const string& rawTitle) {
    string title = rawTitle;
    bool cap = true;
    for (char& c : title) {
        if (cap && isalpha(c)) { c = toupper(c); cap = false; }
        else cap = isspace(c);
    }
    cout << "╔";
    for (int i = 0; i < BOX_WIDTH; ++i) cout << "═";
    cout << "╗\n";
    cout << "║";
    int visualLen = 0;
    for (char c : title) if ((c & 0xC0) != 0x80) ++visualLen;
    int left = max(0, (BOX_WIDTH - visualLen) / 2);
    for (int i = 0; i < left; ++i) cout << " ";
    cout << title;
    for (int i = 0; i < BOX_WIDTH - left - visualLen; ++i) cout << " ";
    cout << "║\n";
}

void printBoxSeparator() {
    cout << "\b╠";
    for (int i = 0; i < BOX_WIDTH; ++i) cout << "═";
    cout << "╣\n";
}

void printBoxFooter() {
    cout << "╚";
    for (int i = 0; i < BOX_WIDTH; ++i) cout << "═";
    cout << "╝\n";
}

void printPaddedLine(const string& rawLabel, const string& value) {
    string label = rawLabel;
    bool cap = true;
    for (char& c : label) {
        if (cap && isalpha(c)) { c = toupper(c); cap = false; }
        else cap = isspace(c);
    }
    string content = label + value;
    int visualLen = 0;
    for (char c : content) if ((c & 0xC0) != 0x80) ++visualLen;
    cout << "║ ";
    cout << content;
    for (int i = 0; i < BOX_WIDTH - visualLen - 1; ++i) cout << " ";
    cout << "║\n";
}

void printCenteredLine(const string& rawText) {
    string text = rawText;
    int visualLen = 0;
    for (char c : text) if ((c & 0xC0) != 0x80) ++visualLen;
    int left = max(0, (BOX_WIDTH - visualLen) / 2);
    cout << "║";
    for (int i = 0; i < left; ++i) cout << " ";
    cout << text;
    for (int i = 0; i < BOX_WIDTH - left - visualLen; ++i) cout << " ";
    cout << "║\n";
}

void waitEnter() {
    cout << "\n";
    bool cap = true;
    string msg = "Press Enter To Continue";
    for (char c : msg) {
        if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
        else { cout << c; cap = isspace(c); }
    }
    cout << endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

