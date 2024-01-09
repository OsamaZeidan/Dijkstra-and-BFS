
/*___________________________________________________includes__________________________________________________*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char *stringToPrintOnFile; // array of results...
/*___________________________________________________structs__________________________________________________*/

typedef struct List
{
    char cityName[15];
    int distance; // distance form previous city to this city.
    struct List *next;
} List;

typedef struct ListOfLists
{
    List *list;
    bool isVisited;
    char name[15];
    char prevCity[15];
    int distance;
    struct ListOfLists *next;
} ListOfLists;

typedef struct Queue
{
    ListOfLists *vertex;
    struct Queue *next;
} Queue;

/*___________________________________________________prototypes__________________________________________________*/
ListOfLists *findVertex(char name[15], ListOfLists *vertices);
void insertVertex(ListOfLists **list, char name[15]);
void insertCity(List **list, char name[15], int distance);
void printCities(List *list);
int numOfVertices(ListOfLists *graph);
void printGraph(ListOfLists *vertices);
List *findCity(char city[15], List *list);
void BFS(ListOfLists *graph, char src[15], char dest[15]);
void enqueue(Queue **queue, ListOfLists *vertex);
void dequeue(Queue **queue);
void dijkstra(ListOfLists *graph, char src[15], char dest[15]);
ListOfLists *fileToGraph();
void printOnFile();
void printMenue();
void disposeGraph(ListOfLists **graph);

/*___________________________________________________main()__________________________________________________*/
int main(void)
{
    stringToPrintOnFile = malloc(sizeof(char) * 1000);
    char src[15], dest[15];
    ListOfLists *vertices;
    // printGraph(vertices);
    printMenue();
    int choice;
    scanf("%d", &choice);
    while (true)
    {
        switch (choice)
        {
        case 1:
            printf("\nThe data Loaded Automatically\n");
            break;
        case 2:
            // load data
            disposeGraph(&vertices);
            vertices = fileToGraph();
            printf("Enter source city: ");
            while ((getchar()) != '\n')
                ;
            scanf("%s", src);
            // check if the city exist
            if (findVertex(src, vertices) == NULL)
            {
                printf("\n\nThe city doesn't exist!\n");
                break;
            } // end if()
            break;
        case 3:
            // load data
            disposeGraph(&vertices);
            vertices = fileToGraph();
            printf("Enter destination city: ");
            while ((getchar()) != '\n')
                ;
            scanf("%s", dest);
            // check if the city exist
            if (findVertex(dest, vertices) == NULL)
            {
                printf("\n\nThe city doesn't exist!\n");
                break;
            } // end if()
            // check if the same as src
            if (strcmp(src, dest) == 0)
            {
                printf("\n\nThe source and destination are the same!\n");
                break;
            } // end if()
            printf("Dijkstra: \n");
            strcat(stringToPrintOnFile, "\nDijkstra: \n\n");
            dijkstra(vertices, src, dest);
            // load data
            disposeGraph(&vertices);
            vertices = fileToGraph();
            printf("\n\nBFS: \n\n");
            strcat(stringToPrintOnFile, "\n\nBFS: \n\n");
            BFS(vertices, src, dest);
            printf("\n\n");
            break;
        case 4:
            printOnFile();
            printf("Good Bye!\n");
            // free strings
            free(stringToPrintOnFile);
            // free graph
            disposeGraph(&vertices);
            return 0;
            break;
        default:
            printf("Invalid Choice!\n");
            break;
        } // end switch()

        printMenue();
        scanf("%d", &choice);
    } // end while()

    return 0;
} // end main()

/*___________________________________________________insertCity()__________________________________________________*/
void insertCity(List **list, char name[15], int distance)
{
    // Insert Last
    List *newNode = malloc(sizeof(List));
    strcpy(newNode->cityName, name);
    newNode->distance = distance;
    newNode->next = NULL;
    if ((*list) == NULL)
    {
        *list = newNode;
        return;
    } // end if()
    else
    {
        List *tmp = *list;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        } // end while()
        tmp->next = newNode;
    } // end else
} // end insertCity()

