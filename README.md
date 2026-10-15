# CS-300-10834-M01-DSA-Analysis-and-DesignWhat the Program Does

The program shows a menu with four options:

Load Data Structure: asks for the course file name, reads and validates every line, and loads the courses into a binary search tree.
Print Course List: prints every course, including math courses, in alphanumeric order.
Print Course: asks for a course number and prints its title and each prerequisite's number and title.
Exit

Before loading, the program checks the file for formatting errors. Every line must have a course number and a title, no course number may appear twice, and every prerequisite must exist as a course in the file. The file is read in full before prerequisites are checked, so a prerequisite listed later in the file is still accepted. The existing data is only replaced once a new file passes every check.

How to Build and Run
g++ -std=c++11 -o ProjectTwo ProjectTwo.cpp
./ProjectTwo

When prompted, enter the file name CS_300_ABCU_Advising_Program_Input.csv.

Sample Output
What would you like to do? 2
Here is a sample schedule:


What was the problem I was solving? ABCU's advisors needed a quick way to see the Computer Science course list in order and to look up any course's prerequisites while meeting with students. In Project One, I designed the solution and compared three data structures for storing the course data. In Project Two, I built the working program.

How did I approach the problem? I started with the requirements, then wrote pseudocode for loading, sorting, and searching with each candidate data structure and analyzed their runtime. A vector is simple, but finding a course means scanning the whole list, and printing in order means sorting first. A hash table finds a course very quickly but keeps no order, so printing the sorted list still requires a sort. A binary search tree stores courses by course number, so searching is fast (O(log n) on average) and an in-order traversal prints the sorted list with no extra sorting step. Because the advisors need both lookups and a sorted list, I chose the binary search tree. Understanding these trade-offs is why data structures matter: the right choice makes the main operations simple and fast, and the wrong one adds extra work to every request.

How did I overcome roadblocks? Removing a node with two children from a binary search tree was the hardest part of the course's tree assignment. I worked through it by handling each case separately: no children, one child, and two children. For the two-child case, I replaced the node with the smallest course in its right subtree. In Project Two, validating prerequisites was tricky because a course can be listed as a prerequisite before its own line appears in the file. I solved this by reading the whole file first, then checking every prerequisite against the full set of course numbers. I tested each fix with both good input and deliberately broken input.

How has this project expanded my approach to designing software? I now plan the data structure around the operations the user actually needs, instead of picking one out of habit. Writing pseudocode and estimating runtime before coding made the implementation faster and caught design problems early.

How has this project changed the way I write maintainable, readable, and adaptable programs? I keep each function to one job, such as parsing a line, loading the file, searching the tree, or printing a course. I use descriptive names and comments, and I validate all user and file input so bad data produces a clear error message instead of a crash. Because the tree is in its own class, the program could switch to a different data source or data structure without rewriting the menu or display code.
