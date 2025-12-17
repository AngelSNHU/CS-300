//============================================================================
// Name        : Project Two.cpp
// Author      : Angel Esparza
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project Two Code
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

//sets and programs a menu, allowing user to access the different functions of the program
void DisplayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit." << endl << endl;
    cout << "What would you like to do? ";
}

//sets the course structure code/parameters that holds all of the course info
struct Course {
    string courseId;
    string courseName;
    vector<string> preRequisite;
};

//tree node (internal structure) 
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() { // The Default Constructor
        left = nullptr;
        right = nullptr;
    }
    //initialization of course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }

};


//set class (containing data members) & setting up binary search tree
class BinarySearchTree {

private:

    void Destruct(Node* node);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string courseId);
    void PrintCourse(Node* node);
};

//Default Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

//destructor
BinarySearchTree::~BinarySearchTree() {
    Destruct(root);
};

//destructs left and right pointer nodes and then deletes them
void BinarySearchTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        node->left = nullptr;
        Destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};


//function to search for a course
void BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            //functions that print the course id and course name
            cout << currentNode->course.courseId << ", ";
            cout << currentNode->course.courseName;
            cout << endl; //newline
            cout << "Prerequisites: ";
            //function to also print the prerequisites
            for (string preRequisite : currentNode->course.preRequisite) {
                if (preRequisite == currentNode->course.preRequisite.back()) {

                    cout << preRequisite << endl;
                }
                else {
                    cout << preRequisite << ", ";
                }
            }

            return;
        }
        //function to search left pointer (if not found)
        else if (courseId < currentNode->course.courseId) {

            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        //function to search right pointer (if not found)
        else {

            currentNode = currentNode->right;
        }
    }
    //statement to print if course isn't on the list/not found
    cout << "Course " << courseId << "not found. " << endl;
    return;
}

//inserts course into a course list
void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {

    if (tree->root == nullptr) {
        tree->root = node;
    }
    else {
        Node* curr = tree->root;
        while (curr != nullptr) {

            if (node->course.courseId < curr->course.courseId) {
                if (curr->left == nullptr) {
                    curr->left = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->left;
                }
            }
            else {

                if (curr->right == nullptr) {
                    curr->right = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->right;
                }
            }

        }

    }
}
//prints course list
void BinarySearchTree::PrintCourse(Node* node) {

    //Traverse and print entire tree
    if (node == nullptr) {
        return;
    }

    //function that looks at left node then will print the course id and course name then look at right node
    PrintCourse(node->left);
    cout << node->course.courseId << ", ";
    cout << node->course.courseName << endl;
    PrintCourse(node->right);
};

//function that allows user to load file 
void loadCourse(string filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (file.is_open()) {
        cout << "File loaded." << endl; // notify them with a message that file was loaded

        int num;
        string line;
        string word;

        while (getline(file, line)) {

            num = 0;
            Node* node = new Node();
            stringstream str(line);

            while (num < 2) {
                getline(str, word, ',');
                if (num == 0) {
                    node->course.courseId = word;
                }
                else {
                    node->course.courseName = word;
                }
                num++;
            }
            while (getline(str, word, ',')) {
                node->course.preRequisite.push_back(word);
            }

            bst->Insert(bst, node); //function that inserts node into bst
        }
    }
    else {
        cout << "File error, please try again. " << endl; //function that prints file error if the file isn't found
        return;
    }

}

void main() {

    BinarySearchTree* bst = new BinarySearchTree();

    string fileChoice;
    string courseChoice;

    int userInput = 0;

    cout << "Welcome to the course planner." << endl << endl;
    //this is the functions that set while loop to allow the program to stay open when the choice isn't 9 (exit)
    while (userInput != 9) {
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {
            //loads file that user chooses and prompts what file they would like to choose
        case 1:
            cout << endl; //newline
            cout << "What is the filename you would like to load? ";
            cin >> fileChoice;

            loadCourse(fileChoice, bst);
            cout << endl; //newline
            break;

            //function to print the respective courses in alphanumeric order
        case 2:
            cout << "Here is a sample schedule: " << endl << endl;
            cout << "COURSE ID | COURSE NAME" << endl << endl;
            bst->PrintCourse(bst->root); //prints course to start at root
            cout << endl;
            break;

            //function so that it prints course that user inputed, along with the prerequisites
        case 3:
            cout << endl;
            cout << "What course do you want to know about? ";
            cin >> courseChoice;
            cout << endl;

            std::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
            bst->Search(courseChoice);

            cout << endl;
            break;

            //function that prints exit message and then exits the program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

            //message if no valid choice is entered
        default:
            cout << userInput << " is not a valid option." << endl << endl;
            break;
        }
    }
}