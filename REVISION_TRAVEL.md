# Exam Rank 05 — travel revision

Based on your local exercise subjects, checked 2 November 2026. We use your exam numbering: **Level 0 = vect2, bigint, polyset; Level 1 = life, bsq.** Your repository calls these folders level1 and level2.

Level 0 is randomly assigned among the three, according to your confirmation. You reported that passing Level 0 carries over to the next session; that campus rule has not been independently verified.

## 1. C++ foundations shared by Level 0

**A class bundles data with operations on that data.** A vect2 stores two integers; a bigint can store decimal digits as a string.

**Copy versus reference**
- `T value = other;`: a separate object.
- `T& ref = other;`: another name for the same object.
- `const T& ref = other;`: refers to that object but cannot modify it through ref.
- A reference is not a snapshot: changes made through another writable name are visible through a const reference.

**this versus *this**
- `this` is a pointer to the current object.
- `*this` is the current object itself.
- Returning `*this` from a function returning `T&` gives the caller a reference to the object that was modified.

**Reading an operator declaration**
`bool operator==(const vect2& other) const;`
For `a == b`, a is the current object and b is other. The bool at the front describes the result, not the left operand. The first const protects b through the parameter; the final const prevents ordinary modification of a's members through this call.

**Return types**
| Operation | Changes current object? | Typical return |
|---|---|---|
| assignment = | Yes: copies right operand into left | T& |
| +=, -=, *= | Yes | T& |
| +, -, scalar * | No | T by value |
| prefix ++a | Yes | T&: updated object |
| postfix a++ | Yes | T by value: old copy |
| ==, !=, <, etc. | No | bool |
| writable indexing | May be used to modify a component | int& |
| read-only indexing | No | int or const int& |

Never return a reference to a local result: it dies when the function returns. Returning that result by value is valid.

**Orthodox canonical form:** default constructor, copy constructor, copy assignment, destructor. A copy constructor creates a new object; assignment replaces the state of an existing one. With owned raw pointers, think about deep copying, self-assignment and freeing exactly once. std::string manages its own storage.

A void return type means “returns no value”; it says nothing about which arguments the function accepts.

## 2. Level 0 — vect2

**Goal:** represent a vector (x, y) with integer components.

Examples:
- (2, 3) + (4, 1) = (6, 4).
- (2, 3) * 3 = (6, 9). Support 3 * v as well.
- -(2, 3) = (-2, -3).
- Equality means BOTH components match.
- v[0] accesses x; v[1] accesses y. No bounds checking is required by this subject.
- Output format: `{2, 3}`.

Provide default (0, 0), two-integer and copy construction, assignment, arithmetic and compound arithmetic, unary minus, prefix/postfix increment and decrement, equality/inequality, both indexing forms and stream output.

**Why two indexing forms?** A writable object needs a reference so `v[0] = 9` changes the stored integer. A const object still needs a read-only indexing function.

**Why free functions?** In `3 * v`, the left operand is an int, so this cannot be a member operator of vect2. Stream output similarly has a stream on the left. Write into the supplied stream and return that same stream, enabling chaining.

**Postfix reminder:** if v is (2, 3), `old = v++` leaves old at (2, 3), while v becomes (3, 4). Some comments in the supplied main are misleading; distinguish the printed old value from the object's updated state.

Check: const objects, assignment chaining, modifying through [], negative components and exact output.

## 3. Level 0 — bigint

**Goal:** an unsigned integer too large for built-in integer types. A decimal string such as "12345678901234567890" is a useful representation.

Keep a consistent representation: zero is "0"; other values have no leading zeros. Do not convert the whole number to int to do arithmetic.

**Addition = school addition, right to left.**
For each column:
1. Read each available digit; use zero when one number has run out.
2. Convert characters using digitCharacter - '0'.
3. sum = leftDigit + rightDigit + carry.
4. Output digit = sum % 10.
5. Next carry = sum / 10.
6. Move toward the beginning of both numbers.
Continue while digits OR carry remain.

Example 89 + 79:
- 9 + 9 = 18: digit 8, carry 1.
- 8 + 7 + 1 = 16: digit 6, carry 1.
- Remaining carry gives 168.

If you append the computed digits, they arrive backwards; reverse once at the end. Alternatively prepend each digit. Do not write res[0] when res is empty: indexed assignment does not grow a string.

**Comparison:** with normalized strings, more digits means larger. For equal lengths, compare left to right; the FIRST different digit decides. All matching digits means equality. Example: 100 > 99 even though '1' < '9'.

**Decimal digit shifts — not binary shifts:**
- 42 << 3 = 42000: append three zeros.
- 1337 >> 2 = 13: remove two rightmost digits.
- 42 >> 1 = 4.
- 42 >> 2 = 0, and larger right shifts also yield zero.
- Shifting by zero preserves the number; zero shifted left stays zero.

Return zero when the right-shift count is **at least the digit count**, not digit count minus one. A digit count and a last index are different things.

Support the construction, assignment, +, +=, comparisons, increments, shifts, compound shifts and stream operations exercised by the subject main. That main includes a **bigint-valued right-shift count**: integer-only shift parameters do not automatically cover it. Avoid blindly converting an arbitrarily large count to a machine integer.

Check: 0 + 0, 999 + 1, unequal lengths, equal numbers, differences near the last digit, shifts by 0 / length - 1 / length / greater than length.

## 4. Level 0 — polyset

**Bag:** a collection that permits repeated values.
Insert 3, 3, 8: a bag may contain [3, 3, 8].

**Set:** a collection of distinct values.
Insert 3, 3, 8: the set contains [3, 8].

**has(3)** asks “is 3 present?” It returns true or false; it does not insert or remove anything.

