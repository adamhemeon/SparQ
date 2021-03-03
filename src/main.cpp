/*
 * Adam Hemeon
 *
 * Data Structures: Linked List
 *
 *
 * This program uses the Linked List data structure in a simple text editor.
 * Keeps the entire text on a linked list, one line in each separate node.
 *
 *
 * Program is run by typing ./SparQ [filename] from the command line.
 *
 * Files to edit must be located in the same folder as the SparQ.exe,
 * by default this is the cmake-build-debug folder.
 */

#include <string>

#include "LinkedList.h"
#include "Editor.h"

// Using namespace
using namespace std;

// --------------------------------------------------------------------------------

/**
 * Summary: main routine for EDIT text editor
 *
 * Accepts command line arguments
 * Adapted from: https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
 *
 * @param argc
 * @param argv
 * @return error code
 */
int main(int argc, char **argv) {

    // Declare variables
    Editor editor; // editor object

    // Set the filename using the command line arguments.
    if (argc > 2) {

        cout << "EDIT provided with too many arguments." << endl;
        cout << "EDIT takes either no arguments or a valid filename as an argument." << endl;
        return 0;

    } else if (argc == 1) {

        // No command line arguments passed except for program name
        editor.currentLineNumber = 1;

    } else if (argc == 2) {

        // Set the filename using the command line arguments
        editor.myFileName = argv[1];

        //cout << "The filename to be edited is: '" << myFileName << "'" << endl; // TEST

        // Check if the filename is a valid file name.
        if (editor.isValidFileName(editor.myFileName)) {

            //cout << "Filename '" << myFileName << "' is valid." << endl; // TEST

            // If file is valid but no '.' exists, assume .txt as the file extension
            if (editor.myFileName.find('.') == string::npos) {

                //Add .txt as the file extension
                editor.myFileName += DefaultFileExt;

                //cout << "Default file extension " << DefaultFileExt << " added." << endl; // TEST
                //cout << "New file name is: '" << myFileName << "'" << endl; // TEST
            }

            // Add each line of the file to the list
            editor.populateListFromFile(editor.myFileName, &editor.list);

            // Get the current line from the number of lines
            editor.currentLineNumber = editor.list.getLineCount() + 1;

            // List the contents of the file when program starts for testing purposes.
            // cmdList(&list); // TEST
        }
    } else {
        return 1; // Exit with error code
    }

    // -----------------------------------------------------------------------------------

    // TODO: Extract into linePrompt function
    // Prompt for command or text to be added
    cout << editor.currentLineNumber << "> ";
    getline(cin, editor.currentLineInput);

    // Loop until E command is entered to save and exit
    while (!editor.exitCommandEntered(editor.currentLineInput, editor.myFileName, &editor.list)) {

        // Check if a command has been entered
        if (!editor.textCommandEntered(editor.currentLineInput, editor.ptrCurrentLineNumber, &editor.list, editor.ptrIsInsert)) {

            // Call to add the input as data in the linked list
            editor.addDataToList(editor.currentLineInput, editor.ptrCurrentLineNumber, &editor.list, editor.ptrIsInsert);
        }

        // Prefix with I #> when in Insert mode
        if (editor.isInsert) {
            cout << "I " << editor.currentLineNumber << "> ";
        } else {
            cout << editor.currentLineNumber << "> ";
        }

        // Wait for line input
        getline(cin, editor.currentLineInput);

    }

    return 0;
} // end main routine



