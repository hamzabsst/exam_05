#include "searchable_array_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "set.hpp"

#include <climits>
#include <iostream>
#include <set>
#include <streambuf>

static const char *current_case;
static unsigned long checks = 0;
static unsigned long failures = 0;
static unsigned int cases = 0;

// The supplied tree prints every insertion/deletion. Keep test results readable.
class quiet_output : public std::streambuf {
    std::streambuf *previous;
public:
    quiet_output() : previous(std::cout.rdbuf(this)) {}
    ~quiet_output() { std::cout.rdbuf(previous); }
protected:
    int_type overflow(int_type c) { return traits_type::not_eof(c); }
};

static void check(bool condition, const char *message) {
    ++checks;
    if (!condition) {
        ++failures;
        if (failures <= 20)
            std::cerr << "[FAIL] " << current_case << ": " << message << '\n';
    }
}

template <typename Array, typename Tree>
static void compare_has(const Array &array, const Tree &tree,
                        const std::set<int> &expected, int value) {
    const bool a = array.has(value);
    const bool t = tree.has(value);
    const bool want = expected.count(value) != 0;
    ++checks;
    if (a != want || t != want) {
        ++failures;
        if (failures <= 20)
            std::cerr << "[FAIL] " << current_case << ": has(" << value
                      << ") array=" << a << " tree=" << t
                      << " expected=" << want << '\n';
    }
}

template <typename Array, typename Tree>
static void verify(const Array &array, const Tree &tree,
                   const std::set<int> &expected) {
    for (int value = -70; value <= 70; ++value)
        compare_has(array, tree, expected, value);
    const int edges[] = {INT_MIN, INT_MIN + 1, -1000, 1000, INT_MAX - 1, INT_MAX};
    for (unsigned int i = 0; i < sizeof(edges) / sizeof(edges[0]); ++i)
        compare_has(array, tree, expected, edges[i]);
    for (std::set<int>::const_iterator it = expected.begin(); it != expected.end(); ++it)
        compare_has(array, tree, expected, *it);
}

struct bags {
    searchable_array_bag array;
    searchable_tree_bag tree;
    std::set<int> expected;

    void verify_all() const {
        // Query through const base references to exercise virtual dispatch.
        const searchable_bag &a = array;
        const searchable_bag &t = tree;
        verify(a, t, expected);
    }

    void insert(int value) {
        bag &a = array;
        bag &t = tree;
        a.insert(value);
        t.insert(value);
        expected.insert(value);
    }

    void insert(int *values, int count) {
        bag &a = array;
        bag &t = tree;
        a.insert(values, count);
        t.insert(values, count);
        for (int i = 0; i < count; ++i)
            expected.insert(values[i]);
    }

    void clear() {
        bag &a = array;
        bag &t = tree;
        a.clear();
        t.clear();
        expected.clear();
    }
};

static void empty_and_single() {
    bags b;
    b.verify_all();
    b.insert(0);
    b.verify_all();
    b.insert(0);
    b.verify_all();
}

static void both_branches() {
    bags b;
    int values[] = {10, 5, 15, 2, 7, 12, 20, 1, 3, 6, 8, 11, 13, 18, 22};
    for (unsigned int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        b.insert(values[i]);
        b.verify_all();
    }
}

static void ascending() {
    bags b;
    for (int i = -64; i <= 64; ++i) {
        b.insert(i);
        b.verify_all();
    }
}

static void descending() {
    bags b;
    for (int i = 64; i >= -64; --i) {
        b.insert(i);
        b.verify_all();
    }
}

static void bulk_and_boundaries() {
    bags b;
    int values[] = {INT_MIN, INT_MAX, 0, -1, 1, -42, 42, 0, INT_MIN, INT_MAX};
    b.insert(values, sizeof(values) / sizeof(values[0]));
    b.verify_all();
    b.insert(values, sizeof(values) / sizeof(values[0]));
    b.verify_all();
    b.insert(values, 0);
    b.verify_all();
    b.insert(0, 0);
    b.verify_all();
}

static void clear_and_reuse() {
    bags b;
    for (int round = 0; round < 8; ++round) {
        b.clear();
        b.clear();
        b.verify_all();
        int values[] = {round, -round, round * 7, INT_MIN, INT_MAX};
        b.insert(values, sizeof(values) / sizeof(values[0]));
        b.verify_all();
    }
    b.clear();
    b.verify_all();
}

static void copy_independence() {
    bags original;
    bags empty_copy(original);
    empty_copy.verify_all();
    int values[] = {0, -10, 10, -20, 20, INT_MIN, INT_MAX, 10};
    original.insert(values, sizeof(values) / sizeof(values[0]));
    bags copy(original);
    copy.verify_all();
    copy.insert(55);
    original.verify_all();
    original.insert(-55);
    copy.verify_all();
    original.clear();
    original.verify_all();
    copy.verify_all();
}

