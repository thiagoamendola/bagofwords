#include "main.h"


int main(int argc, char **argv) {
    printf("AE hooo\n");

    Image* image = readImage("obj8__325.pgm");
    Point2DList* intersections = getSuperpixelIntersections(image);

    printf("RESULTS %d/%d\n", intersections->size, image->nx * image->ny);

    destroyPoint2DList(&intersections);
    destroyImage(&image);
    return 0;
}