/*___________________________________________________insertVertex()__________________________________________________*/
void insertVertex(ListOfLists **list, char name[15])
{
    // Insert Last
    ListOfLists *newNode = malloc(sizeof(ListOfLists));
    strcpy(newNode->name, name);
    newNode->isVisited = false;
    newNode->list = NULL;
    newNode->next = NULL;
    newNode->distance = 99999;
    if ((*list) == NULL)
    {
        *list = newNode;

        return;
    } // end if()
    else
    {
        ListOfLists *tmp = *list;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        } // end while()
        tmp->next = newNode;
    } // end else
} // end insertList()

/*___________________________________________________findVertex()__________________________________________________*/
ListOfLists *findVertex(char name[15], ListOfLists *vertices)
{
    ListOfLists *tmp = vertices;
    while (tmp != NULL && strcmp(tmp->name, name) != 0)
    {
        tmp = tmp->next;
    } // end while()
    return tmp;
} // end findVertex()

/*___________________________________________________findCity()__________________________________________________*/
List *findCity(char city[15], List *list)
{
    List *tmp = list;
    while (tmp != NULL && strcmp(tmp->cityName, city) != 0)
    {
        tmp = tmp->next;
    } // end while()
    return tmp;
} // end findCity()

/*___________________________________________________printCities()__________________________________________________*/
void printCities(List *list)
{
    List *tmp = list;
    while (tmp != NULL)
    {
        printf("%s => ", tmp->cityName);
        tmp = tmp->next;
    } // end while()
    printf("\n");
} // end printCities()

/*___________________________________________________printGraph()__________________________________________________*/
void printGraph(ListOfLists *vertices)
{
    ListOfLists *tmp = vertices;
    while (tmp != NULL)
    {
        printf("%s >>> ", tmp->name);
        printCities(tmp->list);
        tmp = tmp->next;
    } // end while()
} // end printGraph()

/*___________________________________________________numOfVertices()__________________________________________________*/
int numOfVertices(ListOfLists *graph)
{
    int count = 0;
    ListOfLists *tmp = graph;
    while (tmp != NULL)
    {
        count++;
        tmp = tmp->next;
    } // end while()
    return count;
} // end numOfVertices()

/*___________________________________________________enqueue()_________________________________________________________*/
void enqueue(Queue **queue, ListOfLists *vertex)
{
    Queue *newNode = malloc(sizeof(Queue));
    newNode->vertex = vertex;
    newNode->next = NULL;
    if ((*queue) == NULL)
    {
        *queue = newNode;
        return;
    } // end if()
    else
    {
        Queue *tmp = *queue;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        } // end while()
        tmp->next = newNode;
    } // end else
} // end enqueue()

/*___________________________________________________dequeue()__________________________________________________*/
void dequeue(Queue **queue)
{
    if ((*queue) == NULL)
    {
        return;
    } // end if()
    else
    {
        Queue *tmp = *queue;
        *queue = (*queue)->next;
        free(tmp);
    } // end else
} // end dequeue()

/*___________________________________________________front()__________________________________________________*/
ListOfLists *front(Queue *queue)
{
    if (queue == NULL)
    {
        return NULL;
    } // end if()
    else
    {
        return queue->vertex;
    } // end else
} // end front()

/*___________________________________________________findMinDistance()__________________________________________________*/
ListOfLists *findMinDistance(ListOfLists *graph)
{
    // find min distance city and not visited
    ListOfLists *tmp = graph;
    ListOfLists *min = tmp;
    while (tmp != NULL)
    {
        if (tmp->distance < min->distance)
        {
            min = tmp;
        } // end if()
        tmp = tmp->next;
    } // end while()
    return min;
} // end of findMinDistance()

/**
 * This Function use the BFS algorithm to find the shortest path between two cities.
 * The Time Complexity of BFS algorithm is O(|V| + |E|), and the same in this function.
 * This function use the queue to determine the visited vertesies.
 */