static void assignment_independence() {
    bags source;
    int values[] = {4, 2, 6, 1, 3, 5, 7, INT_MIN, INT_MAX};
    source.insert(values, sizeof(values) / sizeof(values[0]));
    bags destination;
    destination.insert(999);
    check(&(destination.array = source.array) == &destination.array,
          "array assignment must return *this");
    check(&(destination.tree = source.tree) == &destination.tree,
          "tree assignment must return *this");
    destination.expected = source.expected;
    destination.verify_all();
    compare_has(destination.array, destination.tree, destination.expected, 999);
    destination.insert(55);
    source.verify_all();
    source.insert(-55);
    destination.verify_all();
    source.clear();
    destination.verify_all();
    destination = source;
    destination.verify_all();
    destination.insert(42);
    bags &same = destination;
    destination = same;
    destination.verify_all();
}

static void source_lifetime() {
    bags assigned;
    bags *copied;
    {
        bags source;
        int values[] = {-5, 0, 5, INT_MIN, INT_MAX};
        source.insert(values, sizeof(values) / sizeof(values[0]));
        assigned = source;
        copied = new bags(source);
    }
    assigned.verify_all();
    copied->verify_all();
    delete copied;
}

static unsigned long next_random(unsigned long &state) {
    state = (state * 1664525UL + 1013904223UL) & 0xffffffffUL;
    return state >> 16;
}

static void mixed_operations() {
    bags b;
    unsigned long state = 42;
    for (int step = 0; step < 600; ++step) {
        const unsigned long operation = next_random(state) % 12;
        if (operation == 0) {
            b.clear();
        } else if (operation < 4) {
            int values[7];
            for (int i = 0; i < 7; ++i)
                values[i] = static_cast<int>(next_random(state) % 401) - 200;
            b.insert(values, 7);
        } else {
            b.insert(static_cast<int>(next_random(state) % 401) - 200);
        }
        b.verify_all();
        for (int i = 0; i < 8; ++i)
            compare_has(b.array, b.tree, b.expected,
                        static_cast<int>(next_random(state) % 1001) - 500);
    }
}

template <typename Bag>
class counting_bag : public Bag {
public:
    int insertions;
    counting_bag() : insertions(0) {}
    void insert(int value) {
        ++insertions;
        Bag::insert(value);
    }
    void insert(int *values, int count) { Bag::insert(values, count); }
};

static void set_wrappers() {
    counting_bag<searchable_array_bag> a;
    counting_bag<searchable_tree_bag> t;
    set array_set(a);
    set tree_set(t);
    std::set<int> expected;
    verify(array_set, tree_set, expected);
    check(&array_set.get_bag() == &a, "array get_bag must expose its backing bag");
    check(&tree_set.get_bag() == &t, "tree get_bag must expose its backing bag");
    int values[] = {5, -5, 0, 5, -5, 12, INT_MIN, INT_MAX, 0};
    for (unsigned int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        array_set.insert(values[i]);
        tree_set.insert(values[i]);
        expected.insert(values[i]);
        verify(array_set, tree_set, expected);
    }
    int bulk[] = {5, 0, 7, 7, -7, INT_MIN, INT_MAX};
    array_set.insert(bulk, sizeof(bulk) / sizeof(bulk[0]));
    tree_set.insert(bulk, sizeof(bulk) / sizeof(bulk[0]));
    expected.insert(bulk, bulk + sizeof(bulk) / sizeof(bulk[0]));
    verify(array_set, tree_set, expected);
    verify(array_set.get_bag(), tree_set.get_bag(), expected);
    check(a.insertions == static_cast<int>(expected.size()),
          "array set forwarded duplicate insertions");
    check(t.insertions == static_cast<int>(expected.size()),
          "tree set forwarded duplicate insertions");
    array_set.clear();
    tree_set.clear();
    expected.clear();
    verify(array_set, tree_set, expected);
    verify(a, t, expected);
    array_set.insert(42);
    tree_set.insert(42);
    expected.insert(42);
    verify(array_set, tree_set, expected);
    // Changes to the backing bags must be visible through their wrappers.
    a.insert(-42);
    t.insert(-42);
    expected.insert(-42);
    verify(array_set, tree_set, expected);
}

static void run_case(const char *name, void (*test)()) {
    current_case = name;
    ++cases;
    const unsigned long before = failures;
    test();
    std::cerr << (failures == before ? "[PASS] " : "[FAIL] ") << name << '\n';
}

int main() {
    quiet_output quiet;
    run_case("empty bags, zero and duplicate insertion", empty_and_single);
    run_case("left/right branches and missing values", both_branches);
    run_case("ascending insertion / right-skewed tree", ascending);
    run_case("descending insertion / left-skewed tree", descending);
    run_case("bulk insert, duplicates and integer limits", bulk_and_boundaries);
    run_case("repeated clear and reuse", clear_and_reuse);
    run_case("copy constructor and independent storage", copy_independence);
    run_case("assignment, replacement, empty and self-assignment", assignment_independence);
    run_case("copies survive source destruction", source_lifetime);
    run_case("600 deterministic mixed operations", mixed_operations);
    run_case("set wrappers, duplicate filtering and backing bags", set_wrappers);
    std::cerr << cases << " cases, " << checks << " checks, " << failures << " failures\n";
    return failures == 0 ? 0 : 1;
}
