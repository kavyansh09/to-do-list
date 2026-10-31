<!-- markdownlint-disable -->

# To-Do List and Task Manager — Explained Simply

**Subject:** DSA Lab | **Language:** C | **File:** `TaskManager.c`
**Main concepts:** Linked List, Sorting

This document explains what the project does and how the code works, in plain language. You do not need to read the code first.

---

## 1. What is this project?

A to-do list app that runs in the terminal. You can add tasks (with a title, priority and deadline), edit them, mark them done, delete them, sort them, filter them and search them.

The special part is **how the tasks are stored**: in a **linked list**, not an array. And to put them in order we use a **sorting algorithm** (bubble sort) that we wrote ourselves.

Everything lives in memory while the program runs. When you close it, the tasks are gone.

### The four modules

| # | Module | What it does | Menu option |
|---|---|---|---|
| 1 | Task creation | Add a task as a new node in the linked list | 1 |
| 2 | Task update and deletion | Edit a task, mark it completed, delete it | 2, 3, 4 |
| 3 | Task sorting | Sort by priority, deadline or title | 5 |
| 4 | Task display and filter | Show all / pending / completed tasks, search by keyword | 6, 7, 8, 9 |

---

## 2. How to run it

```bash
gcc TaskManager.c -o taskmanager
./taskmanager
```

---

## 3. What is a linked list? (the key idea)

An **array** is like a row of lockers side by side. The size is fixed, and inserting or deleting in the middle means shifting everything.

A **linked list** is like a **treasure hunt**. Each clue (called a **node**) holds some information *and* tells you where the next clue is. The nodes can be anywhere in memory; they are connected only by these "where to go next" pointers.

```
head ──► [ Task 1 | next ]──► [ Task 2 | next ]──► [ Task 3 | next ]──► NULL
```

- **`head`** points to the first node. If you lose `head`, you lose the whole list.
- **`NULL`** means "nothing here". The last node's `next` is `NULL`, so we know the list ended.
- An **empty list** is when `head` is `NULL`.

**Why use it here?** We don't know how many tasks the user will add. A linked list grows one node at a time using `malloc`, so there is no fixed limit. Deleting a task just means changing one link, with no shifting needed.

---

## 4. The node: what one task looks like

```c
struct Task {
    int id;               // automatic number: 1, 2, 3...
    char title[31];       // task name, up to 30 characters
    int priority;         // 1 = High, 2 = Medium, 3 = Low
    int day, month, year; // deadline
    int completed;        // 0 = Pending, 1 = Completed
    struct Task *next;    // pointer to the next task
};
```

The last line is what makes it a linked list: a task that contains a pointer to another task.

Two global variables hold the list:

```c
struct Task *head = NULL;   // start of the list
int nextId = 1;             // next task ID to hand out
```

---

## 5. Walking through the code

### 5.1 Helper functions (input and checking)

| Function | What it does |
|---|---|
| `readInt(msg)` | Prints a message and reads a number. If the user types letters, it clears the bad input and asks again, so the program never gets stuck |
| `readTitle()` | Reads a title, spaces allowed (`%30[^\n]` means "read up to 30 characters until Enter") |
| `readPriority()` | Keeps asking until the user enters 1, 2 or 3 |
| `validDate(d, m, y)` | Checks the date is real. Handles different month lengths and leap years, so 31-02-2026 is rejected |
| `readDate()` | Keeps asking day, month, year until `validDate` says OK |
| `priorityName(p)` | Turns 1/2/3 into "High"/"Medium"/"Low" for printing |

### 5.2 Module 1: `addTask()` — create a node

Steps:
1. `malloc` creates a new empty node.
2. Fill in its ID, title, priority, deadline. Status starts as Pending. `next = NULL`.
3. **Attach it at the end:** if the list is empty, the new node becomes `head`. Otherwise, walk to the last node and set its `next` to the new node.

```
Before:  head ► [A] ► [B] ► NULL
Add C:   head ► [A] ► [B] ► [C] ► NULL
```

### 5.3 Module 2: edit, complete, delete

**`findTask(id)`** walks the list from `head` until it finds the ID or reaches `NULL`. Edit and "mark completed" both use it. Edit lets you change the title, priority or deadline. Mark completed sets `completed = 1`.

**`deleteTask()`** is the classic linked list operation. We walk with **two pointers**: `cur` (the node we want to remove) and `prev` (the one just before it).

```
Delete B:
Before:  head ► [A] ► [B] ► [C] ► NULL
                  ▲prev   ▲cur
Step:    prev->next = cur->next     (A now points to C)
After:   head ► [A] ──────► [C] ► NULL        then free(B)
```

Special case: if the node to delete is the **first** one (`prev` is `NULL`), we just move `head` to the next node.

### 5.4 Module 3: sorting (bubble sort)

**The idea of bubble sort:** go through the list comparing each pair of neighbours. If a pair is in the wrong order, swap them. Repeat until a full pass makes no swaps. Big values "bubble" to the end, like bubbles rising in water.

Tiny example, sorting priorities `3, 1, 2`:

