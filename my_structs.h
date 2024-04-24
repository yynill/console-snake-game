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
} Node;

typedef struct NodeList
{
    Node nodes[MAX_NODES];
    int size;
} NodeList;
