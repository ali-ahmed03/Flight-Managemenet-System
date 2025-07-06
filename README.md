This is a simple console-based Flight Management System written in C++ using a Binary Search Tree (BST) for flight data storage and retrieval. It allows users to add, update, delete, search, and book flights while maintaining flight data using file I/O.

Features:
Object-Oriented Design 
Add and update flight records (5 flights)
Book tickets for available flights
Check flight status and availability
Mark flights as deleted (soft delete). Deleted flights are hidden but can be restored
Persist flight data across sessions using a text file
Reuse flight slots after deletion by updating the same flight number

Data Saving:
Flight data is saved to flight_data.txt after every change. Even deleted flights are stored and displayed as empty entries, allowing for updates later.

Technologies:
C++
File Handling (fstream)
Binary Search Tree for organizing flight records
