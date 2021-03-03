/*
 * Adam Hemeon
 *
 * Data Structures: Linked List
 *
 *
 * This program uses the Linked List data structure in a simple text editor.
 * Keeps the entire text on a linked list, one line in each separate node.
 *
 * Program is run by typing ./SparQ [filename] from the command line.
 *
 * Files to edit must be located in the same folder as the SparQ.exe,
 * by default this is the cmake-build-debug folder.
 */

#ifndef SPARQ_EDITOR_H
#define SPARQ_EDITOR_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <exception>
#include <regex>
#include <sstream>

#include "LinkedList.h"

// Enum Commands
enum command {
    cmdL,
    cmdLn,
    cmdLnm,
    cmdD,
    cmdDn,
    cmdDnm,
    cmdI,
    cmdIn,
    cmdE,
    cmdNone
};

// Constants
std::string const DefaultFileExt = ".txt";
//regex const InvalidWindowsFileExpr("[\\<\\>\\:\\\"\\/\\\\\\|\\?\\*]");
//regex const InvalidWindowsFileExpr(R"([\<\>\:\"\/\\\|\?\*])");

class Editor {

public:

    // Variables
    LinkedList list; // custom Linked List object
    std::string myFileName; // filename from command line args
    std::string currentLineInput; // string from user input
    int currentLineNumber; // maintain current line number as index
    int *ptrCurrentLineNumber = &currentLineNumber; // pointer to expose currentLineNumber to functions
    bool isInsert = false; // used to differentiate user input as an Add() or an Insert()
    bool *ptrIsInsert = &isInsert; // pointer to expose isInsert to functions

    // Constructors
    Editor();
    virtual ~Editor();

    // Methods
    bool isFileExists(const std::string &);
    bool isValidFileName(const std::string &);
    void populateListFromFile(const std::string &, LinkedList *);
    void saveWriteFile(const std::string &, LinkedList *);
    command checkCommand(const std::string &);
    bool textCommandEntered(const std::string &, int *, LinkedList *, bool *);
    bool exitCommandEntered(const std::string &, const std::string &, LinkedList *);
    void cmdExit(std::string, LinkedList *);
    void cmdList(LinkedList *);
    void cmdList(int, LinkedList *list);
    void cmdList(int, int, LinkedList *list);
    void cmdDelete(int *, LinkedList *);
    void cmdDelete(int, int *, LinkedList *);
    void cmdDelete(int, int, int *, LinkedList *);
    void cmdInsert(int *, LinkedList *, bool *);
    void cmdInsert(int, int *, LinkedList *, bool *);
    void addDataToList(const std::string &, int *, LinkedList *, bool *);
};

// Custom Exceptions
struct FileFailedToOpenException : public std::exception {
public:
    const std::string what() {
        return "Unable to open file.";
    }
};//end FileFailedToOpenException struct

struct FileFailedToCloseException : public std::exception {
public:
    const std::string what() {
        return "Unable to close file.";
    }
};//end FileFailedToCloseException struct

#endif //SPARQ_EDITOR_H
