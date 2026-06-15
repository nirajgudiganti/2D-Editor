#include <stdio.h>
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
    int id;
    int type;
    int active;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

Shape shapes[MAX_OBJECTS];
int shapeCount = 0;
int nextId = 1;

void clearPicture() {
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            picture[i][j] = EMPTY;
        }
    }
}

void displayPicture() {
    int i, j;

    printf("\n");

    for (i = 0; i < WIDTH + 2; i++) {
        printf("-");
    }

    printf("\n");

    for (i = 0; i < HEIGHT; i++) {
        printf("|");

        for (j = 0; j < WIDTH; j++) {
            printf("%c", picture[i][j]);
        }

        printf("|\n");
    }

    for (i = 0; i < WIDTH + 2; i++) {
        printf("-");
    }

    printf("\n");
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        picture[y][x] = PIXEL;
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int error = dx - dy;

    while (1) {
        setPixel(x1, y1);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int doubleError = 2 * error;

        if (doubleError > -dy) {
            error -= dy;
            x1 += sx;
        }

        if (doubleError < dx) {
            error += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1);
    drawLine(x2, y1, x2, y2);
    drawLine(x2, y2, x1, y2);
    drawLine(x1, y2, x1, y1);
}

void drawCircle(int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int decision = 3 - 2 * radius;

    if (radius <= 0) {
        return;
    }

    while (y >= x) {
        setPixel(cx + x, cy + y);
        setPixel(cx - x, cy + y);
        setPixel(cx + x, cy - y);
        setPixel(cx - x, cy - y);

        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx + y, cy - x);
        setPixel(cx - y, cy - x);

        x++;

        if (decision > 0) {
            y--;
            decision = decision + 4 * (x - y) + 10;
        } else {
            decision = decision + 4 * x + 6;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void drawShape(Shape shape) {
    if (shape.type == LINE) {
        drawLine(shape.x1, shape.y1, shape.x2, shape.y2);
    }
    else if (shape.type == RECTANGLE) {
        drawRectangle(shape.x1, shape.y1, shape.x2, shape.y2);
    }
    else if (shape.type == CIRCLE) {
        drawCircle(shape.x1, shape.y1, shape.radius);
    }
    else if (shape.type == TRIANGLE) {
        drawTriangle(
            shape.x1, shape.y1,
            shape.x2, shape.y2,
            shape.x3, shape.y3
        );
    }
}

void redrawPicture() {
    int i;

    clearPicture();

    for (i = 0; i < shapeCount; i++) {
        if (shapes[i].active == 1) {
            drawShape(shapes[i]);
        }
    }
}

void readShapeDetails(Shape *shape, int type) {
    shape->type = type;

    if (type == LINE) {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d",
              &shape->x1, &shape->y1,
              &shape->x2, &shape->y2);
    }
    else if (type == RECTANGLE) {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d",
              &shape->x1, &shape->y1,
              &shape->x2, &shape->y2);
    }
    else if (type == CIRCLE) {
        printf("Enter center x y and radius: ");
        scanf("%d %d %d",
              &shape->x1, &shape->y1,
              &shape->radius);
    }
    else if (type == TRIANGLE) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d",
              &shape->x1, &shape->y1,
              &shape->x2, &shape->y2,
              &shape->x3, &shape->y3);
    }
}

void addShape(int type) {
    Shape shape;

    if (shapeCount >= MAX_OBJECTS) {
        printf("Cannot add more shapes.\n");
        return;
    }

    readShapeDetails(&shape, type);

    shape.id = nextId;
    shape.active = 1;

    shapes[shapeCount] = shape;
    shapeCount++;
    nextId++;

    redrawPicture();

    printf("Shape added. ID = %d\n", shape.id);
}

void listShapes() {
    int i;
    int found = 0;

    printf("\nShape List:\n");

    for (i = 0; i < shapeCount; i++) {
        if (shapes[i].active == 1) {
            found = 1;

            printf("ID: %d | Type: ", shapes[i].id);

            if (shapes[i].type == LINE) {
                printf("Line\n");
            }
            else if (shapes[i].type == RECTANGLE) {
                printf("Rectangle\n");
            }
            else if (shapes[i].type == CIRCLE) {
                printf("Circle\n");
            }
            else if (shapes[i].type == TRIANGLE) {
                printf("Triangle\n");
            }
        }
    }

    if (found == 0) {
        printf("No shapes available.\n");
    }
}

int findShapeIndex(int id) {
    int i;

    for (i = 0; i < shapeCount; i++) {
        if (shapes[i].id == id && shapes[i].active == 1) {
            return i;
        }
    }

    return -1;
}

void deleteShape() {
    int id;
    int index;

    printf("Enter shape ID to delete: ");
    scanf("%d", &id);

    index = findShapeIndex(id);

    if (index == -1) {
        printf("Shape not found.\n");
        return;
    }

    shapes[index].active = 0;
    redrawPicture();

    printf("Shape deleted.\n");
}

void modifyShape() {
    int id;
    int index;
    int type;
    int oldId;

    printf("Enter shape ID to modify: ");
    scanf("%d", &id);

    index = findShapeIndex(id);

    if (index == -1) {
        printf("Shape not found.\n");
        return;
    }

    printf("Choose new shape type:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("Enter type: ");
    scanf("%d", &type);

    if (type < LINE || type > TRIANGLE) {
        printf("Invalid shape type.\n");
        return;
    }

    oldId = shapes[index].id;

    readShapeDetails(&shapes[index], type);

    shapes[index].id = oldId;
    shapes[index].active = 1;

    redrawPicture();

    printf("Shape modified.\n");
}

void showMenu() {
    printf("\nMenu\n");
    printf("1. Draw Line\n");
    printf("2. Draw Rectangle\n");
    printf("3. Draw Circle\n");
    printf("4. Draw Triangle\n");
    printf("5. Display Picture\n");
    printf("6. List Shapes\n");
    printf("7. Delete Shape\n");
    printf("8. Modify Shape\n");
    printf("9. Clear Picture\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main() {
    int choice;

    clearPicture();

    printf("2D Graphics Editor\n");
    printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
    printf("Use coordinates x y.\n");
    printf("x range: 0 to %d\n", WIDTH - 1);
    printf("y range: 0 to %d\n", HEIGHT - 1);

    while (1) {
        showMenu();

        scanf("%d", &choice);

        if (choice == 1) {
            addShape(LINE);
        }
        else if (choice == 2) {
            addShape(RECTANGLE);
        }
        else if (choice == 3) {
            addShape(CIRCLE);
        }
        else if (choice == 4) {
            addShape(TRIANGLE);
        }
        else if (choice == 5) {
            printf("The picture is:\n");
            displayPicture();
        }
        else if (choice == 6) {
            listShapes();
        }
        else if (choice == 7) {
            deleteShape();
        }
        else if (choice == 8) {
            modifyShape();
        }
        else if (choice == 9) {
            shapeCount = 0;
            nextId = 1;
            clearPicture();
            printf("Picture cleared.\n");
        }
        else if (choice == 0) {
            printf("Exiting program.\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}