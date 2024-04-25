#include <stdio.h>
#include <stdlib.h>
#include "my_structs.h"
#include <string.h>

void initializeUndiscoveredList(NodeList *undiscoveredList);
void appendNode(NodeList *list, Node newNode);
void removeNode(NodeList *list, Node node);
int getNodeIndex(NodeList list, Node node);
void moveNode(NodeList *fromList, NodeList *toList, Node node);
Node lowF_Node(NodeList *list);
NodeList generateSuccessor(Node parentNode);
void calculate_node_cost(Node *thisNode);
int calculate_manhattan_distance(Node node1, Node node2);
NodeList backtrackingFrom(Node node);
Node getNodeByPosition(NodeList *list, Position pos);

NodeList undiscoveredList;
NodeList openList;
NodeList closedList;

Node startNode;
Node endNode;

void init_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    undiscoveredList.size = 0;
    openList.size = 0;
    closedList.size = 0;

    strcpy(undiscoveredList.name, "undiscoveredList");
    strcpy(openList.name, "openList");
    strcpy(closedList.name, "closedList");

    initializeUndiscoveredList(&undiscoveredList);

    printf("un: %d\n", undiscoveredList.size);
    printf("op: %d\n", openList.size);
    printf("cl: %d\n", closedList.size);

    startNode = (Node){{headX, headY}, 0, {-1, -1}};
    endNode = (Node){{appleX, appleY}, 0, {-1, -1}};

    moveNode(&undiscoveredList, &openList, startNode);
}

void run_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    int searaching = 1;
    Node goalNode;

    while (searaching && undiscoveredList.size != 0)
    {
        Node qNode = lowF_Node(&openList);
        moveNode(&openList, &closedList, qNode); // <-- bug

        NodeList successors = generateSuccessor(qNode);
        for (int i = 0; i < successors.size; i++)
        {
            // if successor is the goal, stop search
            if (successors.nodes[i].pos.x == endNode.pos.x &&
                successors.nodes[i].pos.y == endNode.pos.y)
            {
                searaching = 0;
                goalNode = successors.nodes[i];
                break;
            }
            calculate_node_cost(&successors.nodes[i]);
            moveNode(&undiscoveredList, &openList, successors.nodes[i]);
        }
    }

    if (!searaching)
    {
        backtrackingFrom(goalNode);
    }
    else
    {
        printf("error executing a*");
    }

    printf("un: %d\n", undiscoveredList.size);
    printf("op: %d\n", openList.size);
    printf("cl: %d\n", closedList.size);

    printf("goal Node: (%d,%d))\n", goalNode.pos.x, goalNode.pos.y);
}

void initializeUndiscoveredList(NodeList *undiscoveredList)
{
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            Node newNode = {{x, y}, 0, {-1, -1}};
            appendNode(undiscoveredList, newNode);
        }
    }
}

void appendNode(NodeList *list, Node newNode)
{
    if (list->size < MAX_NODES)
    {
        list->nodes[(list->size)++] = newNode;
    }
    else
    {
        printf("Error: Cannot append node to %s, list is full.\n", list->name);
    }
}

void removeNode(NodeList *list, Node node)
{
    int nodeIndex = getNodeIndex(*list, node);
    if (nodeIndex != -1)
    {
        for (int i = nodeIndex; i < list->size - 1; i++)
        {
            list->nodes[i] = list->nodes[i + 1];
        }
        list->size--; // Decrement size after removal
    }
    else
    {
        printf("Error: Invalid index specified.\n");
    }
}

void moveNode(NodeList *fromList, NodeList *toList, Node node)
{
    int nodeIndex = getNodeIndex(*fromList, node);

    if (nodeIndex != -1)
    {
        removeNode(fromList, node);
        appendNode(toList, node);
    }
    else
    {
        printf("Node not found in the array %s.\n", fromList->name);
    }
}

int getNodeIndex(NodeList list, Node node)
{
    for (int i = 0; i < list.size; i++)
    {
        if (list.nodes[i].pos.x == node.pos.x &&
            list.nodes[i].pos.y == node.pos.y)
        {
            return i;
        }
    }
    return -1;
}

Node lowF_Node(NodeList *list)
{
    Node lowF = list->nodes[0];
    for (int i = 0; i < list->size; i++)
    {
        if (list->nodes[i].f_cost < lowF.f_cost)
        {
            lowF = list->nodes[i];
        }
    }
    return lowF;
}

NodeList generateSuccessor(Node parentNode)
{
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    NodeList successors;
    successors.size = 0;

    for (int i = 0; i < 4; i++)
    {
        int newX = parentNode.pos.x + directions[i][0];
        int newY = parentNode.pos.y + directions[i][1];

        Node newNode = {{newX, newY}, 0, {parentNode.pos.x, parentNode.pos.x}};

        // Check if the new position is within bounds
        if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
            getNodeIndex(undiscoveredList, newNode) != -1)
        {
            appendNode(&successors, newNode);
        }
    }
    return successors;
}

void calculate_node_cost(Node *thisNode)
{
    int g_cost = calculate_manhattan_distance(*thisNode, startNode);
    int h_cost = calculate_manhattan_distance(*thisNode, endNode);
    int f_cost = g_cost + h_cost;

    // directly assign f cost
    thisNode->f_cost = f_cost;
}

int calculate_manhattan_distance(Node node1, Node node2)
{
    return abs(node1.pos.x - node2.pos.x) + abs(node1.pos.y - node2.pos.y);
}

NodeList backtrackingFrom(Node node)
{
    NodeList path;

    if (node.pos.x == startNode.pos.x &&
        node.pos.y == startNode.pos.y)
    {
        return path;
    }

    while (1) // Infinite loop until the start node is reached
    {
        if (node.pos.x == startNode.pos.x &&
            node.pos.y == startNode.pos.y)
        {
            // Return the path when the start node is reached
            return path;
        }

        appendNode(&path, node);

        // Find the parent node in the closed list
        node = getNodeByPosition(&closedList, node.parent);

        // Check if the parent node is valid
        if (node.pos.x == -1 && node.pos.y == -1)
        {
            // Return an empty path if the parent node is not found
            return path;
        }
    }
}

Node getNodeByPosition(NodeList *list, Position pos)
{
    Node notFoundNode;
    notFoundNode.pos.x = -1;
    notFoundNode.pos.y = -1;

    for (size_t i = 0; i < list->size; i++)
    {
        if (list->nodes[i].pos.x == pos.x &&
            list->nodes[i].pos.y == pos.y)
        {
            return list->nodes[i];
        }
    }
    return notFoundNode;
}

// visualize paths with "."