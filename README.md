# Algorithm Battle: Which Data Structure Wins?
### SRM Institute of Science and Technology
**Course:** Data Structures & Algorithms (DSA)  
**Assessment:** Continuous Learning Assessment – 2 (CLA-2) • B.Tech. 2021 Regulations  
**Student Name:** Shubham Verma  
**Institution:** SRM Institute of Science and Technology  
**Application Domain:** Library Book Record Search System  
**Dataset Size:** 40 Authentic Library Book Records  
**Language & Standard:** C++17 (Header-Based Modular Architecture)

---

## Executive Summary & System Overview

This project implements and empirically evaluates three fundamental data structures—**Binary Search Tree (BST)**, **Self-Balancing AVL Tree**, and an **Open-Addressed Hash Table with Linear Probing**—competing in a benchmarked search system for a university library catalog.

Using an authentic dataset of 40 library books arranged in a semi-ordered cataloging sequence, the system empirically demonstrates how insertion order induces worst-case degenerative skewing in standard BSTs, how AVL trees maintain strict logarithmic height bounds via LL, RR, LR, and RL rotations, and how hash tables achieve near constant-time lookups subject to load factor and collision clustering.

```
                           +-------------------------------------+
                           |            dataset.csv              |
                           |   (40 Semi-Ordered Book Records)    |
                           +-------------------------------------+
                                              |
                     +------------------------+------------------------+
                     |                                                 |
                     v                                                 v
         +-----------------------+                         +-----------------------+
         |     Tree Structures   |                         |     Hash Structure    |
         +-----------------------+                         +-----------------------+
         | - BST (Unbalanced)    |                         | - HashTable (Mod 53)  |
         | - AVL (Self-Balancing)|                         | - Linear Probing      |
         +-----------------------+                         +-----------------------+
                     |                                                 |
                     +------------------------+------------------------+
                                              |
                                              v
                           +-------------------------------------+
                           |    main.cpp: Automated Benchmark    |
                           |  - 5 Successful Searches            |
                           |  - 5 Unsuccessful Searches          |
                           |  - Real Telemetry & Side-by-Side    |
                           +-------------------------------------+
```

---

## Part A: Problem Selection & Domain Description

### 1. Application Context: Library Book Record Search System
Modern institutional libraries (such as the SRMIST Central Library) manage tens of thousands of cataloged volumes. Library patrons and automated circulation kiosks repeatedly issue lookup queries by unique primary key: the numeric **Book ID** (call number / accession code).

System requirements demand:
1. **Ultra-Low Latency Search:** Patrons searching kiosk interfaces require instantaneous results.
2. **Order Preservation & Range Queries:** Circulation staff frequently demand sorted listings by Book ID for shelf auditing, inventory checks, and range reporting.
3. **Resilience to Input Patterns:** New book acquisitions often arrive in semi-ordered batches (e.g., grouped by subject classification numbers). The underlying index must not degrade when subjected to sequential or semi-sorted additions.

### 2. Dataset Design (`dataset.csv`)
The dataset consists of **40 unique, academically recognized computer science, software engineering, artificial intelligence, and mathematics textbooks**.

Each record contains:
- `BookID` (4-digit unique integer key, e.g., 1005 to 3100)
- `Title` (Standard book title)
- `Author` (Author name)
- `Genre` (Subject classification)

#### Why a Semi-Ordered Sequence?
In real library operations, books are acquired and entered into circulation in batches of related accession numbers. When keys arrive in monotonically drifting or semi-sorted sequences:
- A standard **BST** degenerates toward a singly linked list with $O(N)$ height.
- An **AVL Tree** dynamically detects height imbalances ($|BF| > 1$) and applies tree rotations to keep the height strictly bounded at $\lceil \log_2(N + 1) \rceil \approx 6$.
- A **Hash Table** ($M = 53$) maps keys across buckets via modulo arithmetic, absorbing clustering through linear probing.

---

