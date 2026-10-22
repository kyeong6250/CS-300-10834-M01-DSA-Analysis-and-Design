# CS 300 – Data Structures and Algorithms

In this course, I worked on a course-planning application for ABC University. The goal was to create a program that could load course information from a file, organize the data, print all courses in alphanumeric order, and allow a user to search for an individual course and view its prerequisites.

## What was the problem I was solving?

The main problem was deciding how to organize and retrieve course data efficiently. I explored three different data structures: a vector, a hash table, and a binary search tree. Each structure could store the course information, but they had different strengths depending on whether the program needed to search for one course or print all courses in sorted order.

For Project One, I analyzed the runtime and memory usage of these structures and compared their advantages and disadvantages. I recommended using a binary search tree because it supports efficient searching while also allowing the courses to be printed in alphanumeric order using an in-order traversal.

For Project Two, I implemented the advising program in C++ using that binary search tree design.

## How did I approach the problem?

I started by breaking the problem into smaller tasks: reading and validating the course file, creating course objects, storing those objects in a data structure, searching for individual courses, and printing the complete course list.

Understanding data structures was important because the structure I selected directly affected how efficiently the program could perform these tasks. A vector is simple but may require searching through every element. A hash table provides fast average lookup times but does not naturally maintain sorted order. A binary search tree provided a good balance because it supported average-case efficient searches and naturally produced a sorted list through in-order traversal.

## How did I overcome roadblocks?

One challenge was validating the input data correctly. Each course could contain a different number of prerequisites, so the program had to parse each line carefully. I also needed to verify that every prerequisite listed in the file actually existed as another course.

Another challenge was implementing the binary search tree operations correctly. I worked through the insertion, search, and traversal logic separately instead of trying to implement everything at once. Testing with both valid and invalid input helped me catch issues with file handling, menu selections, lowercase course numbers, and missing course information.

## How has this work expanded my approach to designing software?

This project reinforced the importance of planning before writing code. Comparing multiple data structures before implementing the final program helped me understand that there is not always one universally best solution. The correct design depends on what operations the program needs to perform most often.

I also became more comfortable thinking about runtime complexity while designing software instead of only worrying about whether the program produces the correct output.

## How has my programming become more maintainable, readable, and adaptable?

Throughout this project, I focused on separating responsibilities into different functions. File loading, validation, binary search tree insertion, searching, printing, and menu handling were kept separate instead of being placed into one large function.

I also used descriptive variable and function names, consistent formatting, comments where they improved understanding, and input validation to make the program easier to follow and maintain. These practices make it easier to change or reuse parts of the program later without rewriting the entire application.

## Portfolio Artifacts

- Project One runtime and memory analysis
- `ProjectTwo.cpp` – Working C++ advising assistance program using a binary search tree
