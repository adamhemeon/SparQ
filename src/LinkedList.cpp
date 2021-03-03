/*
 * Adam Hemeon
 *
 * Data Structures: Linked List
 *
 *
 * This program uses the Linked List data structure in a simple text editor.
 * Keeps the entire text on a linked list, one line in each separate node.
 *
 * LinkedList .cpp implementation file
 */

#include "LinkedList.h"
#include <iostream>
#include <string>

// Constructor
LinkedList::LinkedList() : start(nullptr) {

}

// Destructor
LinkedList::~LinkedList() {
    Node* node = start;

    while (node != nullptr) {
        Node* temp = node;
        node = node->next;
        delete temp;
    }
}

/**
 * Summary: Adds a node to the end of the linked list.
 *
 * @param int index
 * @param string data
 */
void LinkedList::Add(int index,std::string data) {

    Node* newNode = new Node();
    newNode->index = index;
    newNode->data = data;

    if (start == nullptr) {
        // start a new chain
        start = newNode;
    } else {   // start pointer isn't null
        // add to the end of an existing chain
        Node* node = start;
        Node* prev = nullptr;

        // Loop until the end of the chain
        while (node != nullptr) {

            // set prev pointer to the node we were just at
            prev = node;

            // move to next node
            node = node->next;
        }

        // if the end was found, attach new node
        if (prev != nullptr) {
            prev->next = newNode;
        }
    }
}

/**
 * Summary: Deletes a node at specified index.
 *
 * @param int index
 */
void LinkedList::Delete(int index) {
    // find the node
    // note the next pointer
    // update previous node's next pointer
    // delete the disconnected node

    Node* node = start;
    Node* prev = nullptr;

    // navigate through the chain and find the node to delete
    while (node != nullptr) {

        if (node->index == index) {
            // found the node, so exit the loop
            break;
        }

        prev = node;
        node = node->next;
    }

    // was the node found?
    if (node != nullptr) {
        // Are we deleting the start node?
        if(prev == nullptr) { // or is node == start
            // point the start to the second node
            start = node->next;
        } else {
            // deleting any other node but the first
            prev->next = node->next; // detaching the node
        }

        delete node; // THEN WE CAN DELETE IT

    } else {
        std::cout << "Node not found." << std::endl;
    }
}

/**
 * Summary: Inserts a node at the before index, pushes all nodes forward in the chain.
 *
 * @param int before
 * @param int index
 * @param string data
 */
void LinkedList::Insert(int before, int index, std::string data) {

    Node* newNode = new Node();
    newNode->index = index;
    newNode->data = data;

    Node* node = start;
    Node* prev = nullptr;

    // finding node to insert before
    while (node != nullptr) {

        if(node->index == before) {
            // found node, so exit loop
            break;
        }
        prev = node;
        node = node->next;
    }

    // did we find the node?
    if (node != nullptr) {
        // are we inserting before the first node?
        if (prev == nullptr) { // or is node == start

            // make the new node the new starting node
            newNode->next = start;
            start = newNode;
        } else {
            // insert the new node in the chain
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }
}

// Overloaded output operator
std::ostream &operator<<(std::ostream &output, LinkedList &list) {

    Node* node = list.start;

    // Print line number followed by the node's string data and end the line
    while (node != nullptr) {
        output << node->index << "> " << node->data << std::endl;

        node = node->next;
    }
    return output;  // chain them together like strings
}

/**
 * Summary: Returns how many nodes or "lines" are in the chain.
 *
 * @return How many nodes are in the chain.
 */
int LinkedList::getLineCount() {

    int lineCount = 0;

    // Loop through the list and increment the counter
    for(LinkedList::iterator i = this->begin(); i != this->end(); ++i) {

        lineCount++;
    }

    return lineCount; // how many nodes are in the chain
}

/**
 * Summary: Reorders the index values of the linked list nodes to help maintain data integrity.
 */
void LinkedList::reorderIndexes() {

    int currentLine = 0;

    // Loop through the list
    for(LinkedList::iterator i = this->begin(); i != this->end(); ++i) {

        currentLine++;

        // Set the node's index value to the current line
        i.node->index = currentLine;
    }
}