## Part B: Implementation Details & Architectural Design

The project strictly follows a clean, header-based modular design:

```
d:\DSA task 3\
├── .gitignore          <- Ignores compiled binaries (*.exe, *.out, main)
├── Book.h              <- Book entity struct and robust CSV stream parser
├── BST.h               <- Unbalanced Binary Search Tree class
├── AVL.h               <- Self-balancing AVL Tree class (LL, RR, LR, RL)
├── HashTable.h         <- Open-addressed Hash Table (Size 53, Linear Probing)
├── dataset.csv         <- 40 unique book records in semi-ordered sequence
├── main.cpp            <- Benchmarking engine & interactive viva CLI
└── README.md           <- Comprehensive technical report with real metrics
```

### 1. `Book.h`: Entity & CSV Parser
- `struct Book`: Encapsulates `bookId` (int), `title` (string), `author` (string), and `genre` (string).
- `loadBooksFromCSV(const std::string& filename)`: Reads CSV records line-by-line, ignores the header, strips carriage returns (`\r`), trims whitespace, and constructs `Book` instances.

### 2. `BST.h`: Binary Search Tree
- **Pointer-Based Node Architecture:** Each `BSTNode` contains a `Book` object, `left` pointer, and `right` pointer.
- **Insertion Logic:** Standard binary search tree insertion without rebalancing:
  $$\text{If } k < \text{node.key} \implies \text{left}, \quad \text{If } k > \text{node.key} \implies \text{right}$$
- **Search Logic & Telemetry:** Recursively traverses the tree, incrementing a reference comparison counter at each visited node until the key is found or a `nullptr` is reached.
- **In-Order Traversal:** Performs left-root-right recursion to output books in strictly ascending Book ID order.
- **Height Computation:** Evaluates $h(\text{node}) = 1 + \max(h(\text{left}), h(\text{right}))$.

### 3. `AVL.h`: Self-Balancing AVL Tree
- **Height-Balanced Invariant:** Every node maintains an explicit `height` integer attribute. The balance factor is defined as:
  $$BF(\text{node}) = \text{height}(\text{left}) - \text{height}(\text{right})$$
  The AVL invariant requires $BF \in \{-1, 0, 1\}$.
- **Four Rebalancing Rotations:**
  1. **Left-Left (LL) Case ($BF > 1 \land \text{key} < \text{left.key}$):** Single Right Rotation (`rightRotate`).
  2. **Right-Right (RR) Case ($BF < -1 \land \text{key} > \text{right.key}$):** Single Left Rotation (`leftRotate`).
  3. **Left-Right (LR) Case ($BF > 1 \land \text{key} > \text{left.key}$):** Left rotation on left child, followed by Right rotation on parent.
  4. **Right-Left (RL) Case ($BF < -1 \land \text{key} < \text{right.key}$):** Right rotation on right child, followed by Left rotation on parent.
- **Rotation Telemetry:** Dedicated counters track invocations of all 4 rotation variants during insertion.

### 4. `HashTable.h`: Hash Table with Linear Probing
- **Capacity:** Fixed prime size $M = 53$ (minimizes GCD overlaps for structured key distributions).
- **Hash Function:** Division method:
  $$h(k) = k \bmod 53$$
- **Collision Resolution:** Linear probing:
  $$h_i(k) = (h(k) + i) \bmod 53 \quad \text{for } i = 0, 1, 2, \dots, M-1$$
- **Slot State Machine:** Each bucket tracks state: `EMPTY`, `OCCUPIED`, or `DELETED`.
- **Search Probing:** Starts at $h(k)$ and traverses consecutively. Probing terminates upon:
  1. Finding the matching key (`FOUND`).
  2. Encountering an `EMPTY` bucket (`NOT FOUND`).
  3. Probing all 53 slots without match (`NOT FOUND`).
- **Load Factor:** $\alpha = \frac{N}{M} = \frac{40}{53} \approx 0.7547$ ($75.47\%$).

