<!-- markdownlint-disable -->
# To-Do List and Task Manager

> DSA Lab Project · C

A menu-driven console application in C that stores tasks in a **singly linked list** and lets the user add, edit, complete, delete, sort, filter and search them.

**Main DSA concepts:** Linked List, Sorting

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

The list grows dynamically with `malloc`, so it does not require a fixed number of tasks. Deleting a task re-points a link without shifting elements as an array would require.

## Features

- Add tasks with a title, priority and deadline.
- Edit a task's title, priority or deadline.
- Mark tasks as completed or delete them.
- Display all, pending or completed tasks.
- Sort by priority, deadline or title using bubble sort.
- Search task titles case-insensitively by keyword.
- Validate numeric input, priorities and calendar dates, including leap years.
- Free all allocated nodes when the program exits.

## Complexity

For `n` tasks, adding, finding, deleting, displaying and filtering take `O(n)` time. Sorting takes `O(n²)` time in the worst case and `O(1)` extra space. The task list uses `O(n)` space.

## Build and run

```bash
gcc TaskManager.c -o taskmanager
./taskmanager          # on Windows: taskmanager.exe
```

## Limitations

- Tasks are kept in memory only and are lost when the program exits.
- Titles are limited to 30 characters.
- Deadlines are not compared against today's date, so overdue tasks are not flagged.
- Bubble sort may be slow for very large lists.

## Possible extensions

- Save and load tasks from a file.
- Replace bubble sort with merge sort for `O(n log n)` sorting on linked lists.
- Add a tail pointer so task insertion is `O(1)`.
- Highlight overdue tasks using the system date.

## Author

Kavyansh Gupta · [GitHub](https://github.com/kavyansh09)