/*___________________________________________________BFS()__________________________________________________*/
void BFS(ListOfLists *graph, char src[15], char dest[15])
{
    Queue *visited = NULL; // queue for visited vetices
    char *stringToPrintOnFileTmp;
    int totalDistance = 0;
    ListOfLists *srcCity = findVertex(src, graph);
    ListOfLists *destCity = findVertex(dest, graph);
    srcCity->distance = 0;
    enqueue(&visited, srcCity);
    srcCity->isVisited = true;
    while (visited != NULL)
    {
        ListOfLists *tmpSrcCity = front(visited);
        char tmpPrevName[15];
        strcpy(tmpPrevName, tmpSrcCity->name);
        dequeue(&visited);
        if (strcmp(tmpSrcCity->name, dest) == 0)
        {
            strcpy(tmpPrevName, tmpSrcCity->prevCity);
            printf("%s << (%dkm) ", dest, tmpSrcCity->distance);
            strcat(stringToPrintOnFile, dest);
            strcat(stringToPrintOnFile, " << (");
            stringToPrintOnFileTmp = malloc(sizeof(char) * 10);
            sprintf(stringToPrintOnFileTmp, "%d", tmpSrcCity->distance);
            strcat(stringToPrintOnFile, stringToPrintOnFileTmp);
            strcat(stringToPrintOnFile, "km) ");
            free(stringToPrintOnFileTmp);

            totalDistance += tmpSrcCity->distance; // total distance from source to destination city.
            while (strcmp(tmpPrevName, src) != 0)
            {
                ListOfLists *tmp2 = findVertex(tmpPrevName, graph);
                totalDistance += tmp2->distance;
                printf("%s << (%dkm) ", tmpPrevName, tmp2->distance);
                strcat(stringToPrintOnFile, tmpPrevName);
                strcat(stringToPrintOnFile, " << (");
                stringToPrintOnFileTmp = malloc(sizeof(char) * 10);
                sprintf(stringToPrintOnFileTmp, "%d", tmp2->distance);
                strcat(stringToPrintOnFile, stringToPrintOnFileTmp);
                strcat(stringToPrintOnFile, "km) ");
                free(stringToPrintOnFileTmp);
                strcpy(tmpPrevName, tmp2->prevCity);
            } // end while()
            printf("%s << \n\nTotal Distance: %d(km)\n ", src, totalDistance);
            strcat(stringToPrintOnFile, src);
            strcat(stringToPrintOnFile, " << \n\nTotal Distance: ");
            stringToPrintOnFileTmp = malloc(sizeof(char) * 10);
            sprintf(stringToPrintOnFileTmp, "%d", totalDistance);
            strcat(stringToPrintOnFile, stringToPrintOnFileTmp);
            strcat(stringToPrintOnFile, "(km)\n ");
            free(stringToPrintOnFileTmp);
            return;
        } // end if()
        List *tmpList = tmpSrcCity->list;
        while (tmpList != NULL)
        {
            ListOfLists *tmpSrc = findVertex(tmpList->cityName, graph);

            if (tmpSrc->isVisited == true)
            {
                tmpList = tmpList->next;
                continue;
            } // end if()
            tmpSrc->distance = tmpList->distance;
            strcpy(tmpSrc->prevCity, tmpPrevName);
            enqueue(&visited, tmpSrc);
            tmpSrc->isVisited = true;
            tmpList = tmpList->next;

        } // end while()
        if (visited == NULL)
        {
            printf("There is No Path!\n");
            break;
        } // end if()

    } // end while()

} // end printPath()

/**
 * This Function use the dijkstra algorithm to find the shortest path between two cities.
 * The Time Complexity of Dijkstra algorithm is O(|V| + |E|) but in this function the Time Complexity is O(v^2)
 * beacuse this function does not use the MinHeaps...
 */
