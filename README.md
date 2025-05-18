# Student Management System (SMS)

## Overview

This project is a simple, console‐based Student Management System (SMS) written in C++. It uses a binary search tree (BST) to store student records (each record consists of an ID, a name, an age, and a grade) and provides a menu‐driven interface for adding, displaying (sorted by ID or by grade), searching, updating, and deleting student records. In addition, the system supports serializing (saving) and deserializing (loading) student data to/from a binary file (students.dat) so that your data persists between sessions.

## Features

• **Add Student:**  
  – Prompts for a student’s ID (ensuring it is unique), name (validated for non‐empty and length), age (validated as an integer between 1 and 120), and grade.  
  – Inserts the new student record into a BST (sorted by ID).

• **Display Students:**  
  – **Display by ID:** Prints a list of all students sorted (in‑order) by their ID.  
  – **Display by Grade:** Prints a list of all students sorted (descending) by their grade.

• **Search:**  
  – Given a student’s ID, the system searches the BST and (if found) prints the student’s details.

• **Update:**  
  – Prompts for a student’s ID. If the student is found, you can update (or leave unchanged) the student’s name, age, and grade.

• **Delete:**  
  – Given a student’s (unique) ID, the system removes that student’s record from the BST.

• **Save & Exit:**  
  – Serializes (saves) the entire BST (using a pre‑order traversal) into a binary file (students.dat) so that your data is preserved for the next session.

• **Load (on startup):**  
  – On startup, the system attempts to deserialize (load) student records from students.dat (if the file exists and is non‑empty). If no file (or an empty file) is found, the system starts “fresh.”

## Technical Details

• **Data Structure:**  
  – A binary search tree (BST) is used to store student records (sorted by student ID).  
  – Each node (of type “Node”) holds a “Student” struct (with fields for id, name, age, and grade) and pointers to its left and right children.

• **Serialization:**  
  – Student records are serialized (written) to a binary file (students.dat) using a pre‑order traversal.  
  – On startup, the system deserializes (reads) the file (if it exists) to restore the BST.

• **Input Validation:**  
  – The system performs robust input validation (for example, ensuring that the student’s ID is unique, that the name is non‑empty and not too long, and that the age is an integer between 1 and 120).

• **Error Handling:**  
  – The system catches (and handles) exceptions (for example, if the file cannot be opened or if an unexpected error occurs during deserialization).

## Design Considerations

• **Efficiency:**  
  – We chose a binary search tree (BST) (sorted by student ID) so that searching, inserting, and deleting a student record (by ID) is O(log n) (on average) – much faster than an unsorted array (or a linked list) (which would require O(n) for a linear search).  
  – Serialization (saving and loading) is done using a pre‑order traversal (O(n) in time) so that the entire tree is written (and later read) in a predictable order.

• **Memory Usage:**  
  – Each BST node (of type “Node”) holds a “Student” struct (with an integer (id), a std::string (name), an integer (age), and a double (grade)) plus two pointers (to its left and right children).  
  – While this (and dynamic allocation) incurs extra memory overhead (compared to a contiguous array), the BST’s O(log n) performance (for key operations) justifies the extra cost.

• **Implementation Complexity:**  
  – Implementing a BST (with operations such as insert, remove, and search) is more complex than using a simple array (or a linked list).  
  – For example, our “remove” (or “delete”) operation (in the BST) must handle cases (such as a node with two children) that require extra logic (for example, replacing the node with its in‑order successor).  
  – However, the BST’s O(log n) performance (for searching, inserting, and deleting) justifies the extra implementation effort – especially in a system (like our SMS) where you frequently search (or update) a student’s record by their unique ID.