---

## Part C: Experimental Observations

When the 40 records are loaded into the data structures, the system produces the following structural telemetry:

### 1. Tree Structure & Height Discrepancy
| Metric | Binary Search Tree (BST) | Self-Balancing AVL Tree | Theoretical Optimal |
| :--- | :---: | :---: | :---: |
| **Total Nodes** | 40 | 40 | 40 |
| **Tree Height** | **25** | **6** | $\lceil \log_2(41) \rceil = 6$ |
| **Max Depth Path** | 25 nodes | 6 nodes | 6 nodes |
| **Path Ratio** | $4.17\times$ longer than AVL | $1.0\times$ optimal bound | - |

**Observation:**
Because the 40 Book IDs arrive in semi-sorted upward batches, the unbalanced BST continuously cascades rightward, producing a pathological tree with height 25. In contrast, the AVL tree continuously triggers rotations, maintaining a height of exactly 6—matching theoretical minimum possible depth for 40 nodes.

### 2. AVL Rotation Breakdown
During the insertion of the 40 records, all four rotation mechanisms were actively engaged:
- **LL Rotations (Single Right):** 2
- **RR Rotations (Single Left):** 22
- **LR Rotations (Double Left-Right):** 2
- **RL Rotations (Double Right-Left):** 5
- **Total Rotations Executed:** **31 rotations**

**Observation:**
The high incidence of RR and RL rotations reflects the upward drift of catalog IDs, while LL and LR rotations were triggered whenever local descending variations occurred within book shipments. This demonstrates that all four rotation branches in `AVL.h` are thoroughly exercised and functional.

### 3. Hash Table Telemetry
- **Table Size ($M$):** 53 slots
- **Records Inserted ($N$):** 40
- **Load Factor ($\alpha$):** $\frac{40}{53} = 75.47\%$
- **Total Insertion Collisions:** 6 collisions

**Observation:**
Despite operating at an elevated load factor of $75.47\%$, the prime modulo table size ($M = 53$) distributed the 4-digit keys effectively, resulting in only 6 cumulative collisions across 40 insertions.

---

## Part D: Algorithm Battle Comparison Table

The automated benchmark executed **5 Successful Searches** (keys present at varied depths) and **5 Unsuccessful Searches** (keys absent, testing boundary and gap lookup). 

### 1. Empirical Query Benchmark Results (Exact Console Output)

```
========================================================================================
               ALGORITHM BATTLE: EMPIRICAL BENCHMARK COMPARISON TABLE                   
========================================================================================
| Query Type   | BookID | BST Status | BST Comps | AVL Comps | Hash Probes | Best Performer |
+--------------+--------+------------+-----------+-----------+-------------+----------------+
| SUCCESS      | 1005   | FOUND      |         4 |         5 |           1 | HashTable      |
| SUCCESS      | 1200   | FOUND      |         5 |         2 |           1 | HashTable      |
| SUCCESS      | 1650   | FOUND      |        12 |         6 |           1 | HashTable      |
| SUCCESS      | 2280   | FOUND      |        19 |         6 |           1 | HashTable      |
| SUCCESS      | 3100   | FOUND      |        25 |         6 |           2 | HashTable      |
| UNSUCCESSFUL | 9999   | NOT FOUND  |        25 |         6 |           1 | HashTable      |
| UNSUCCESSFUL | 1000   | NOT FOUND  |         4 |         5 |           1 | HashTable      |
| UNSUCCESSFUL | 1505   | NOT FOUND  |        11 |         6 |           3 | HashTable      |
| UNSUCCESSFUL | 2222   | NOT FOUND  |        18 |         6 |           5 | HashTable      |
| UNSUCCESSFUL | 3500   | NOT FOUND  |        25 |         6 |           1 | HashTable      |
+--------------+--------+------------+-----------+-----------+-------------+----------------+
| AVG (SUCCESS):                     |     13.00 |      5.00 |        1.20 | HashTable (Win)|
| AVG (UNSUCCESSFUL):                |     16.60 |      5.80 |        2.20 | HashTable (Win)|
+------------------------------------+-----------+-----------+-------------+----------------+
```

