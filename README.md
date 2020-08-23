# DataBase-Using-B-Tree
B-trees are used in file system, database to provide guarantied logarithmic time key-search, insert, and remove.This report is about implementation of Data Base system using B-tree. The main functions of database are insertion, deletion and searching which can be done by B-tree in logarithmic time.For storing data into secondary memory here we are using file system and creating a binary file for accessing and storing data.
Introduction: B-Tree is a self-balancing search tree. In most of the other self-balancing search trees (like AVL and Red-Black Trees), it is assumed that
everything is in main memory. To understand the use of B-Trees, we must think of the huge amount of data that cannot fit in main memory. When the number of keys is
high, the data is read from disk in the form of blocks. Disk access time is very high compared to main memory access time. The main idea of using B-Trees is to reduce
the number of disk accesses. Most of the tree operations (search, insert, delete, max, min, ..etc ) require O(h) disk accesses where h is the height of the tree. B-tree is a fat tree. The height of B-Trees is kept low by putting maximum possible keys in a B-Tree node. Generally, a B-Tree node size is kept equal to the disk block size. Since h is low for B-Tree, total disk accesses for most of the operations are reduced significantly compared to balanced Binary Search Trees like AVL Tree, Red-Black Tree, etc. Functionality: Data base satisfying following Functionalities:
 Insertion
 Searching
 Deletion
 Display
Insertion: In B-tree node we are storing a array structure having a integer as primary key and string as data. And size of array is 2t-1 where t is degree of tree. Each node containing 2t number of child.
For Insertion we are following given algorithm:-
1) Initialize x as root.
2) While x is not leaf, do following
a) Find the child of x that is going to to be traversed next. Let the child be y.
b) If y is not full, change x to point to y.
c) If y is full, split it and change x to point to one of the two parts of y. If k is smaller than mid key in y, then set x as first part of y. Else second part of y. When we split y, we move a key from y to its parent x.
3) The loop in step 2 stops when x is leaf. x must have space for 1 extra key as we have been splitting all nodes in advance. So simply insert k to x.

Searching: In our Project we are searching data on the basis of primary key. We are taking primary from user whose data we want to search and then retrieving data from
that primary key. Search is similar to the search in Binary Search Tree. Let the primary key to be searched be k. We start from the root and recursively traverse down. For every visited non-leaf node, if the node has the key, we simply return the node. Otherwise, we recur down to the appropriate child (The child which is just before the first greater key) of the node. If we reach a leaf node and don’t find k in the leaf node, we return NULL.

Deletion: In Deletion we are asking primary key from user which he/she wants to delete
Then following given steps:
 Search line number where data is stored in the binary file
 Create one temporary file
 Copy all data from original file except line where data is stored
 Delete original file
 Rename temporary file as original file
 Create b-tree based on new data
Display: This function will display all the data present in database.This method is similar to Inorder traversal of B-Tree. We start from the leftmost child, recursively print the leftmost child, then repeat the same process for remaining children and keys. In the end, recursively print the rightmost child.
