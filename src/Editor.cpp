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

#include "Editor.h"

/**
 * Constructor
 */
Editor::Editor() : currentLineNumber(0) {}

/**
 * Destructor
 */
Editor::~Editor() {}

/**
 * Summary: Checks input string for a command as enum using regex.
 * For use in switch case statement that takes [E, L, L n, L n m, D, D n, D n m, I, I n] as commands.
 *
 * Adapted from: https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings
 * @param const string &input
 * @return enum command
 */
command Editor::checkCommand(const std::string &input) {

    // If the command is E, return right away.
    if (input == "E") return cmdE;

    // Else define regex expression for each command
    std::regex listExpr("[L]"); // Just an L
    std::regex listNumExpr("[L][\\s][0-9]+"); // L followed by whitespace followed by any number of digits
    std::regex listNumMExpr("[L][\\s][0-9]+[\\s][0-9]+"); // L -> space -> any num of digits -> space -> any num of digits

    std::regex deleteExpr("[D]");
    std::regex deleteNumExpr("[D][\\s][0-9]+");
    std::regex deleteNumMExpr("[D][\\s][0-9]+[\\s][0-9]+");

    std::regex insertExpr("[I]");
    std::regex insertNumExpr("[I][\\s][0-9]+");

    // Return corresponding enum for the expression
    if (regex_match(input, listExpr)) return cmdL;
    if (regex_match(input, deleteExpr)) return cmdD;
    if (regex_match(input, insertExpr)) return cmdI;
    if (regex_match(input, listNumExpr)) return cmdLn;
    if (regex_match(input, deleteNumExpr)) return cmdDn;
    if (regex_match(input, insertNumExpr)) return cmdIn;
    if (regex_match(input, listNumMExpr)) return cmdLnm;
    if (regex_match(input, deleteNumMExpr)) return cmdDnm;

    return cmdNone;
} // end checkCommand method

/**
 * Summary: Checks the input string for a command.
 * [L, L n, L n m, D, D n, D n m, I, I n] as commands.
 *
 * Parses the command then calls the corresponding function.
 *
 * @param const string &input
 * @param int *currentLineNumber
 * @param LinkedList *list
 * @param bool *isInsert
 * @return true if input has been recognized as a command.
 */
bool Editor::textCommandEntered(const std::string &input, int *currentLineNumber, LinkedList *list, bool *isInsert) {

    // Define string stream and variables to parse the command into it's letter and int value(s).
    // Adapted from: https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
    std::istringstream ss(input);
    std::string cmd;
    int n = 0;
    int m = 0;

    // Check if the input is a command, parse the command, call the corresponding function.
    switch (checkCommand(input)) {
        case cmdL:

            // Call the List command
            cmdList(list);

            return true;

        case cmdLn:

            // Retrieve the command and the number(s) from the string stream
            ss >> cmd;
            ss >> n;

            // Call the corresponding function
            cmdList(n, list);

            return true;

        case cmdLnm:
            ss >> cmd;
            ss >> n;
            ss >> m;
            cmdList(n, m, list);
            return true;
        case cmdD:
            cmdDelete(currentLineNumber, list);
            return true;
        case cmdDn:
            ss >> cmd;
            ss >> n;
            cmdDelete(n, currentLineNumber, list);
            return true;
        case cmdDnm:
            ss >> cmd;
            ss >> n;
            ss >> m;
            cmdDelete(n, m, currentLineNumber, list);
            return true;
        case cmdI:
            cmdInsert(currentLineNumber, list, isInsert);
            return true;
        case cmdIn:
            ss >> cmd;
            ss >> n;
            cmdInsert(n, currentLineNumber, list, isInsert);
            return true;
        default:
            return false; // Input is not a valid command
    }
} // end textCommandEntered method

/**
 * Summary: Used as text editor loop control, executes a save and exit
 *          before returning true and ending the program.
 *
 *          For command [E].
 *
 * @param const string &input
 * @param const string &filename
 * @param LinkedList *list
 * @return true if E command entered
 */
bool Editor::exitCommandEntered(const std::string &input, const std::string &filename, LinkedList *list) {

    // If the command entered matched the E enum
    if (checkCommand(input) == cmdE) {

        // Run the save and exit function
        cmdExit(filename, list);

        return true;
    }
    return false;
} // end exitCommandEntered method

