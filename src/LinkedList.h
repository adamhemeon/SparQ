/*
 * Adam Hemeon
 *
 * Data Structures: Linked List
 *
 *
 * This program uses the Linked List data structure in a simple text editor.
 * Keeps the entire text on a linked list, one line in each separate node.
 *
 * LinkedList .h header file
 */

#ifndef ASSIGNMENT1_LINKEDLIST_H
#define ASSIGNMENT1_LINKEDLIST_H

#include <string>
// Internal data class
class Node {
public:
    int index; // to keep track of the line number
    std::string data; // the string data to be stored
    Node* next; // a pointer to the next node in the chain

    Node() : index(-1), data(), next(nullptr) {} // constructor
};

class LinkedList {

private:
    Node* start;

public:
    class iterator { // like STL C++ library used on vectors
    public:
        Node* node;

        // Constructors
        iterator() : node(nullptr) {}
        iterator(Node* node) : node(node) {}

        // Overloaded Ops
        std::string operator*(){ return node->data; }
        bool operator!=(iterator it){ return this->node != it.node; }
        bool operator==(iterator it){ return this->node == it.node; }
        iterator& operator++() { node = node->next; return *this; }
        // operator--() wouldn't be able unless we add prev pointers to each node
    };

    LinkedList();

    virtual ~LinkedList();

    void Add(int index, std::string data); // Insert on specified line
    void Delete(int index); // Delete by index
    void Insert( int before, int index, std::string data); // Insert before specified line

    int getLineCount();
    void reorderIndexes();

    friend std::ostream& operator<<(std::ostream& output, LinkedList& list);

    // Begin is a wrapper for start pointer
    iterator begin() { return iterator(start); }
    iterator end() { return iterator(nullptr); }

};


#endif //ASSIGNMENT1_LINKEDLIST_H

