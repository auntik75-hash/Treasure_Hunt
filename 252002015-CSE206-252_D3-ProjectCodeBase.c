/* ============================================================
   TREASURE HUNT GAME  -  Data Structure Lab Project
   Covers:
   1. 1D/2D Array operations, insertion, deletion
   2. Linear Search & Binary Search
   3. Bubble Sort (leaderboard)
   4-6. Linked List: traversal, insertion, deletion (inventory)
   7. Graph traversal - BFS (auto solve shortest path)
   8-10. Binary Tree / Binary Search Tree operations (player records)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 8          /* board dimension (2D array) */
#define MAX_PLAYERS 20

char board[SIZE][SIZE];
int playerRow, playerCol;
int treasureRow, treasureCol;

/* ---------- Linked List (Inventory) ---------- */
typedef struct Node {
    char item[20];
    struct Node* next;
} Node;
Node* inventory = NULL;

/* ---------- Binary Search Tree (Player Records) ---------- */
typedef struct TreeNode {
    char name[20];
    int score;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
TreeNode* root = NULL;

/* ---------- Leaderboard Array ---------- */
typedef struct {
    char name[20];
    int score;
} Player;
Player leaderboard[MAX_PLAYERS];
int playerCount = 0;

/* ============================================================
   1. ARRAY OPERATIONS - Board setup, print, wall insert/delete
   ============================================================ */
void initBoard() {
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            board[i][j] = '.';

    playerRow = 0; playerCol = 0;
    treasureRow = SIZE - 1; treasureCol = SIZE - 1;
    board[playerRow][playerCol] = 'P';
    board[treasureRow][treasureCol] = 'T';
}

void printBoard() {
    int i, j;
    printf("\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
            printf(" %c ", board[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* insertion into array -> add wall */
void insertWall(int r, int c) {
    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == '.') {
        board[r][c] = '#';
        printf("Wall added at (%d,%d)\n", r, c);
    } else {
        printf("Invalid position!\n");
    }
}

/* deletion from array -> remove wall */
void deleteWall(int r, int c) {
    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == '#') {
        board[r][c] = '.';
        printf("Wall removed at (%d,%d)\n", r, c);
    } else {
        printf("No wall found there!\n");
    }
}

/* ============================================================
   Player Movement
   ============================================================ */
int movePlayer(char dir) {
    int newRow = playerRow, newCol = playerCol;
    if (dir == 'w') newRow--;
    else if (dir == 's') newRow++;
    else if (dir == 'a') newCol--;
    else if (dir == 'd') newCol++;
    else { printf("Invalid key!\n"); return 0; }

    if (newRow < 0 || newRow >= SIZE || newCol < 0 || newCol >= SIZE) {
        printf("Can't move outside the board!\n");
        return 0;
    }
    if (board[newRow][newCol] == '#') {
        printf("Blocked by a wall!\n");
        return 0;
    }

    board[playerRow][playerCol] = '.';
    playerRow = newRow; playerCol = newCol;

    if (playerRow == treasureRow && playerCol == treasureCol) {
        board[playerRow][playerCol] = 'P';
        printf("\n*** Congratulations! You found the treasure! ***\n");
        return 1;
    }
    board[playerRow][playerCol] = 'P';
    return 0;
}

/* ============================================================
   4-6. LINKED LIST - Inventory: insertion, deletion, traversal
   ============================================================ */
void insertItem(char* itemName) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->item, itemName);
    newNode->next = inventory;
    inventory = newNode;
    printf("Item '%s' added to inventory.\n", itemName);
}

void deleteItem(char* itemName) {
    Node *temp = inventory, *prev = NULL;
    while (temp != NULL) {
        if (strcmp(temp->item, itemName) == 0) {
            if (prev == NULL) inventory = temp->next;
            else prev->next = temp->next;
            free(temp);
            printf("Item '%s' removed from inventory.\n", itemName);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Item '%s' not found.\n", itemName);
}

void traverseInventory() {
    Node* temp = inventory;
    if (temp == NULL) { printf("Inventory is empty.\n"); return; }
    printf("Inventory: ");
    while (temp != NULL) {
        printf("[%s] -> ", temp->item);
        temp = temp->next;
    }
    printf("NULL\n");
}

/* ============================================================
   2. LINEAR SEARCH & BINARY SEARCH  (on leaderboard)
   ============================================================ */
int linearSearch(char* name) {
    int i;
    for (i = 0; i < playerCount; i++)
        if (strcmp(leaderboard[i].name, name) == 0)
            return i;
    return -1;
}

/* leaderboard must be sorted by name for binary search to work */
int binarySearch(char* name) {
    int low = 0, high = playerCount - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        int cmp = strcmp(leaderboard[mid].name, name);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* ============================================================
   3. SORTING - Bubble Sort (leaderboard by score, descending)
   ============================================================ */
void bubbleSortByScore() {
    int i, j;
    Player temp;
    for (i = 0; i < playerCount - 1; i++) {
        for (j = 0; j < playerCount - i - 1; j++) {
            if (leaderboard[j].score < leaderboard[j + 1].score) {
                temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

void sortByNameForBinarySearch() {
    int i, j;
    Player temp;
    for (i = 0; i < playerCount - 1; i++) {
        for (j = 0; j < playerCount - i - 1; j++) {
            if (strcmp(leaderboard[j].name, leaderboard[j + 1].name) > 0) {
                temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

void addPlayer(char* name, int score) {
    if (playerCount < MAX_PLAYERS) {
        strcpy(leaderboard[playerCount].name, name);
        leaderboard[playerCount].score = score;
        playerCount++;
    }
}

void printLeaderboard() {
    int i;
    printf("\n--- Leaderboard ---\n");
    for (i = 0; i < playerCount; i++)
        printf("%d. %s - %d\n", i + 1, leaderboard[i].name, leaderboard[i].score);
}

/* ============================================================
   7. GRAPH TRAVERSAL - BFS (Auto-solve shortest path to treasure)
   ============================================================ */
typedef struct { int r, c; } Point;

void bfsSolve() {
    int visited[SIZE][SIZE] = {0};
    int dist[SIZE][SIZE];
    Point queue[SIZE * SIZE];
    int front = 0, rear = 0;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    int i;

    for (i = 0; i < SIZE * SIZE; i++) { /* init distances */ }
    memset(dist, -1, sizeof(dist));

    queue[rear++] = (Point){playerRow, playerCol};
    visited[playerRow][playerCol] = 1;
    dist[playerRow][playerCol] = 0;

    while (front < rear) {
        Point cur = queue[front++];
        if (cur.r == treasureRow && cur.c == treasureCol) {
            printf("Treasure reachable! Shortest path length = %d steps\n", dist[cur.r][cur.c]);
            return;
        }
        for (i = 0; i < 4; i++) {
            int nr = cur.r + dr[i], nc = cur.c + dc[i];
            if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE &&
                !visited[nr][nc] && board[nr][nc] != '#') {
                visited[nr][nc] = 1;
                dist[nr][nc] = dist[cur.r][cur.c] + 1;
                queue[rear++] = (Point){nr, nc};
            }
        }
    }
    printf("Treasure is unreachable with current walls!\n");
}

/* ============================================================
   8-10. BINARY SEARCH TREE - Player records: insert, search, traverse
   ============================================================ */
TreeNode* insertBST(TreeNode* node, char* name, int score) {
    if (node == NULL) {
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        strcpy(newNode->name, name);
        newNode->score = score;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (score < node->score)
        node->left = insertBST(node->left, name, score);
    else
        node->right = insertBST(node->right, name, score);
    return node;
}

TreeNode* searchBST(TreeNode* node, int score) {
    if (node == NULL || node->score == score) return node;
    if (score < node->score) return searchBST(node->left, score);
    return searchBST(node->right, score);
}

void inorderBST(TreeNode* node) {
    if (node == NULL) return;
    inorderBST(node->left);
    printf("%s (%d)  ", node->name, node->score);
    inorderBST(node->right);
}

/* ============================================================
   MAIN MENU / GAME LOOP
   ============================================================ */
int main() {
    int choice, won = 0;
    char dir, name[20], item[20];
    int r, c, score;

    initBoard();
    printf("=== WELCOME TO TREASURE HUNT ===\n");

    while (1) {
        printf("\n----- MENU -----\n");
        printf("1. Show Board\n");
        printf("2. Move Player (w/a/s/d)\n");
        printf("3. Add Wall\n");
        printf("4. Remove Wall\n");
        printf("5. Add Inventory Item\n");
        printf("6. Remove Inventory Item\n");
        printf("7. Show Inventory\n");
        printf("8. Auto-Solve (BFS Shortest Path)\n");
        printf("9. Add Player Score to Leaderboard & BST\n");
        printf("10. Show Leaderboard (Sorted by Score)\n");
        printf("11. Search Player (Linear Search)\n");
        printf("12. Search Player (Binary Search)\n");
        printf("13. Show BST In-order (Players by Score)\n");
        printf("14. Search Player in BST (by Score)\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printBoard(); break;
            case 2:
                printf("Enter direction (w/a/s/d): ");
                scanf(" %c", &dir);
                won = movePlayer(dir);
                printBoard();
                if (won) printf("You reached the treasure!\n");
                break;
            case 3:
                printf("Enter row and column for wall: ");
                scanf("%d %d", &r, &c);
                insertWall(r, c);
                break;
            case 4:
                printf("Enter row and column to remove wall: ");
                scanf("%d %d", &r, &c);
                deleteWall(r, c);
                break;
            case 5:
                printf("Enter item name: ");
                scanf("%s", item);
                insertItem(item);
                break;
            case 6:
                printf("Enter item name to remove: ");
                scanf("%s", item);
                deleteItem(item);
                break;
            case 7: traverseInventory(); break;
            case 8: bfsSolve(); break;
            case 9:
                printf("Enter player name and score: ");
                scanf("%s %d", name, &score);
                addPlayer(name, score);
                root = insertBST(root, name, score);
                printf("Player added.\n");
                break;
            case 10:
                bubbleSortByScore();
                printLeaderboard();
                break;
            case 11:
                printf("Enter name to search: ");
                scanf("%s", name);
                r = linearSearch(name);
                if (r != -1) printf("Found: %s - %d\n", leaderboard[r].name, leaderboard[r].score);
                else printf("Player not found.\n");
                break;
            case 12:
                sortByNameForBinarySearch();
                printf("Enter name to search: ");
                scanf("%s", name);
                r = binarySearch(name);
                if (r != -1) printf("Found: %s - %d\n", leaderboard[r].name, leaderboard[r].score);
                else printf("Player not found.\n");
                break;
            case 13:
                printf("Players (in-order by score): ");
                inorderBST(root);
                printf("\n");
                break;
            case 14:
                printf("Enter score to search in BST: ");
                scanf("%d", &score);
                {
                    TreeNode* found = searchBST(root, score);
                    if (found) printf("Found player: %s with score %d\n", found->name, found->score);
                    else printf("No player with that score.\n");
                }
                break;
            case 0:
                printf("Thanks for playing!\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