/**
 * Summary: This function implements the save & exit command.
 * Result of switch case statement for [E] command.
 *
 * Prompts user to provide a valid filename if no filename exists.
 *
 * Calls the saveWriteFile() function.
 *
 * @param string filename
 * @param LinkedList *list
 */
void Editor::cmdExit(std::string filename, LinkedList *list) {

    // If the filename is currently empty
    if (filename.empty()) {
        // Validate user input to accept valid windows filenames
        do {
            std::cout << "Enter filename: ";
            getline(std::cin, filename);

            // Check if there is no '.' in the filename
            if ((count(filename.begin(), filename.end(), '.') == 0)) {

                // Add the .txt extension if none exists.
                filename += DefaultFileExt;
            }

            // If filename exists, ask to overwrite
            if (isFileExists(filename)) {

                // Declare variable to hold user input
                std::string overwriteFlag;

                std::cout << "\nFile: " << filename << " already exists." << std::endl;

                // Get user input, end when Y or N is entered.
                do {
                    std::cout << "Would you like to overwrite it? (Y/N) ";
                    getline(std::cin, overwriteFlag);
                } while (overwriteFlag.find('Y') == std::string::npos && overwriteFlag.find('N') == std::string::npos);

                // Reset the filename to blank if the user does not choose Y to overwrite
                if (overwriteFlag.find('Y') == std::string::npos) {
                    filename = ""; // resets the outer do while loop
                }
            }
        } while (!isValidFileName(filename));
    }

    // Save the contents of the list to a named file
    saveWriteFile(filename, list);
} // end cmdExit method

/**
 * Summary: This function implements the list command with no index params.
 * Result of switch case statement for [L] command.
 *
 * @param LinkedList *list
 */
void Editor::cmdList(LinkedList *list) {
    //cout << "Reached cmdList(list)" << endl; // TEST

    // Loop through the entire list
    for (LinkedList::iterator i = list->begin(); i != list->end(); ++i) {

        // Print out each node of the list
        std::cout << i.node->index << "> "; // Line number
        std::cout << *i << std::endl; // String data in the node
    }
} // end cmdList method

/**
 * Summary: This function implements the list command with one index param.
 * Result of switch case statement for [L n] command.
 *
 * @param int n
 * @param LinkedList *list
 */
void Editor::cmdList(int n, LinkedList *list) {
    //cout << "Reached cmdList(n, list)" << endl; // TEST
    //cout << "n = " << n << endl; // TEST

    // Get the line count before the loop runs
    int lineCount = list->getLineCount();

    // Check that the line number to list is within the bounds of the list
    if (n > 0 && n <= lineCount) {

        // Loop through the list
        for (LinkedList::iterator i = list->begin(); i != list->end(); ++i) {

            // Print the line if the index matches the line number
            if (n == i.node->index) {
                std::cout << i.node->index << "> "; // Line number
                std::cout << *i << std::endl; // String data in the node
            }
        }
    }
} // end cmdList method

/**
 * Summary: This function implements the list command with two index params.
 * Result of switch case statement for [L n m] command.
 *
 * @param int n
 * @param int m
 * @param LinkedList *list
 */
void Editor::cmdList(int n, int m, LinkedList *list) {
    //cout << "Reached cmdList(n, m, list)" << endl; // TEST
    //cout << "n = " << n << " m = " << m << endl; // TEST

    // Only attempt the command if n is less than m
    if (n < m) {

        // Get the line count before the loop runs
        int lineCount = list->getLineCount();

        // If n is less than 1, set to 1
        if (n < 1) { n = 1;}

        // If n is within the bounds of the list
        if (n >= 1 && n <= lineCount) {

            // Set the m value to the end of the list if it's currently out of bounds
            if (m > lineCount) { m = lineCount; }

            // Loop through the list
            for (LinkedList::iterator i = list->begin(); i != list->end(); ++i) {

                // Print the line if the index is between the specified line numbers
                if (i.node->index >= n && i.node->index <= m) {
                    std::cout << i.node->index << "> ";
                    std::cout << *i << std::endl;
                }
            }
        }
    }
} // end cmdList method

/**
 * Summary: This function implements the delete command with no index param.
 * Result of switch case statement for [D] command.
 *
 * @param int *currentLineNumber
 * @param LinkedList *list
 */
