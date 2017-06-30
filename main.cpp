#include "main.h"


int main(int argc, char **argv) {
    printf("AE hooo\n");

    Image* image = readImage("obj8__325.pgm");
    Point2DList* intersections = getSuperpixelIntersections(image);
    printf("MANOOOOO\n");

    printf("%d\n", intersections->size);

    destroyPoint2DList(&intersections);
    destroyImage(&image);
    return 0;
}
