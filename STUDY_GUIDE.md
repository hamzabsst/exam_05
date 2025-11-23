# 42 Exam Rank 05 - Study Guide

## Overview
You have **5 possible exercises** across 2 difficulty levels:
- **Level 1 (easier)**: bigint, polyset, vect2
- **Level 2 (harder)**: bsq, life

---

## 📚 LEVEL 1 EXERCISES

### 1. POLYSET (C++ - Polymorphism & Inheritance)

**What is it?**
- **Bag**: A collection that allows duplicates (like [3, 5, 3, 7])
- **Set**: A collection that does NOT allow duplicates (like [3, 5, 7])

**The Challenge:**
You get 4 base classes already implemented:
1. `bag` - abstract base class
2. `searchable_bag` - abstract class adding search capability
3. `array_bag` - bag implemented with arrays
4. `tree_bag` - bag implemented with binary search tree

**What YOU must code:**

#### Part 1: Make bags searchable
Create 2 classes using **multiple inheritance**:

```cpp
// searchable_array_bag.hpp
class searchable_array_bag : public array_bag, public searchable_bag {
    // Implement has() method - iterate through array to find value
    bool has(int val) const;
};

// searchable_tree_bag.hpp
class searchable_tree_bag : public tree_bag, public searchable_bag {
    // Implement has() method - traverse BST to find value
    bool has(int val) const;
};
```

**Key Points:**
- Use `virtual` inheritance to avoid diamond problem
- `has()` for array: simple loop through elements
- `has()` for tree: BST traversal (go left if smaller, right if bigger)
- Don't forget Orthodox Canonical Form (constructor, copy constructor, assignment operator, destructor)

#### Part 2: Create the Set class
Wrap a searchable_bag to prevent duplicates:

```cpp
class set {
private:
    searchable_bag &bag;  // Reference to a bag
public:
    // Before inserting, check if value already exists
    void insert(int val) {
        if (!bag.has(val))  // Only insert if not present
            bag.insert(val);
    }
};
```

**Key Concept:** A set is just a bag that checks for duplicates before inserting!

---

### 2. BIGINT (C++ - Operator Overloading)

**What is it?**
Store and operate on numbers bigger than what normal `int` or `long` can hold (like 999999999999999999999999).

**How?**
Store the number as a **string** internally!

**What to implement:**
```cpp
class bigint {
private:
    std::string value;  // Store number as string

public:
    // Operators you MUST implement:
    bigint operator+(const bigint &other);  // Addition
    bigint& operator++();                    // Pre-increment (++x)
    bigint operator++(int);                  // Post-increment (x++)
    bigint operator<<(int shift);            // Decimal shift (43 << 3 = 43000)

    // Comparison operators
    bool operator==(const bigint &other);
    bool operator<(const bigint &other);
    bool operator>(const bigint &other);

    // BONUS: Implement subtraction even if subject doesn't say so
    // (needed for grademe to compile)
    bigint operator-(const bigint &other);
};
```

**Key Algorithm - Addition:**
```
  "1234"
+ "5678"
-------
```
1. Start from rightmost digit (like manual addition)
2. Add digits + carry
3. Store result digit by digit
4. Handle carry propagation

**Decimal shift:**
- `43 << 3` means multiply by 10³ = 43000
- Just append zeros to the string!

---

### 3. VECT2 (C++ - Operator Overloading)

**What is it?**
A 2D vector class for math operations.

**What to implement:**
```cpp
class vect2 {
private:
    int x, y;

public:
    vect2(int x, int y);

    // Operators
    vect2 operator+(const vect2 &other);  // Vector addition
    vect2 operator-(const vect2 &other);  // Vector subtraction
    vect2 operator*(int scalar);           // Scalar multiplication
    int& operator[](int index);            // Access x (index 0) or y (index 1)

    // Stream output
    friend ostream& operator<<(ostream &os, const vect2 &v);
};
```

**Key Points:**
- Vector addition: `(2,3) + (4,5) = (6,8)`
- Scalar multiplication: `(2,3) * 5 = (10,15)`
- `operator[]`: 0 returns x, 1 returns y
- Output format: `"{x, y}"`

---

## 🔥 LEVEL 2 EXERCISES

