# Exam Rank 05 practice

Study material and practice implementations for 42 Exam Rank 05, organized by level and exercise. This repository records preparation work; it does not claim that the exam has been passed or that every implementation is correct.

## Layout

| Path | Contents |
| --- | --- |
| `level1/bigint/` | Arbitrary-precision unsigned integer practice |
| `level1/polyset/` | Bags, searchable bags, and set-related classes |
| `level1/vect2/` | Two-dimensional vector class and operator practice |
| `level2/bsq/` | Largest-square grid exercise in C |
| `level2/life/` | Game of Life exercise in C |
| `practice/` | Additional practice versions |
| `archive/` | Earlier versions and preserved build artifacts |

Exercise folders include combinations of source files, subject text (`sub.txt`), entry points, and tester scripts. Start with the level folders and read the local subject before comparing implementations.

## Compile an exercise

There is no root Makefile. Requirements vary by exercise: a C compiler or a C++98 compiler, with Bash and optionally Valgrind for the tester scripts.

For example, build the supplied vector exercise:

```sh
cd level1/vect2
c++ -Wall -Wextra -Werror -std=c++98 main.cpp vect2.cpp -o vect2
./vect2
```

Review each tester before running it. Some compare against an external `rendu/` folder, create temporary files, or require Valgrind, so they are not all standalone test commands.

## Study workflow

1. Read the subject and identify the required interface and allowed functions.
2. Try an implementation independently.
3. Compile with warnings enabled and exercise normal and edge cases.
4. Compare behavior with the supplied examples and inspect memory ownership.

Old binaries in `archive/` are preserved history; compile the sources for your own machine. The current school subject remains the authority for exam requirements.
