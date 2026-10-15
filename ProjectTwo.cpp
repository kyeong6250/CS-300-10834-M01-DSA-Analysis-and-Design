#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// Stores one ABCU course and any prerequisite course numbers.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Removes leading and trailing whitespace from input fields.
string trim(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Converts course numbers to uppercase so searches are case-insensitive.
string toUpper(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return value;
}

// Binary search tree used to store courses by alphanumeric course number.
class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        explicit Node(const Course& value)
            : course(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insertNode(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
            return;
        }

        if (course.courseNumber < node->course.courseNumber) {
            insertNode(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            insertNode(node->right, course);
        } else {
            // Replace duplicate course information instead of creating a duplicate node.
            node->course = course;
        }
    }

    const Course* searchNode(Node* node, const string& courseNumber) const {
        while (node != nullptr) {
            if (courseNumber == node->course.courseNumber) {
                return &node->course;
            }

            node = (courseNumber < node->course.courseNumber)
                       ? node->left
                       : node->right;
        }
        return nullptr;
    }

    void printInOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        printInOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << '\n';
        printInOrder(node->right);
    }

    void destroyTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        clear();
    }

    // Prevent accidental shallow copies of raw tree pointers.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void insert(const Course& course) {
        insertNode(root, course);
    }

    const Course* search(const string& courseNumber) const {
        return searchNode(root, courseNumber);
    }

    void printCourseList() const {
        printInOrder(root);
    }

    void clear() {
        destroyTree(root);
        root = nullptr;
    }

    bool empty() const {
        return root == nullptr;
    }
};

// Splits one CSV line into individual fields. The supplied ABCU data does not
// contain quoted commas, so a simple comma parser is sufficient for this file.
vector<string> splitCsvLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream stream(line);

    while (getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }

    // Preserve a trailing empty field if a line ends with a comma.
    if (!line.empty() && line.back() == ',') {
        fields.emplace_back("");
    }

    return fields;
}

// Reads and validates the entire course file before loading the tree. This
// allows prerequisite references to be checked even if their course appears
// later in the input file.
bool loadCoursesFromFile(const string& fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open file " << fileName << ".\n";
        return false;
    }

    vector<Course> parsedCourses;
    unordered_set<string> courseNumbers;
    string line;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        vector<string> fields = splitCsvLine(line);
        if (fields.size() < 2) {
            cout << "Format error on line " << lineNumber
                 << ": course number and course title are required.\n";
            return false;
        }

        Course course;
        course.courseNumber = toUpper(trim(fields[0]));
        course.courseTitle = trim(fields[1]);

        if (course.courseNumber.empty() || course.courseTitle.empty()) {
            cout << "Format error on line " << lineNumber
                 << ": course number or course title is blank.\n";
            return false;
        }

        if (courseNumbers.find(course.courseNumber) != courseNumbers.end()) {
            cout << "Format error on line " << lineNumber
                 << ": duplicate course number " << course.courseNumber << ".\n";
            return false;
        }

        for (size_t i = 2; i < fields.size(); ++i) {
            string prerequisite = toUpper(trim(fields[i]));
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        courseNumbers.insert(course.courseNumber);
        parsedCourses.push_back(course);
    }

    if (parsedCourses.empty()) {
        cout << "Error: The course data file contains no courses.\n";
        return false;
    }

    // Every listed prerequisite must also appear as a course in the file.
    for (const Course& course : parsedCourses) {
        for (const string& prerequisite : course.prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) {
                cout << "Format error: prerequisite " << prerequisite
                     << " for " << course.courseNumber
                     << " does not exist in the file.\n";
                return false;
            }
        }
    }

    // Only replace the existing data after the new file has passed validation.
    courseTree.clear();
    for (const Course& course : parsedCourses) {
        courseTree.insert(course);
    }

    cout << parsedCourses.size() << " courses loaded successfully.\n";
    return true;
}

void printMenu() {
    cout << "\n"
         << "1. Load Data Structure.\n"
         << "2. Print Course List.\n"
         << "3. Print Course.\n"
         << "9. Exit\n\n"
         << "What would you like to do? ";
}

// Prints a selected course and the number/title of each prerequisite.
void printCourseInformation(const BinarySearchTree& courseTree,
                            const string& requestedCourseNumber) {
    const string courseNumber = toUpper(trim(requestedCourseNumber));
    const Course* course = courseTree.search(courseNumber);

    if (course == nullptr) {
        cout << "Course " << courseNumber << " not found.\n";
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << '\n';

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    cout << "Prerequisites:\n";
    for (const string& prerequisiteNumber : course->prerequisites) {
        const Course* prerequisite = courseTree.search(prerequisiteNumber);
        if (prerequisite != nullptr) {
            cout << prerequisite->courseNumber << ", "
                 << prerequisite->courseTitle << '\n';
        }
    }
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner.\n";

    while (choice != 9) {
        printMenu();

        string choiceInput;
        getline(cin, choiceInput);

        try {
            size_t processed = 0;
            choice = stoi(trim(choiceInput), &processed);
            if (processed != trim(choiceInput).size()) {
                throw invalid_argument("extra characters");
            }
        } catch (...) {
            cout << choiceInput << " is not a valid option.\n";
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "What is the name of the course data file? ";
                string fileName;
                getline(cin, fileName);
                fileName = trim(fileName);

                if (fileName.empty()) {
                    cout << "Error: A file name is required.\n";
                    dataLoaded = false;
                    break;
                }

                dataLoaded = loadCoursesFromFile(fileName, courseTree);
                break;
            }

            case 2:
                if (!dataLoaded) {
                    cout << "Please load the course data first.\n";
                    break;
                }

                cout << "Here is a sample schedule:\n\n";
                courseTree.printCourseList();
                break;

            case 3: {
                if (!dataLoaded) {
                    cout << "Please load the course data first.\n";
                    break;
                }

                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourseInformation(courseTree, courseNumber);
                break;
            }

            case 9:
                cout << "Thank you for using the course planner!\n";
                break;

            default:
                cout << choice << " is not a valid option.\n";
                break;
        }
    }

    return 0;
}
