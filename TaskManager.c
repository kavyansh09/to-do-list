#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LEN 31

struct Task {
    int id;
    char title[LEN];
    int priority;   // 1 = High, 2 = Medium, 3 = Low
    int day, month, year;
    int completed;  // 0 = Pending, 1 = Completed
    struct Task *next;
};

struct Task *head = NULL;
int nextId = 1;

// ---------- Input helpers ----------
int readInt(const char *msg) {
    int x, c;
    printf("%s", msg);
    while (scanf("%d", &x) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        if (c == EOF)
            exit(0);
        printf("Invalid input! %s", msg);
    }
    return x;
}

void readTitle(char *title) {
    printf("Enter Title: ");
    scanf(" %30[^\n]", title);
}

int readPriority() {
    int p;
    do {
        p = readInt("Enter Priority (1. High  2. Medium  3. Low): ");
    } while (p < 1 || p > 3);
    return p;
}

int validDate(int d, int m, int y) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (y < 2000 || y > 2100 || m < 1 || m > 12)
        return 0;
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0))
        return d >= 1 && d <= 29;
    return d >= 1 && d <= days[m - 1];
}

void readDate(int *d, int *m, int *y) {
    do {
        *d = readInt("Enter Deadline Day: ");
        *m = readInt("Enter Deadline Month: ");
        *y = readInt("Enter Deadline Year: ");
        if (!validDate(*d, *m, *y))
            printf("Invalid date! Try again.\n");
    } while (!validDate(*d, *m, *y));
}

// ---------- Display ----------
const char *priorityName(int p) {
    if (p == 1) return "High";
    if (p == 2) return "Medium";
    return "Low";
}

// filter: 0 = all, 1 = pending, 2 = completed
void displayTasks(int filter) {
    struct Task *t;
    int found = 0;

    printf("\n%-4s %-30s %-8s %-11s %-9s\n", "ID", "Title", "Priority", "Deadline", "Status");
    printf("----------------------------------------------------------------------\n");

    for (t = head; t != NULL; t = t->next) {
        if (filter == 1 && t->completed) continue;
        if (filter == 2 && !t->completed) continue;

        printf("%-4d %-30s %-8s %02d-%02d-%04d  %-9s\n", t->id, t->title, priorityName(t->priority),
               t->day, t->month, t->year, t->completed ? "Completed" : "Pending");
        found = 1;
    }
    if (!found)
        printf("No tasks found!\n");
}