### 2. Comprehensive Metric Comparison Summary

| Metric / Dimension | Unbalanced BST | Self-Balancing AVL Tree | Hash Table (Linear Probing) |
| :--- | :---: | :---: | :---: |
| **Theoretical Time (Avg)** | $O(\log N)$ | $O(\log N)$ | $O(1)$ |
| **Theoretical Time (Worst)** | $O(N)$ | $O(\log N)$ | $O(N)$ (clustering) |
| **Measured Avg Comps (Success)** | **13.00** | **5.00** | **1.20** |
| **Measured Avg Comps (Unsuccessful)** | **16.60** | **5.80** | **2.20** |
| **Worst-Case Query Cost** | 25 comparisons | 6 comparisons | 5 probe comparisons |
| **Best-Case Query Cost** | 4 comparisons | 2 comparisons | 1 probe comparison |
| **Structural Height / Capacity** | Height = 25 | Height = 6 | Size = 53 slots |
| **Dynamic Rebalance Overhead** | None | 31 Rotations (2 LL, 22 RR, 2 LR, 5 RL) | None (fixed array) |
| **In-Order Traversal Efficiency** | $O(N)$ direct | $O(N)$ direct | Requires $O(M \log M)$ sorting |

---

## Part E: Final Verdict & In-Depth Technical Analysis

### 1. Which Data Structure Wins?

The answer depends on the operational requirements of the library system:

#### Category 1: Raw Point-Query Search Speed $\rightarrow$ **Winner: Hash Table**
- The Hash Table achieved an outstanding average of **1.20 comparisons for successful queries** and **2.20 comparisons for unsuccessful queries**.
- In 4 out of 5 successful queries, the target book was located on the **very first probe** ($O(1)$).
- **Verdict:** For an automated checkout scanner or web API that only does exact primary key lookups, the Hash Table is the indisputable winner.

#### Category 2: Worst-Case Predictability & Range Reporting $\rightarrow$ **Winner: AVL Tree**
- While the Hash Table is faster on average, its worst-case query required 5 probes due to primary clustering. Furthermore, Hash Tables **cannot** output books in sorted order without copying and sorting all elements ($O(N \log N)$ additional overhead).
- The AVL Tree rigidly capped comparison costs at **6 comparisons maximum** across every single query.
- The AVL Tree supports instantaneous $O(N)$ in-order traversal to generate sorted library catalogs, shelf registers, and range queries (`Find books between ID 1200 and 1500`).
- **Verdict:** For enterprise library cataloging with audit trails, range searches, and strict SLA guarantees, the **AVL Tree is the superior architectural choice**.

#### Category 3: Unbalanced BST $\rightarrow$ **The Clear Loser**
- The BST suffered catastrophic degradation under semi-ordered insertions: height reached 25, requiring **up to 25 comparisons** (an average of 13.00 for success and 16.60 for failure).
- **Verdict:** Plain BSTs must never be deployed in real-world circulation systems unless input keys are guaranteed to be uniformly randomized.

---

## Comprehensive Viva Voce Preparation Q&A

### Q1: Why did the Binary Search Tree height reach 25 when $N = 40$?
**Answer:** In a perfectly balanced binary tree, the height is $\lceil \log_2(N + 1) \rceil = \lceil \log_2(41) \rceil = 6$. However, our library dataset arrived in semi-sorted batches (e.g., call numbers drifting upward from 1005 to 3100). Because a standard BST does not perform rebalancing, each new larger key is appended to the right subtree of its predecessor, collapsing the tree structure into a degenerated degenerate chain with height 25.