/*___________________________________________________dijkstra()__________________________________________________*/
void dijkstra(ListOfLists *graph, char src[15], char dest[15])
{
    ListOfLists *tmpSrc = findVertex(src, graph);
    ListOfLists *tmpDest = findVertex(dest, graph);

    char *stringToPrintOnFileTmp;

    int numOfVisitedCities = numOfVertices(graph);
    int prevDistance = 0;
    tmpSrc->distance = 0;
    ListOfLists *tmpSrcCity = tmpSrc;
    List *tmpList = tmpSrc->list;
    int minDistance = 0;
    char minDistCity[15] = " ";
    strcpy(minDistCity, tmpSrc->name);
    while (numOfVisitedCities != 0)
    {
        tmpSrcCity = findVertex(minDistCity, graph);
        numOfVisitedCities--;
        tmpSrcCity->isVisited = true;
        tmpSrc = tmpSrcCity;
        tmpList = tmpSrcCity->list;
        prevDistance = tmpSrcCity->distance;
        minDistance = 99999;

        while (tmpList != NULL)
        {
            tmpSrcCity = findVertex(tmpList->cityName, graph);
            if (tmpSrcCity->distance > prevDistance + tmpList->distance)
            {
                tmpSrcCity->distance = prevDistance + tmpList->distance;
                strcpy(tmpSrcCity->prevCity, tmpSrc->name);
            } // end if()
            tmpList = tmpList->next;
        } // end while()

        // choose min distance city
        tmpSrcCity = graph;
        while (tmpSrcCity != NULL)
        {
            if (tmpSrcCity->isVisited == false && tmpSrcCity->distance < minDistance)
            {
                minDistance = tmpSrcCity->distance;
                strcpy(minDistCity, tmpSrcCity->name);
            } // end if()
            tmpSrcCity = tmpSrcCity->next;
        } // end while()
    }     // end while()

    tmpSrcCity = findVertex(dest, graph);

    printf("\nTotal Distance:  << (%dkm) \n\n", tmpSrcCity->distance);
    strcat(stringToPrintOnFile, "Total Distance: ");
    stringToPrintOnFileTmp = malloc(sizeof(char) * 10);
    sprintf(stringToPrintOnFileTmp, "%d", tmpSrcCity->distance);
    strcat(stringToPrintOnFile, stringToPrintOnFileTmp);
    strcat(stringToPrintOnFile, "(km)\n\n");
    free(stringToPrintOnFileTmp);
    while (strcmp(tmpSrcCity->name, src) != 0)
    {
        printf("%s << ", tmpSrcCity->name);
        strcat(stringToPrintOnFile, tmpSrcCity->name);
        strcat(stringToPrintOnFile, " << ");
        tmpSrcCity = findVertex(tmpSrcCity->prevCity, graph);

    } // end while()
    printf("%s << ", tmpSrcCity->name);
    strcat(stringToPrintOnFile, tmpSrcCity->name);
    strcat(stringToPrintOnFile, " << ");
} // end dijkstra()

/*___________________________________________________fileToGraph()__________________________________________________*/
ListOfLists *fileToGraph()
{
    ListOfLists *vertices = NULL; // Graph of Cities and Distances...
    FILE *in = fopen("cities.txt", "r");
    if (in == NULL)
    {
        printf("File Not Found");
        return NULL;
    } // end if()
    char city1[15], city2[15];
    int distance;
    while (fscanf(in, "%s\t%s\t%dkm\n", city1, city2, &distance) != EOF)
    {
        ListOfLists *tmp = findVertex(city1, vertices);
        if (tmp == NULL)
        {
            insertVertex(&vertices, city1);
            tmp = findVertex(city1, vertices);
        } // end if()
        insertCity(&(tmp->list), city2, distance);
    } // end while()
    fclose(in);
    return vertices;
} // end fileToGraph()

/*___________________________________________________printOnFile()__________________________________________________*/
void printOnFile()
{
    FILE *out = fopen("shortest_distance.txt", "w");
    if (out == NULL)
    {
        printf("File Not Found");
        return;
    } // end if()
    fprintf(out, "%s", stringToPrintOnFile);
    fclose(out);
} // end printOnFile()

void printMenue()
{
    printf("\n1. Load Cities.\n");
    printf("2. Enter Source.\n");
    printf("3. Enter Destination.\n");
    printf("4. Exit.\n");
} // end printMenue()

/*___________________________________________________disposeGraph()__________________________________________________*/
void disposeGraph(ListOfLists **graph)
{
    ListOfLists *tmp = *graph;
    while (tmp != NULL)
    {
        ListOfLists *tmp2 = tmp;
        tmp = tmp->next;
        List *tmpList = tmp2->list;
        while (tmpList != NULL)
        {
            List *tmpList2 = tmpList;
            tmpList = tmpList->next;
            free(tmpList2);
        } // end while()
        free(tmp2);
    } // end while()
} // end disposeGraph()
