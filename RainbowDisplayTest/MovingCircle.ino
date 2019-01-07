
/**
 * Moves a circle right or left (by increments of the run value) and up or down (by increments of
 * the rise value), bouncing the circle off the wall if it's x or y hit the max defined by width or
 * height.
 *
 * @param c The circle object
 */
void moveCircle(Circle *circle) {
    if (circle->xDir == RIGHT) {
        circle->x = circle->x + circle->run;

        if (circle->x >= circle->width) {

            circle->x = (circle->width - (circle->x - circle->width - 1.0));
            circle->xDir = LEFT;
        }

    } else {
        circle->x = circle->x - circle->run;

        if (circle->x < 0) {

            circle->x = (-1.0 * circle->x) - 1.0;
            circle->xDir = RIGHT;
        }
    }

    if (circle->yDir == DOWN) {
        circle->y = circle->y + circle->rise;

        if (circle->y >= circle->height) {

            circle->y = (circle->height - (circle->y - circle->height - 1.0));
            circle->yDir = UP;
        }

    } else {
        circle->y = circle->y - circle->rise;

        if (circle->y < 0) {

            circle->y = (-1.0 * circle->y) - 1.0;
            circle->yDir = DOWN;
        }
    }
}

/**
 * Returns a value based on how close the provided x/y coordinate is to the center of the circle.
 * @param x
 * @param y
 * @param c
 * @return a value between 0-1.0, representing distance gradient from circle squared
 */
float getCircleVal(int x, int y, Circle *circle) {
    float distance = dist(circle->x, circle->y, x, y);
    if (distance <= circle->radius) {
        return (1.0 - (distance / circle->radius));
        ///*(1.0 - */((1 - (distance / circle->radius)) * (1 - (distance / circle->radius)));//);
    }

    return 0.0;
}
