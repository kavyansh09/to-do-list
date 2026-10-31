# To-Do List and Task Manager

> DSA Lab Project · C

A menu-driven console application in C that stores tasks in a **singly linked list** and lets the user add, edit, complete, delete, sort, filter and search them.

**Main DSA concepts:** Linked List, Sorting

For a beginner-friendly walkthrough of the code, see [DSA_TaskManager_Explained.md](DSA_TaskManager_Explained.md).

---

## Modules

| # | Module | What it does | Menu options |
|---|---|---|---|
| 1 | Task Creation | Add a task (title, priority, deadline, status) as a new linked list node | 1 |
| 2 | Task Update and Deletion | Edit a task's title, priority or deadline; mark it completed; delete it | 2, 3, 4 |
| 3 | Task Sorting | Sort tasks by priority, deadline or title | 5 |
| 4 | Task Display and Filter | Display all, pending or completed tasks; search by keyword | 6, 7, 8, 9 |

## Data structure

Each task is one node of the list:

```c
struct Task {
    int id;               // auto-generated, starts at 1
    char title[31];
    int priority;         // 1 = High, 2 = Medium, 3 = Low
    int day, month, year; // deadline
    int completed;        // 0 = Pending, 1 = Completed
    struct Task *next;    // link to the next node
};
```

```
head --> [1|Submit report|...] --> [2|Buy groceries|...] --> [3|Fix bug|...] --> NULL
```

`head` is a global pointer to the first node. An empty list is `head == NULL`.

**Why a linked list?** The number of tasks is not known in advance, so the list grows one node at a time with `malloc` and needs no fixed size. Deleting a task re-points one link, with no shifting of elements as an array would need.

## How each operation works

**Add (insertion at the tail).** A new node is allocated with `malloc`. If the list is empty it becomes `head`; otherwise the code walks to the last node and links the new one after it.

**Find.** `findTask(id)` walks the list until it finds the ID or reaches `NULL`. Edit and mark-completed both use it.

**Delete.** Two pointers are kept while walking: `cur` (the node to remove) and `prev` (the node before it). If `prev` is `NULL`, the first node is being deleted and `head` moves forward; otherwise `prev->next` skips over `cur`. The node is then released with `free`.

**Sort (bubble sort on the list).** Repeated passes compare each node with its neighbour and swap them if out of order. After each pass the largest remaining element is in its final place, so the `last` marker moves one node closer to the front. A `swapped` flag ends the loop early when a pass makes no swaps. The code swaps the **data** of two nodes and leaves the `next` links untouched.

| Sort key | Comparison used |
|---|---|
| Priority | `1 (High)` before `2 (Medium)` before `3 (Low)` |
| Deadline | Date converted to `year*10000 + month*100 + day`, so earlier dates compare smaller |
| Title | Case-insensitive alphabetical order |

The sort is stable: tasks with equal keys keep their previous relative order. It changes the list itself, so later displays stay in sorted order.

**Display and filter.** One function walks the list and prints only the nodes that match the chosen filter (all, pending or completed).

**Search.** Walks the list and prints every task whose title contains the keyword, ignoring case.

## Complexity (n = number of tasks)

| Operation | Time | Note |
|---|---|---|
| Add task | O(n) | Walks to the tail; O(1) with a tail pointer |
| Find / edit / mark completed | O(n) | Linear search by ID |
| Delete | O(n) | O(n) to find the node, O(1) to unlink |
| Sort | O(n²) worst case, O(n) if already sorted | Bubble sort with early exit, O(1) extra space |
| Display / filter | O(n) | |
| Search by keyword | O(n x m) | m = title length |

Space used is O(n): one node per task.

## Input validation

- Non-numeric input is rejected and asked again instead of crashing.
- Priority must be 1, 2 or 3.
- Deadline must be a real calendar date (month 1-12, correct days per month, leap years handled, years 2000-2100). For example, 31-02-2026 is rejected.
- Titles are limited to 30 characters and may contain spaces.
- Edit, complete and delete report "Task not found!" for unknown IDs.
- All nodes are freed when the program exits.

## Build and run

```bash
gcc TaskManager.c -o taskmanager
./taskmanager          # on Windows: taskmanager.exe
```

## Sample output

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

Enter choice: 9
Enter keyword: BUG

ID   Title                          Priority Deadline    Status
3    Fix bug                        High     30-11-2026  Pending
```

<!-- Add a screenshot of your own run here:  ![Demo](screenshot.png) -->

## Limitations

- Tasks are kept in memory only and are lost when the program exits.
- Title length is fixed at 30 characters.
- Deadlines are not compared against today's date, so overdue tasks are not flagged.
- Bubble sort is O(n²), fine for a personal task list but slow for very large lists.

## Possible extensions

- Save and load tasks from a file.
- Use merge sort, which suits linked lists and runs in O(n log n).
- Keep a tail pointer so adding a task is O(1).
- Use a doubly linked list.
- Highlight overdue tasks using the system date.

## Author

Kavyansh Gupta · [GitHub](https://github.com/kavyansh09)
