//============================================================================
// Name        : Source.cpp
// Author      : Ryan Berch
// Version     : 1.0
//============================================================================

#include <iostream>
#include <time.h>
#include<algorithm>
#include "CSVparser.h"

using namespace std;

// define a structure to hold course information
struct Course {
    string ID; // unique identifier
    string Name;
    vector<string> Prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* parent;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid AND PARENT NODE
    Node(Course aCourse, Node* aParent) :
            Node() {
        course = aCourse;
        parent = aParent;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    Node* GetRoot();
    BinarySearchTree();
    ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Deconstruct(Node* node);
    Course Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

//Public Deconstructor
BinarySearchTree::~BinarySearchTree() {
    //Calls private Deconstructor and sets the root to nullptr
    BinarySearchTree::Deconstruct(root);
    root = nullptr;
}

void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}

Node* BinarySearchTree::GetRoot() { return root; }

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder left
        inOrder(node->left);
        //output course ID and Name
        cout << node->course.ID << ", " << node->course.Name << endl;
        //InOrder right
        inOrder(node->right);
    }
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    // FIXME (5) Implement inserting a bid into the tree
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course, nullptr);
    }
    // else
    else {
        // add Node root and bid
        this->addNode(root, course);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    //if node's course is greater than course
    if (node != nullptr && node->course.ID.compare(course.ID) > 0) {
        //If node's left is null
        if (node->left == nullptr) {
            //Insert course into new node at node's left, passing current node as the parent
            node->left = new Node(course, node);
            return;
        }
        //else node's left not null
        else {
            //recursive call to add node after node's left, passing current node as the parent
            this->addNode(node->left, course);
        }
    }
    //else node's course is greater than bid
    else if (node != nullptr && node->course.ID.compare(course.ID) < 0) {
        //if the node's right is not null
        if (node->right == nullptr) {
            //New node with course at node's right
            node->right = new Node(course, node);
            return;
        }
        //else node's right not null
        else {
            //recursive call to insert course after node's right
            this->addNode(node->right, course);
        }
    }
}

Course BinarySearchTree::Search(string Id) {
    // set current node equal to root
    Node* cur = root;
    // keep looping downwards until bottom reached or matching Id found
    while (cur != nullptr) {
        // if match found, return current course
        if (cur->course.ID.compare(Id) == 0) {
            return cur->course;
        }
        // if course is smaller than current node then traverse left
        else if (Id.compare(cur->course.ID) < 0) {
            cur = cur->left;
        }
        // else larger so traverse right
        else {
            cur = cur->right;
        }

    }

    Course emptyCourse;
    return emptyCourse;
}

//Private deconstructor which deletes all nodes recursively
void BinarySearchTree::Deconstruct(Node* node) {
    if (node->left != nullptr) {
        Deconstruct(node->left);
    }
    if (node->right != nullptr) {
        Deconstruct(node->right);
    }
    delete node;
}

//Method to display a course's ID, name, and prerequisites
void displayCourse(Course course) {
    cout << course.ID << ", " << course.Name << endl;
    if (course.Prerequisites.size() > 0) {
        cout << "Prerequisites: ";
        for (int i = 0; i < course.Prerequisites.size(); ++i) {
            if (i == course.Prerequisites.size() - 1) {
                cout << course.Prerequisites.at(i) << endl;
            }
            else {
                cout << course.Prerequisites.at(i) << ", ";
            }
        }
    }
    cout << endl;
}


//Function to load courses from file with csvPath into tree bst
void loadCourses(string csvPath, BinarySearchTree* bst) {
    int j = 2;
    
    cout << endl << "Loading CSV file: " << csvPath << endl;

    // initialize the CSV Parser using the given path
    //Fixed hardcoded file path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of courses
            Course course;
            course.ID = file[i][0];
            course.Name = file[i][1];
            
            //Loop to add prerequisites, if any, into the vector
            string cur = file[i][j]; 
            while (!cur.empty()) {
                course.Prerequisites.push_back(cur);
                
                ++j;
                cur = file[i][j];
            }

            bst->Insert(course);
        }
        cout << endl;
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    //Variable declaration
    string csvPath;
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    string CourseID;
    Course course;

    cout << "Welcome to the course planner. " << endl << endl;

    //Switch case menu
    int choice = 0;
    while (choice != 9) {
        //Menu display
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display Course List" << endl;
        cout << "  3. Print Course" << endl;
        //Added case for calling the deconstructor
        cout << "  4. Delete Course Information" << endl;
        cout << "  9. Exit" << endl << endl;
        //Prompt user for input
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        //Case 1 loads courses from the given file path
        case 1:
            //Prompt user for file path
            cout << "Please enter the file path you wish to load: " << endl;
            cin >> csvPath;
            
            // Complete the method call to load the courses
            loadCourses(csvPath, bst);
            break;

        //Case 2 outputs the full list of courses, ordered
        //Updated case 2 to check for data before printing
        case 2:
            if (bst->GetRoot() == nullptr) {
                cout << "There is no data. Please load courses and try again." << endl << endl;
            }
            else {
                cout << "Here is a list of all courses, ordered." << endl << endl;

                //Calls Tree method to output the courses in order
                bst->InOrder();
                cout << endl;
            }

            break;
        
        //Case 3 displays all information for a given course
        case 3:
            //Prompt user for the course number to search for
            cout << "What course do you want to know about?" << endl;
            cin >> CourseID;
            //Makes all characters in the string upper case so as to match the ID's in the tree
            for (int x = 0; x < CourseID.length(); ++x) {
                CourseID.at(x) = toupper(CourseID.at(x));
            }

            //Retrieves Course using Tree search method
            course = bst->Search(CourseID);

            //If the course is not empty, display it's information
            if (!course.ID.empty()) {
                displayCourse(course);
            }
            //Else notify user the course was not found
            else {
                cout << "Course not found" << endl << endl;
            }

            break;

        //Calls deconstructor, and verifies deconstruction was completed.
        case 4:
            cout << "Deleting all course information..." << endl;
            bst->~BinarySearchTree();
            if (bst->GetRoot() == nullptr) {
                cout << "Course information successfully deleted." << endl << endl;
            }
            else {
                cout << "Error deleting information. Please try again." << endl << endl;
            }
            break;

        //Case 9 exits the program
        case 9:
            cout << "Thank you, goodbye." << endl;
            return 0;

        //Default tells the user their selection is invalid
        default:
            cout << choice << " is not a valid option." << endl << endl;
        }
    }
    return 0;
}