void Editor::cmdDelete(int *currentLineNumber, LinkedList *list) {
    //cout << "Reached cmdDelete(list)" << endl; // TEST

    // Don't allow delete at line 1 (no nodes present)
    if (*currentLineNumber > 1) {

        // Find the line to delete as the last node in the chain
        int lineToDelete = list->getLineCount();

        //cout << "Line to delete: " << lineToDelete << endl; // TEST

        // Delete the final node in the chain
        list->Delete(lineToDelete);

        // Set the current line to what was the end of the chain
        *currentLineNumber = lineToDelete;
    }
} // end cmdDelete method

/**
 * Summary: This function implements the delete command with one index param.
 * Result of switch case statement for [D n] command.
 *
 * @param int n
 * @param currentLineNumber
 * @param LinkedList *list
 */
void Editor::cmdDelete(int n, int *currentLineNumber, LinkedList *list) {
    //cout << "Reached cmdDelete(n, list)" << endl; // TEST
    //cout << "n = " << n << endl; // TEST
    //cout << "Line to delete: " << n << endl; // TEST

    // Get the line count before the line runs
    int lineCount = list->getLineCount();

    // Check that the line number to delete is within the bounds of the list
    if (n > 0 && n <= lineCount) {

        // Delete the node that matches the command index
        list->Delete(n);

        // Reorder the indexes
        list->reorderIndexes();

        // Reset the current line number to the old line count (one has been removed)
        *currentLineNumber = lineCount;
    }
} // end cmdDelete method

/**
 * Summary: This function implements the delete command with two index params.
 * Result of switch case statement for [D n m] command.
 *
 * Delete only nodes that exist
 * Fix the n and m values if outside the bounds of the list.
 *
 * @param int n
 * @param int m
 * @param int *currentLineNumber
 * @param LinkedList *list
 */
void Editor::cmdDelete(int n, int m, int *currentLineNumber, LinkedList *list) {
    //cout << "Reached cmdDelete(n, m, list)" << endl; // TEST
    //cout << "n = " << n << " m = " << m << endl; // TEST
    //cout << "Lines to delete: " << n << " to "<< m << endl; // TEST

    // Only attempt to run the command if the first number is less than the second number
    if (n < m) {

        // Get the line count before the loop runs
        int lineCount = list->getLineCount();

        // If n is less than 1, set to 1
        if (n < 1) { n = 1; }

        // If n is within the bounds of the list
        if (n >= 1 && n <= lineCount) {

            // Set the m value to the end of the list if it's currently out of bounds
            if (m > lineCount) { m = lineCount; }

            // cout << "Lines to delete: " << n << " to "<< m << endl; // TEST

            // Delete each node between n and m inclusive
            for (int i = n; i <= m; ++i) {

                // Delete the node at i
                list->Delete(i);

                // Decrement the lineCount
                lineCount--;
            }

            // Reorder the indexes
            list->reorderIndexes();

            // Reset the current line number
            *currentLineNumber = lineCount + 1;
        }
    }
} // end cmdDelete method

/**
 * Summary: This function implements the insert command.
 * Result of switch case statement for [I] command.
 *
 * @param int *currentLineNumber
 * @param LinkedList *list
 * @param bool *isInsert
 */
void Editor::cmdInsert(int *currentLineNumber, LinkedList *list, bool *isInsert) {
    //cout << "Reached cmdInsert(list)" << endl; // TEST

    // Get the line count before the loop runs
    int lineCount = list->getLineCount();

    // If not already in insert mode
    if (!(*isInsert)) {

        // Set the bool pointer to true
        *isInsert = true;

        // Set the line number to the last node
        *currentLineNumber = lineCount;

    } else {

        // Leave insert mode
        *isInsert = false;

        // Reset the line number to the end of the list
        *currentLineNumber = lineCount + 1;
    }
} // end cmdInsert method

/**
 * Summary: This function implements the insert command with one index param.
 * Result of switch case statement for [I n] command.
 *
 * Note to Instructor: This function will set things up to add ON line n (assignment spec says BEFORE line n
 * and seems to replace the contents of that line, but I found it more useful to me if I could specify which
 * line I was to insert on and push that line one forward.
 *
 * @param int n
 * @param int *currentLineNumber
 * @param LinkedList *list
 * @param bool *isInsert
 */