// ---------- Module 1: Create ----------
void addTask() {
    struct Task *t = (struct Task *)malloc(sizeof(struct Task));
    struct Task *p;

    if (t == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    t->id = nextId++;
    readTitle(t->title);
    t->priority = readPriority();
    readDate(&t->day, &t->month, &t->year);
    t->completed = 0;
    t->next = NULL;

    if (head == NULL) {
        head = t;
    } else {
        p = head;
        while (p->next != NULL)
            p = p->next;
        p->next = t;
    }
    printf("Task added! Task ID: %d\n", t->id);
}

// ---------- Module 2: Update and delete ----------
struct Task *findTask(int id) {
    struct Task *t = head;
    while (t != NULL && t->id != id)
        t = t->next;
    return t;
}

void editTask() {
    struct Task *t = findTask(readInt("Enter Task ID to edit: "));
    int ch;

    if (t == NULL) {
        printf("Task not found!\n");
        return;
    }

    printf("1. Title\n2. Priority\n3. Deadline\n");
    ch = readInt("What do you want to edit? ");

    if (ch == 1)
        readTitle(t->title);
    else if (ch == 2)
        t->priority = readPriority();
    else if (ch == 3)
        readDate(&t->day, &t->month, &t->year);
    else {
        printf("Invalid choice!\n");
        return;
    }
    printf("Task updated!\n");
}

void markCompleted() {
    struct Task *t = findTask(readInt("Enter Task ID to mark completed: "));

    if (t == NULL)
        printf("Task not found!\n");
    else if (t->completed)
        printf("Task is already completed!\n");
    else {
        t->completed = 1;
        printf("Task marked as completed!\n");
    }
}

void deleteTask() {
    int id = readInt("Enter Task ID to delete: ");
    struct Task *cur = head, *prev = NULL;

    while (cur != NULL && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Task not found!\n");
        return;
    }

    if (prev == NULL)
        head = cur->next;       // deleting the first node
    else
        prev->next = cur->next;

    free(cur);
    printf("Task deleted!\n");
}

// ---------- Module 3: Sorting (bubble sort on the linked list) ----------
int dateValue(struct Task *t) {
    return t->year * 10000 + t->month * 100 + t->day;
}

int cmpTitle(const char *a, const char *b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int outOfOrder(struct Task *a, struct Task *b, int by) {
    if (by == 1) return a->priority > b->priority;
    if (by == 2) return dateValue(a) > dateValue(b);
    return cmpTitle(a->title, b->title) > 0;
}

// swaps the data of two nodes but keeps the links intact
void swapData(struct Task *a, struct Task *b) {
    struct Task temp = *a;
    struct Task *an = a->next, *bn = b->next;

    *a = *b;
    *b = temp;
    a->next = an;
    b->next = bn;
}

void sortTasks() {
    struct Task *p, *last = NULL;
    int by, swapped;

    if (head == NULL || head->next == NULL) {
        printf("Not enough tasks to sort!\n");
        return;
    }

    by = readInt("Sort by (1. Priority  2. Deadline  3. Title): ");
    if (by < 1 || by > 3) {
        printf("Invalid choice!\n");
        return;
    }

    do {
        swapped = 0;
        for (p = head; p->next != last; p = p->next) {
            if (outOfOrder(p, p->next, by)) {
                swapData(p, p->next);
                swapped = 1;
            }
        }
        last = p;
    } while (swapped);

    printf("Tasks sorted!\n");
    displayTasks(0);
}

// ---------- Module 4: Search ----------
int contains(const char *text, const char *key) {
    char t[LEN], k[LEN];
    int i;

    for (i = 0; text[i]; i++) t[i] = tolower((unsigned char)text[i]);
    t[i] = '\0';
    for (i = 0; key[i]; i++) k[i] = tolower((unsigned char)key[i]);
    k[i] = '\0';

    return strstr(t, k) != NULL;
}

void searchTask() {
    char key[LEN];
    struct Task *t;
    int found = 0;

    printf("Enter keyword: ");
    scanf(" %30[^\n]", key);

    for (t = head; t != NULL; t = t->next) {
        if (contains(t->title, key)) {
            if (!found)
                printf("\n%-4s %-30s %-8s %-11s %-9s\n", "ID", "Title", "Priority", "Deadline", "Status");
            printf("%-4d %-30s %-8s %02d-%02d-%04d  %-9s\n", t->id, t->title, priorityName(t->priority),
                   t->day, t->month, t->year, t->completed ? "Completed" : "Pending");
            found = 1;
        }
    }
    if (!found)
        printf("No task matches the keyword!\n");
}

int main() {
    int choice;
    struct Task *t;

    do {
        printf("\n===== TO-DO LIST AND TASK MANAGER =====\n");
        printf("1. Add Task\n");
        printf("2. Edit Task\n");
        printf("3. Mark Task Completed\n");
        printf("4. Delete Task\n");
        printf("5. Sort Tasks\n");
        printf("6. Display All Tasks\n");
        printf("7. Display Pending Tasks\n");
        printf("8. Display Completed Tasks\n");
        printf("9. Search Task by Keyword\n");
        printf("0. Exit\n");
        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: addTask(); break;
            case 2: editTask(); break;
            case 3: markCompleted(); break;
            case 4: deleteTask(); break;
            case 5: sortTasks(); break;
            case 6: displayTasks(0); break;
            case 7: displayTasks(1); break;
            case 8: displayTasks(2); break;
            case 9: searchTask(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    while (head != NULL) {   // free the whole list
        t = head;
        head = head->next;
        free(t);
    }
    return 0;
}