The exercise gives you:
- bag: the required operations insert, print and clear.
- searchable_bag: adds the requirement has.
- array_bag: storage and operations using an array.
- tree_bag: storage and operations using a binary search tree.

Your first job is adding search:
- searchable_array_bag combines array_bag and searchable_bag. Scan the stored elements; return true on a match, otherwise false after the scan.
- searchable_tree_bag combines tree_bag and searchable_bag. Compare at each node: equal means found; smaller means go left; larger means go right. Reaching a null pointer means absent.

Your second job is a **set wrapper** around a searchable_bag:
- Insert x only if has(x) is false.
- For bulk insertion, apply the same duplicate check to every item, including duplicates within the incoming batch.
- Forward search, printing and clearing to the wrapped bag; expose get_bag as required by the main.

**Inheritance:** the searchable array bag IS an array bag and fulfils the searchable_bag interface.
**Composition/wrapping:** the set USES a searchable bag.

**Abstract class:** has an unimplemented pure virtual function such as `virtual bool has(int) const = 0;`. You cannot instantiate it directly. A concrete derived class supplies the missing behavior.

**Polymorphism:** a searchable_bag pointer can refer to either concrete implementation. Calling its virtual has function chooses the actual object's implementation.

**Virtual inheritance:** both inheritance paths reach bag; virtual inheritance lets them share one bag base subobject.

**protected:** derived classes can access the supplied storage. Keep has const because searching should not change the bag.

Do not assume the wrapper owns the supplied bag: the main passes an existing object by reference. Decide copying and lifetime behavior from that interface; do not delete a borrowed object.

A bag abstraction permits duplicates, but a particular tree implementation may already reject them. The set must enforce uniqueness regardless of the storage it wraps.

Study order: bag/set meaning → array search → tree search → inheritance/interfaces → wrapper → canonical form and const correctness.

## 5. Level 1 — life (C)

**Goal:** read commands that draw an initial board, run Conway's Game of Life, then print the final board.

Invocation: `./life width height iterations`.
The pen starts at the top-left. w/a/s/d move up/left/down/right; x toggles drawing. Read commands until end of input. Check the actual subject for any unspecified pen movement boundary behavior; never access outside allocated storage.

Each cell has up to EIGHT neighbors: surrounding cells, excluding itself. Cells outside the board count as dead.

**Rules for the next generation:**
- A living cell survives with 2 or 3 live neighbors.
- A dead cell becomes alive with exactly 3 live neighbors.
- Every other cell is dead next time.

**Two boards:** read only from the old board while writing the next board. After processing every cell, swap them. Updating the current board in place makes later calculations use a mixture of generations.

Example: a vertical line of three live cells becomes a horizontal line, then vertical again.

At zero iterations, print the initial drawing. Final output uses capital O for alive and a space for dead, with a newline after every row.

Build in stages: allocate empty board → read/draw commands → count neighbors → generate next board → repeat → print/free.

Allowed functions in your saved subject: atoi, read, putchar, malloc, calloc, realloc, free.

## 6. Level 1 — bsq (C)

**Goal:** find the largest obstacle-free square and replace its empty cells with the full character.

Your subject uses a SPACE-SEPARATED header, e.g. `9 . o x`: row count, empty character, obstacle character, full character.

First validate:
- Positive row count and at least one column.
- Exactly the declared number of rows; all widths equal.
- Row-ending newlines.
- Three distinct printable marker characters.
- Body cells contain only empty or obstacle markers.

An invalid map prints `map error\n` to stderr; then continue with the next map. With no filename, read stdin. With multiple map arguments, follow the subject's extra separating newline rule.

**Dynamic programming:** let dp[row][col] mean “side length of the biggest empty square whose bottom-right corner is here.”

- Obstacle → 0.
- Empty cell on first row or first column → 1.
- Other empty cell → 1 + minimum(top, left, top-left).

Why minimum? A bigger square needs support from all three neighboring squares.

Track the biggest side length and its position. With a top-to-bottom, left-to-right scan, replacing the winner only for a STRICTLY larger square preserves the required tie-break: topmost, then leftmost.

If the best bottom-right corner is (r, c) and side is s, the top-left is (r - s + 1, c - s + 1). Fill exactly that square, then print the map. If all cells are obstacles, no square is filled.

Check: one cell, one row, all empty, all obstacles, equal-size ties, malformed headers, uneven rows and several input maps. Free each map's allocations before processing the next.

Allowed functions/globals in your saved subject: malloc, calloc, realloc, free, fopen, fclose, getline, fscanf, fputs, fprintf, stderr, stdout, stdin, errno.

## 7. Recall without looking

1. Why does assignment return T& while addition returns T?
2. What do this and *this mean?
3. Starting at 7, what are old and a after old = a++?
4. For sum = 19, what are the output digit and next carry?
5. What are 42 >> 1 and 42 >> 2 in bigint?
6. Insert 3, 3, 8: what differs between bag and set?
7. Does has insert anything?
8. Why does life need a separate next board?
9. What does a BSQ dp value of 3 represent?
10. How do you preserve BSQ's tie-break?

Answers: (1) assignment modifies an existing object; addition produces a separate result. (2) pointer / object. (3) old = 7, a = 8. (4) 9 and 1. (5) 4 and 0. (6) a bag permits repeated 3; a set keeps one. (7) No. (8) every next-state calculation must use the same old generation. (9) a 3-by-3 empty square ending at that cell. (10) scan row by row and update only for a larger square.

Travel practice: read one exercise, hide the notes, and explain its representation, algorithm, return types and three edge cases aloud. Reading builds recall; readiness still needs implementation from empty files and tests.
