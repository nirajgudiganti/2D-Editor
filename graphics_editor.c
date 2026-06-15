include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 24
#define MAX_OBJECTS 50

#define EMPTY '_'
#define PIXEL '*'

#define LINE 1
#define RECTANGLE 2
#define CIRCLE 3
#define TRIANGLE 4

char picture[HEIGHT][WIDTH];

typedef struct {
    int type;
    int active;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

Shape objects[MAX_OBJECTS];
int objectCount = 0;

void clearPicture()
{
    int i, j;

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            picture[i][j] = EMPTY;
        }
    }
}

void setPixel(int x, int y)
{
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        picture[y][x] = PIXEL;
    }
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        setPixel(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    drawLine(x1, y1, x2, y1);
    drawLine(x2, y1, x2, y2);
    drawLine(x2, y2, x1, y2);
    drawLine(x1, y2, x1, y1);
}

void drawCircle(int cx, int cy, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(y >= x)
    {
        setPixel(cx + x, cy + y);
        setPixel(cx - x, cy + y);
        setPixel(cx + x, cy - y);
        setPixel(cx - x, cy - y);

        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx + y, cy - x);
        setPixel(cx - y, cy - x);

        x++;

        if(d > 0)
        {
            y--;
            d += 4 * (x - y) + 10;
        }
        else
        {
            d += 4 * x + 6;
        }
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void redrawPicture()
{
    int i;

    clearPicture();

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].active)
        {
            Shape s = objects[i];

            if(s.type == LINE)
                drawLine(s.x1, s.y1, s.x2, s.y2);

            else if(s.type == RECTANGLE)
                drawRectangle(s.x1, s.y1, s.x2, s.y2);

            else if(s.type == CIRCLE)
                drawCircle(s.x1, s.y1, s.radius);

            else if(s.type == TRIANGLE)
                drawTriangle(
                    s.x1, s.y1,
                    s.x2, s.y2,
                    s.x3, s.y3
                );
        }
    }
}

void displayPicture()
{
    int i, j;

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            printf("%c", picture[i][j]);
        }
        printf("\n");
    }
}

void addObject()
{
    int type;
    Shape s;

    if(objectCount >= MAX_OBJECTS)
        return;

    printf("Choose shape type:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("Enter shape type: ");
    scanf("%d", &type);

    s.type = type;
    s.active = 1;

    if(type == LINE)
    {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d%d%d%d",
              &s.x1,&s.y1,
              &s.x2,&s.y2);
    }
    else if(type == RECTANGLE)
    {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d%d%d%d",
              &s.x1,&s.y1,
              &s.x2,&s.y2);
    }
    else if(type == CIRCLE)
    {
        printf("Enter center x y and radius: ");
        scanf("%d%d%d",
              &s.x1,&s.y1,
              &s.radius);
    }
    else if(type == TRIANGLE)
    {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d%d%d%d%d%d",
              &s.x1,&s.y1,
              &s.x2,&s.y2,
              &s.x3,&s.y3);
    }

    objects[objectCount] = s;

    printf("Object added with index %d.\n\n",
           objectCount);

    objectCount++;

    redrawPicture();
}

void listObjects()
{
    int i;

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].active)
        {
            printf("Object %d\n", i);
        }
    }
}

void deleteObject()
{
    int index;

    printf("Enter object index: ");
    scanf("%d", &index);

    if(index >= 0 &&
       index < objectCount &&
       objects[index].active)
    {
        objects[index].active = 0;
        redrawPicture();
    }
}

void modifyObject()
{
    int index;
    int type;

    printf("Enter object index: ");
    scanf("%d", &index);

    if(index < 0 ||
       index >= objectCount ||
       !objects[index].active)
    {
        return;
    }

    printf("Choose shape type:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("Enter shape type: ");
    scanf("%d", &type);

    objects[index].type = type;

    if(type == LINE)
    {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d%d%d%d",
              &objects[index].x1,
              &objects[index].y1,
              &objects[index].x2,
              &objects[index].y2);
    }
    else if(type == RECTANGLE)
    {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d%d%d%d",
              &objects[index].x1,
              &objects[index].y1,
              &objects[index].x2,
              &objects[index].y2);
    }
    else if(type == CIRCLE)
    {
        printf("Enter center x y and radius: ");
        scanf("%d%d%d",
              &objects[index].x1,
              &objects[index].y1,
              &objects[index].radius);
    }
    else if(type == TRIANGLE)
    {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d%d%d%d%d%d",
              &objects[index].x1,
              &objects[index].y1,
              &objects[index].x2,
              &objects[index].y2,
              &objects[index].x3,
              &objects[index].y3);
    }

    redrawPicture();
}

void showMenu()
{
    printf("2D Graphics Editor\n");
    printf("Canvas size: 80 x 24\n");
    printf("1. Add object\n");
    printf("2. Delete object\n");
    printf("3. Modify object\n");
    printf("4. Display picture\n");
    printf("5. List objects\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main()
{
    int choice;

    clearPicture();

    while(1)
    {
        showMenu();

        scanf("%d", &choice);

        if(choice == 1)
            addObject();

        else if(choice == 2)
            deleteObject();

        else if(choice == 3)
            modifyObject();

        else if(choice == 4)
        {
            displayPicture();
            printf("\n");
        }

        else if(choice == 5)
            listObjects();

        else if(choice == 0)
        {
            printf("Goodbye.\n");
            break;
        }
    }

    return 0;
}