```
Pass 1:  (3,1) wrong → swap → 1, 3, 2
         (3,2) wrong → swap → 1, 2, 3
Pass 2:  (1,2) ok, (2,3) ok → no swaps → STOP
```

**Which order?** The user picks one of three keys. The function `outOfOrder(a, b, by)` decides whether two neighbours must be swapped:

| Sort by | How two tasks are compared |
|---|---|
| Priority | Smaller number first, so High (1) comes before Low (3) |
| Deadline | The date is turned into one number `year×10000 + month×100 + day` (e.g. 15-11-2026 → 20261115). Smaller number means earlier date |
| Title | Alphabetical, ignoring upper/lower case (`cmpTitle`) |

**How the swap works:** `swapData()` swaps the **contents** of the two nodes but keeps the `next` links where they are. This is much easier than re-wiring pointers, and it keeps the list connected.

The variable `last` marks where the already-sorted part begins. After each pass, the biggest unsorted item is in its final place, so the next pass can stop one node earlier. The `swapped` flag lets the sort stop early if the list is already sorted.

Equal items are never swapped (we only swap when one is strictly greater), so their original order is kept. This is called a **stable** sort.

### 5.5 Module 4: display, filter, search

**`displayTasks(filter)`** is one function for three menu options. It walks the whole list and prints a task only if it matches:

| filter | Shows |
|---|---|
| 0 | all tasks |
| 1 | pending only |
| 2 | completed only |

**`searchTask()`** asks for a keyword and prints every task whose title contains it. Case does not matter ("BUG" finds "Fix bug"). It uses `contains()`, which converts both texts to lowercase and uses `strstr` to look for the keyword inside the title.

### 5.6 `main()`

A `do { ... } while (choice != 0);` loop shows the menu repeatedly, and a `switch` calls the right function. When the user exits, a final loop walks the list and `free`s every node, so no memory is leaked.

---

## 6. Example run

```
Enter choice: 5
Sort by (1. Priority  2. Deadline  3. Title): 2
Tasks sorted!

ID   Title                          Priority Deadline    Status
----------------------------------------------------------------------
2    buy groceries                  Low      02-11-2026  Pending
1    Submit DSA report              Medium   15-11-2026  Pending
3    Fix bug                        High     30-11-2026  Pending
4    Alpha study                    High     05-12-2026  Pending
```

Sorted by deadline: earliest date first.

---

## 7. Speed of each operation (time complexity)

Let **n** = number of tasks. "O(n)" means the work grows in step with the number of tasks, because we may need to look at every node.

| Operation | Time | Why |
|---|---|---|
| Add task | O(n) | Must walk to the last node first |
| Find / edit / complete | O(n) | Walk until the ID is found |
| Delete | O(n) | Walk to find it; unlinking itself is quick |
| Sort (bubble sort) | O(n²) worst case | Up to n passes, each up to n comparisons. O(n) if already sorted |
| Display / filter / search | O(n) | Look at every node once |

Memory used: O(n), one node per task.

---

## 8. Key terms

| Term | Meaning |
|---|---|
| Node | One item of the list (here, one task) |
| Pointer | A variable that stores the memory address of something else |
| `head` | Pointer to the first node |
| `NULL` | "Points to nothing"; marks the end of the list |
| `malloc` / `free` | Ask the system for memory / give it back |
| Traversal | Walking through the list node by node |
| Singly linked | Each node knows only the next node, not the previous one |
| Stable sort | Equal items keep their original order |

---

## 9. Limitations

- Tasks are not saved; they disappear when the program closes.
- Titles are limited to 30 characters.
- Deadlines are not compared with today's date, so overdue tasks are not flagged.
- Bubble sort is slow for very large lists (fine for a personal to-do list).

**Possible improvements:** save to a file; merge sort (O(n log n), works well on linked lists); a tail pointer so adding is instant; highlight overdue tasks.

---

## 10. Likely viva questions

**Q: Why a linked list and not an array?**
The number of tasks is unknown. A linked list grows as needed and deletion only changes a link, with no shifting.

**Q: What is a node here?**
A `struct Task` holding the task data plus a `next` pointer.

**Q: What kind of linked list is it?**
Singly linked: each node points only forward.

**Q: How do you delete a node?**
Keep a `prev` pointer behind `cur`. Set `prev->next = cur->next`, then `free(cur)`. If it is the first node, move `head` instead.

**Q: Which sorting algorithm did you use and what is its complexity?**
Bubble sort. O(n²) worst case, O(n) best case (already sorted, thanks to the early-exit flag), O(1) extra space.

**Q: How do you sort a linked list without moving nodes around?**
`swapData()` swaps the data inside two nodes and leaves the links alone.

**Q: How is the deadline compared?**
Converted to a single number `year×10000 + month×100 + day`, so a normal integer comparison gives the earlier date.

**Q: Why is adding O(n)?**
We walk to the tail every time. Keeping a tail pointer would make it O(1).

**Q: What would be a better sort for this list?**
Merge sort: O(n log n) and it suits linked lists.

**Q: How do you avoid memory leaks?**
Every node created with `malloc` is freed on delete, and the exit loop frees all remaining nodes.