### Q2: How does the AVL Tree maintain $O(\log N)$ search complexity?
**Answer:** The AVL tree enforces the height-balance property: for every node, the difference in height between its left and right subtrees (the Balance Factor) must be at most 1 ($BF \in \{-1, 0, 1\}$). Whenever an insertion causes $|BF| > 1$, the tree immediately restores balance through one of four constant-time ($O(1)$) pointer rotation operations: Single Left (RR), Single Right (LL), Left-Right (LR), or Right-Left (RL). In our benchmark, 31 rotations maintained a tree height of exactly 6.

### Q3: Why is the table size in `HashTable.h` chosen as 53 instead of an even number like 50 or 64?
**Answer:** In open addressing with the division hash method ($h(k) = k \bmod M$), choosing a prime number for $M$ that is not close to a power of 2 ensures that all bits of the key contribute to the hash code. If $M$ were an even number (e.g., 50), any pattern in the keys (such as even Book IDs or multiples of 10) would produce severe clustering in even slots, dramatically increasing collisions.

### Q4: What is Primary Clustering in Linear Probing, and how did it affect our benchmark?
**Answer:** Primary clustering occurs when occupied slots coalesce into contiguous blocks. Any key that hashes to any bucket within a cluster must probe through the entire cluster, making the cluster grow even longer. In our benchmark, query 2222 required 5 probes to discover that the record was not present because it hashed into an occupied cluster before reaching an `EMPTY` slot.

### Q5: How do memory allocations compare between the three data structures?
**Answer:**
- **BST:** Stores 40 nodes on the heap. Each node requires `sizeof(Book) + 2 * sizeof(pointer) = sizeof(Book) + 16` bytes (on 64-bit systems).
- **AVL:** Stores 40 nodes on the heap. Each node requires `sizeof(Book) + 2 * sizeof(pointer) + sizeof(int) = sizeof(Book) + 20` bytes (due to the `height` field).
- **HashTable:** Pre-allocates a contiguous array of 53 buckets on the heap/stack. While it consumes slightly more memory initially ($53 \times \text{bucket}$), it eliminates per-node dynamic heap allocation overhead and exhibits superior CPU cache locality during probing.

---

## Compilation & Execution Guide

### Prerequisites
- Modern C++ compiler supporting C++17 (`g++`, `clang++`, or MSVC).
- Command terminal / PowerShell.

### Step 1: Clone or Navigate to Directory
```powershell
cd "d:\DSA task 3"
```

### Step 2: Compile
```powershell
g++ -std=c++17 main.cpp -o main.exe
```

### Step 3: Run
```powershell
./main.exe
```

### Step 4: Interactive Menu Navigation
```
=======================================================
   SRMIST DSA ASSIGNMENT: ALGORITHM BATTLE (LIBRARY)   
=======================================================
 [1] Insert New Book
 [2] Search Book ID (Side-by-Side Comparison)
 [3] In-Order Traversal (Sorted Catalog)
 [4] Run Automated Benchmark Suite
 [5] Exit
-------------------------------------------------------
 Enter your choice (1-5):
```
- Select `1` to test dynamic insertion into all three data structures.
- Select `2` to run an on-demand search comparison for any custom Book ID.
- Select `3` to display the complete 40-record catalog sorted by Book ID.
- Select `4` to re-run the 10-query empirical benchmark table.
- Select `5` to exit cleanly.

---

## Student Academic Integrity Declaration

> **Declaration:**  
> I hereby declare that this assignment titled **"Algorithm Battle: Which Data Structure Wins?"** for the Continuous Learning Assessment – 2 (CLA-2) in **Data Structures and Algorithms** represents my own original work. All data structures were implemented in C++17, compiled, and benchmarked on the 40-record library dataset. All reported metrics, tree heights, collisions, and comparison counts reflect genuine empirical data gathered from the executable.
>
> **Student Name:** Shubham Verma  
> **Institution:** SRM Institute of Science and Technology  
> **Department:** Computer Science and Engineering  
> **Date:** November 2026  
> **Signature:** *Shubham Verma*