void Editor::cmdInsert(int n, int *currentLineNumber, LinkedList *list, bool *isInsert) {
    //cout << "Reached cmdInsert(n, list)" << endl; // TEST
    //cout << "n = " << n << endl; // TEST

    // Get the line count before the command runs
    int lineCount = list->getLineCount();

    // Check that the line number to make sure the line to insert is within the bounds of the list
    if (n > 0 && n <= lineCount) {

        // If not already in insert mode
        if (!(*isInsert)) {

            // Set the bool pointer to true
            *isInsert = true;

            // Set the line number to the n index
            *currentLineNumber = n;
        }
    }
} // end cmdInsert method

/**
 * Summary: Adds the data to the linked list.
 * Switches on Add() or Insert() based on bool isInsert.
 *
 * @param const string &userInputString
 * @param int *currentLineNumber
 * @param LinkedList *list
 * @param bool *isInsert
 */
void Editor::addDataToList(const std::string &userInputString, int *currentLineNumber, LinkedList *list, bool *isInsert) {

    // If the function was called without insert flag
    if (!(*isInsert)) {

        // Add the data to the list using list->Add()
        list->Add(*currentLineNumber, userInputString);

        // Increment the line number by one
        (*currentLineNumber)++;

    } else {

        //cout << "Insert mode: " << *isInsert << endl; // TEST
        //cout << "Inserting at line: " << *currentLineNumber << endl; // TEST

        // Add the data to the list using list->Insert()
        list->Insert(*currentLineNumber, *currentLineNumber, userInputString);

        // Reorder the indexes
        list->reorderIndexes();

        // Reset the line number to the end
        *currentLineNumber = list->getLineCount() + 1;

        // Reset the isInsert flag
        *isInsert = false;
    }
} // end addDataToList method

/**
 * Summary: Takes in a filename as a string. Validates to see if the filename meets Windows file naming standards.
 * Will accept filenames with either none, or no more than one '.' indicating a file extension.
 * Cited from: https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file
 *
 * Possible Improvement: This function could instead use regex.
 * Example: else if (regex_match (filename,InvalidWindowsFileExpr)){}
 *
 * @param const string& filename
 * @return true if filename is valid.
 * */
bool Editor::isValidFileName(const std::string &filename) {

    // Check if filename is a valid windows file name avoiding Windows reserved characters (<>:"/\|?*)
    if (filename.empty()) {

        //cout << "Filename cannot be empty." << endl; // TEST
        return false;

        // Check if the filename contains a Windows reserved character
    } else if ((filename.find('<') != std::string::npos) || (filename.find('>') != std::string::npos) ||
               (filename.find(':') != std::string::npos) || (filename.find('/') != std::string::npos) ||
               (filename.find('\\') != std::string::npos) || (filename.find('|') != std::string::npos) ||
               (filename.find('?') != std::string::npos) || (filename.find('*') != std::string::npos) ||
               (filename.find('*') != std::string::npos) || (filename.find('"') != std::string::npos)) {
        std::cout << "Filename cannot contain Windows reserved characters (<>:\"/\\|?*)" << std::endl;
        return false;

        // Check if the filename contains more than one '.'
    } else if ((count(filename.begin(), filename.end(), '.') > 1)) {

        std::cout << "Filename cannot contain more than one '.'" << std::endl;
        return false;
    }

    return true; // filename is valid
} // end isValidFileName method

/**
 * Summary: Takes a file name as a string and checks if the file exists.
 *
 * Adapted from: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 *
 * @param const string &filename
 * @return true if the file currently exists
 */
bool Editor::isFileExists(const std::string &filename) {

    // Check file in stream
    std::ifstream infile(filename);

    // Return true if file exists
    return infile.good();
} // end isFileExists method

/**
 * Summary: Takes in a file, reads its contents, and adds each line to a linked list.
 * Run on program startup when a filename is passed to the program as a command line argument.
 *
 * @param const string &filename
 * @param LinkedList *list
 * */