### 4. BSQ (C - Algorithm)

**The Classic Piscine Exercise!**

Find the biggest square of empty spaces on a map.

**Input format:**
```
9 . o x         <- (lines, empty_char, obstacle_char, full_char)
...........................
....o......................
............o..............
```

**Output:**
Fill the biggest square with 'x':
```
.....xxxxxxx...............
....oxxxxxxx...............
.....xxxxxxxo..............
```

**Algorithm (Dynamic Programming):**
1. Create a DP table same size as map
2. For each cell `(i,j)`:
   ```
   if cell is obstacle:
       dp[i][j] = 0
   else:
       dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
   ```
3. Track position of max value
4. Fill the square at that position

**Edge Cases:**
- Invalid maps (print "map error" to stderr)
- Multiple maps as arguments
- Reading from stdin if no arguments
- All lines must have same length

---

### 5. LIFE (C - Conway's Game of Life)

**What is it?**
Simulate Conway's Game of Life on a grid.

**Input:**
```
./life width height iterations
```
Commands from stdin:
- `w a s d` - move pen up, left, down, right
- `x` - toggle pen (lift/lower to start/stop drawing)

**Example:**
```
echo 'sdxddssaaww' | ./life 5 5 0
```
Creates initial pattern, then simulates for 0 iterations.

**Game of Life Rules:**
For each cell, count live neighbors (8 surrounding cells):
1. **Live cell**:
   - 2-3 neighbors → stays alive
   - <2 or >3 neighbors → dies
2. **Dead cell**:
   - Exactly 3 neighbors → becomes alive

**Implementation Steps:**
1. Parse command line args (width, height, iterations)
2. Read stdin commands and draw initial pattern
3. Simulate game for N iterations:
   - Create new board
   - For each cell, count neighbors
   - Apply rules
   - Copy new board to current
4. Print final state ('0' for alive, ' ' for dead)

**Key Points:**
- Cells outside board are considered dead
- Use double buffering (2 arrays) to avoid modifying while reading

---

## 🎯 STUDY STRATEGY

### Day 1 (Today):
1. **Polyset** - Understand inheritance hierarchy
2. **Vect2** - Practice operator overloading (simplest)
3. **Bigint** - More complex operator overloading

### Day 2:
1. **Life** - Implement the simulation logic
2. **BSQ** - Review DP algorithm
3. Re-code polyset from scratch

### Day 3 (Exam Eve):
1. Code each exercise from memory (no looking!)
2. Focus on your weakest ones
3. Review edge cases and error handling

---

## 💡 KEY C++ CONCEPTS TO MASTER

### Orthodox Canonical Form (OCF)
Every class needs:
```cpp
class MyClass {
public:
    MyClass();                              // Default constructor
    MyClass(const MyClass &other);          // Copy constructor
    MyClass& operator=(const MyClass &other); // Assignment operator
    ~MyClass();                             // Destructor
};
```

### Virtual Inheritance (for diamond problem)
```cpp
class A : virtual public Base {};
class B : virtual public Base {};
class C : public A, public B {};  // Only one Base instance
```

### Operator Overloading
```cpp
// Member function
MyClass operator+(const MyClass &other) const;

// Friend function (for << operator)
friend ostream& operator<<(ostream &os, const MyClass &obj);
```

---

## 🐛 COMMON MISTAKES TO AVOID

1. **Polyset**: Forgetting `virtual` inheritance → diamond problem
2. **Bigint**: Not handling carry in addition
3. **Vect2**: Wrong output format for `<<` operator
4. **BSQ**: Not validating map (same line length, valid characters)
5. **Life**: Modifying grid while reading it (use 2 grids!)
6. **All**: Forgetting to implement all OCF methods
7. **All**: Memory leaks (always free what you malloc)

---

## ⚡ QUICK REFERENCE

### C++ Compilation
```bash
c++ -Wall -Wextra -Werror -std=c++98 *.cpp -o program
```

### C Compilation
```bash
gcc -Wall -Wextra -Werror *.c -o program
```

### Testing
- Subject provides main.c/main.cpp to test with
- Make sure your code compiles with their main
- Test edge cases!

---

Good luck! You got this! 🚀
