#include <stdio.h>
#include "my_structs.h"

void initializeUndiscoveredList(NodeList *undiscoveredList);
void appendNode(NodeList *list, Node newNode);
void removeNode(NodeList *list, Node node);
int getNodeIndex(NodeList list, Node node);
void moveNode(NodeList *fromList, NodeList *toList, Node node);

NodeList undiscoveredList;
NodeList openList;
NodeList closedList;

void init_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    undiscoveredList.size = 0;
    openList.size = 0;
    closedList.size = 0;
    initializeUndiscoveredList(&undiscoveredList);

    printf("un: %d\n", undiscoveredList.size);
    printf("op: %d\n", openList.size);
    printf("cl: %d\n", closedList.size);
}

void run_aStar(int headX, int headY, int appleX, int appleY, int *x_vel, int *y_vel)
{
    Node node = {{0, 0}, -1, {-1, -1}};
    moveNode(&undiscoveredList, &openList, node);

    printf("un: %d\n", undiscoveredList.size);
    printf("op: %d\n", openList.size);
    printf("cl: %d\n", closedList.size);
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
        printf("Error: Cannot append node, list is full.\n");
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
        printf("Node not found in the array.\n");
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