void Editor::populateListFromFile(const std::string &filename, LinkedList *list) {

    std::string thisLine; // used to store each line of the in file as it is looped through
    int lineNumber = 0; // keep track of the line number

    // Declare stream object
    std::ifstream myFileIn;

    // Check if the file exists
    if (isFileExists(filename)) {

        // Attempt to open files
        try {
            // Connect file to stream object
            myFileIn.open(filename);

            // Print message if file fails / successfully opens
            if (myFileIn.fail()) {
                std::cout << "File failed to open" << std::endl;
                throw FileFailedToOpenException();
            } else {
                //cout << "File '" << filename <<"' Open" << endl; // TEST

                // attempt to read file to the linked list
                try {
                    // Get in file contents line by line
                    // If the file is not empty, add each line to a linked list
                    while (!myFileIn.eof()) {

                        lineNumber++; // increment the line number before populating the linked list
                        getline(myFileIn, thisLine);

                        // Populate the Linked List line by line
                        list->Add(lineNumber, thisLine);
                    }

                    //cout << "Linked List has been populated." << endl; // TEST

                }
                catch (std::bad_exception &e) {
                    std::cout << "An unexpected error occurred populating the list." << std::endl;
                    std::cout << e.what() << std::endl;
                }
                catch (std::exception &e) {
                    std::cout << "An error occurred populating the list." << std::endl;
                    std::cout << e.what() << std::endl;
                }

                try {
                    // close file resources
                    myFileIn.close();

                    if (myFileIn.is_open()) {
                        throw FileFailedToCloseException();
                    }

                    //cout << "File '" << filename <<"' Closed" << endl; // TEST

                }
                catch (FileFailedToCloseException &e) {
                    std::cout << e.what() << std::endl;
                }
                catch (std::exception &e) {
                    std::cout << "An error occurred closing the file." << std::endl;
                    std::cout << e.what() << std::endl;
                }
            }
        }
        catch (FileFailedToOpenException &e) {
            std::cout << e.what() << std::endl;
        }
        catch (std::system_error &e) {
            std::cout << e.what() << std::endl;
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } else {
        // cout << "File " << filename << " does not yet exist." << endl; // TEST
    }

} // end populateListFromFile method

/**
 * Summary: Writes the contents of the linked list to file.
 *
 * Outfile is overwritten and not appended.
 *
 * @param const string &filename
 * @param LinkedList *list
 * */
void Editor::saveWriteFile(const std::string &filename, LinkedList *list) {

    std::string thisLine; // used to store each line of the in file as it is looped through

    // Declare stream object
    std::ofstream myFileOut;

    // Attempt to open file
    try {
        // Connect files to stream objects
        // Will create a new file if the file doesn't currently exist
        myFileOut.open(filename); // outfile

        // Print message if file fails / successfully opens
        if (myFileOut.fail()) {
            std::cout << "Output File failed to open" << std::endl;
            throw FileFailedToOpenException();
        } else {
            //cout << "File '" << filename <<"' Open" << endl; // TEST
            std::cout << "Writing... ";

            // Attempt to read/write to file
            try {
                // Variable to keep track of when the iterator has reached it's final iteration
                int loopCount = 0;
                int lineCount = list->getLineCount();

                //For each node in the linked list using iterator, send to file out stream
                for (LinkedList::iterator i = list->begin(); i != list->end(); ++i) {

                    // Increment the loop counter
                    loopCount++;

                    // If the loop has not yet reached the end
                    if (loopCount != lineCount) {
                        myFileOut << *i << std::endl; // add a carriage return
                    } else {
                        myFileOut << *i; // on final line, don't add the carriage return
                    }
                }

                std::cout << "Complete!" << std::endl;

            }
            catch (std::bad_exception &e) {
                std::cout << "An unexpected error occurred writing to file." << std::endl;
                std::cout << e.what() << std::endl;
            }
            catch (std::exception &e) {
                std::cout << "An error occurred writing to file." << std::endl;
                std::cout << e.what() << std::endl;
            }

            try {
                // Close file resources
                myFileOut.close();

                if (myFileOut.is_open()) {
                    throw FileFailedToCloseException();
                }

                //cout << "File '" << filename <<"' Closed" << endl; // TEST

            }
            catch (FileFailedToCloseException &e) {
                std::cout << e.what() << std::endl;
            }
            catch (std::exception &e) {
                std::cout << "An error occurred closing the file." << std::endl;
                std::cout << e.what() << std::endl;
            }
        }
    }
    catch (FileFailedToOpenException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::system_error &e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}


// end saveWriteFile method