#define COLS 60
#define ROWS 30
#define MAX_NODES (COLS * ROWS)

typedef struct Position
{
    int x;
    int y;
} Position;

typedef struct Node
{
    Position pos;
    int f_cost;
    Position parent;
    int isObstacle;
} Node;

typedef struct NodeList
{
    char name[100];
    Node nodes[MAX_NODES];
    int size;
} NodeList;
