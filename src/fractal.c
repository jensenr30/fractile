#include "fractal.h"
#include "graphics.h"
#include "my_math.h"
#include "utilities.h"
#include <math.h>

#define DEGREES_TO_RADIANS (M_PI / 180)

void fractal_render(struct fractal *frac, SDL_Surface *dest, uint32_t iterations) {

    // check for NULL pointers for frac
    if (frac == NULL) {
        error("fractal_render() was sent a NULL frac pointer");
        return;
    }
    // check for NULL pointers for dest
    if (dest == NULL) {
        error("fractal_render() was sent a NULL dest pointer");
        return;
    }
    // limit the possible number of shapes a fractal can have
    if (frac->numberOfShapes > FRACTAL_MAX_SHAPES) {
        error_d(
            "fractal_render() was sent a frac with numberOfShapes > FRACTAL_MAX_SHAPES. This will "
            "be automatically corrected. numberOfShapes was equal to ",
            frac->numberOfShapes
        );
        frac->numberOfShapes = FRACTAL_MAX_SHAPES;
    }
    // limit the possible number of children a fractal can have
    if (frac->numberOfChildren > FRACTAL_MAX_CHILDREN) {
        error_d(
            "fractal_render() was sent a frac with numberOfChildren > FRACTAL_MAX_CHILDREN. This "
            "will be automatically corrected. numberOfChildren was equal to ",
            frac->numberOfChildren
        );
        frac->numberOfChildren = FRACTAL_MAX_CHILDREN;
    }

    // call the first iteration. from here, all of the other iterations will be recursively called.
    fractal_render_iteration(frac, dest, iterations, frac->x, frac->y, frac->zoom, frac->twist);
}

// TODO: fractal_render_iteration() is a top-down approach.  What is needed is a bottom-up approach
// which renders individual pieces of the fractal, then uses those renderings to create a composite
// rendering of the next-higher node in the hierarchy, all the way to the origin.
// RE: TODO: Or, perhaps it would be best to calculate all the nodes in the fractal first, then walk
// through the nodes to determine the most efficient way of rendering the fractal using hardware
// acceleration.

// draw a fractal recursively
void fractal_render_iteration(
    struct fractal *frac,
    SDL_Surface *dest,
    int iteration,
    float x,
    float y,
    float scale,
    float twist
) {
    if (frac == NULL || dest == NULL) {
        return;
    }
    if (iteration == 0) {
        return;
    }
    int i;
    float tx[FRACTAL_MAX_SHAPE_POINTS];
    float ty[FRACTAL_MAX_SHAPE_POINTS];

    for (i = 0; i < frac->numberOfShapes; i++) {
        // todo: extract methods for drawing shapes
        switch (frac->shapes[i].type) {
            case fst_circle:
                twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
                draw_circle(
                    dest,
                    x + tx[0] * scale,
                    y + ty[0] * scale,
                    frac->shapes[i].radius * scale,
                    frac->shapes[i].color
                );
                break;

            case fst_line:
                twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
                twist_xy(frac->shapes[i].x[1], frac->shapes[i].y[1], twist, &tx[1], &ty[1]);
                draw_line(
                    dest,
                    x + tx[0] * scale,
                    y + ty[0] * scale,
                    x + tx[1] * scale,
                    y + ty[1] * scale,
                    frac->shapes[i].radius * scale,
                    frac->shapes[i].color
                );
                break;

            default:
            case fst_pixel:
                twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
                draw_pixel(
                    dest,
                    (x + tx[0] * scale) + 0.5,
                    (y + ty[0] * scale) + 0.5,
                    frac->shapes[i].color
                );
                break;
        }
    }

    for (i = 0; i < frac->numberOfChildren; i++) {
        twist_xy(frac->children[i].x, frac->children[i].y, twist, &tx[0], &ty[0]);
        fractal_render_iteration(
            frac,
            dest,
            iteration - 1,
            x + tx[0] * scale,
            y + ty[0] * scale,
            frac->children[i].scale * scale,
            twist + frac->children[i].twist
        );
    }
}

/// this will take a point <x,y> and rotate it around 0 by twist degrees
// <x,y> is a point relative to <0,0>
// twist is the amount (in degrees) that <x,y> should be twisted
// <x_ret,y_ret> is <x,y> rotated by the angle twist
// todo: refactor using a Point struct (see if SDL has any native Point types)
// todo: see if SDL or other library has accellerated rotation about the origin
void twist_xy(float x, float y, float twist_degrees, float *x_ret, float *y_ret) {
    if (x_ret == NULL || y_ret == NULL) {
        return;
    }

    float mag = sqrt(x * x + y * y);

    float angle_radians;

    if (x != 0) {
        angle_radians = atan(y / x);
        // if x is less that 0, then we need to add this correction factor because of the 180 degree
        // cyclic nature of the atan() function
        if (x < 0) angle_radians -= 3.141592654;
    }
    // If x IS equal to zero, then we cannot use atan(y/x) because we would be dividing by zero.
    else {
        // The angle must either be positive or negative 90 degrees based on the sign on y.
        if (y >= 0) {
            angle_radians = 1.570796;
        } else {
            angle_radians = -1.570796;
        }
    }

    angle_radians += twist_degrees * DEGREES_TO_RADIANS;

    // return the twisted x and y coordinates
    *x_ret = mag * cos(angle_radians);
    *y_ret = mag * sin(angle_radians);
}

/// this function renders the children's placement as simple circles.
// this allows the user to visually see where the fractal's "exit" points are. Where it will copy
// itself
void fractal_render_children(struct fractal *frac, SDL_Surface *dest, int iterations) {

    int c;
    for (c = 0; c < frac->numberOfChildren; c++) {
        draw_circle(
            dest,
            frac->x + frac->children[c].x * frac->zoom,
            frac->y + frac->children[c].y * frac->zoom,
            FRACTAL_CHILDREN_DISPLAY_RADIUS * frac->zoom,
            0xffffffff
        );
    }
}

// todo: refactor into more functions
void fractal_set_default(struct fractal *frac) {
    // check for a NULL frac pointer
    if (frac == NULL) {
        error("fractal_set_default() was sent a NULL [frac] pointer! Aborting function call.");
        return;
    }

    // shape, child, pair.
    int s, c, p;

    // for every shape in a fractal
    for (s = 0; s < FRACTAL_MAX_SHAPES; s++) {
        // set defaults
        frac->shapes[s].color = 0xFFFF00FF;  // magenta by default
        frac->shapes[s].fillPercent = 0.5;   // 50% by default
        frac->shapes[s].fillType = fsf_full; // diagonal fill by default
        frac->shapes[s].radius = 30;         // 30 pixels by default
        frac->shapes[s].type = fst_line;     // default is a line
        // for every xy pair in a shape
        for (p = 0; p < FRACTAL_MAX_SHAPE_POINTS; p++) {
            // set defaults
            frac->shapes[s].x[p] = s * 30 - p * 20;  // some unique value based on s and p
            frac->shapes[s].y[p] = -s * 30 + p * 20; // "
        }
    }
    // for every child in a fractal
    for (c = 0; c < FRACTAL_MAX_CHILDREN; c++) {
        // set defaults
        frac->children[c].x =
            200 *
            cos(3.1415 * 2 * c / ((float)FRACTAL_MAX_CHILDREN)
            ); // distribute the children points initially radially outward from the fractal origin.
        frac->children[c].y = 200 * sin(3.1415 * 2 * c / ((float)FRACTAL_MAX_CHILDREN)); // "
        frac->children[c].scale = FRACTAL_DEFAULT_SCALE;              // default scale
        frac->children[c].twist = FRACTAL_DEFAULT_CHILDREN_TWIST * 4; // default twist for children
    }
    // set defaults for other things
    frac->iterationsChildren =
        FRACTAL_DEFAULT_ITERATIONS_CHILDREN; // default iterations of rendered children points
    frac->numberOfChildren = FRACTAL_DEFAULT_CHILDREN; // default number of children
    frac->numberOfShapes = FRACTAL_DEFAULT_SHAPES;     // default number of shapes
    frac->zoom = FRACTAL_DEFAULT_ZOOM;                 // default zoom scale
    frac->twist = FRACTAL_DEFAULT_TWIST;               // default twist
    // default placement of origin on SDL_Surface rendered to is the center of the screen
    frac->x = get_window_width() / 2.0f;
    frac->y = get_window_height() / 2.0f;
}

void fractal_select_deselect_all(struct fractalSelect *select) {
    if (select == NULL) {
        error("fractal_select_clear() was sent a NULL [select] pointer. Aborting function call.");
        return;
    }

    for (int shape = 0; shape < FRACTAL_MAX_SHAPES; shape++) {
        for (int sp = 0; sp < FRACTAL_MAX_SHAPE_POINTS; sp++) {
            select->shapePoints[shape][sp] = 0; // deselect each points set of xy points
        }
        select->shapeRadius[shape] = 0; // deselect each point's radius
    }

    for (int child = 0; child < FRACTAL_MAX_CHILDREN; child++) {
        // deselect that child
        select->child[child] = 0;
    }
}

/// this function inputs a position that a user has clicked and a fractal and determines if the user
/// selected any of the fractal's parts.
// x and y are in pixel space (screen space). they are converted to fractal space internally in this
// function returns -1 for NULL frac pointer returns 0 when no part of the fractal was within range
// to be considered "selected" returns 1 when there was a part of the fractal that was selected.
int fractal_select_point(SDL_Surface *dest, struct fractal *frac, float x, float y) {
    // check for a NULL frac pointer
    if (frac == NULL) {
        error("fractal_select_clear() was sent a NULL [frac] pointer! Aborting function call.");
        return -1;
    }

    // this keeps track of whether or not a part of the fractal was clicked.
    char clickedPart = 0;

    // calculate the <x,y> position in fractal space
    float xf = (x - frac->x) / frac->zoom;
    float yf = (y - frac->y) / frac->zoom;

    // debugging
    // draw_circle(dest, xf, yf, 10, 0xFFFF3311);

    // this keeps track of how close the input <x,y> pair is to the closest part of the fractal.
    // however, for sake of speed of execution, this is stored as a square value.
    // this number is always delta_x^2 + delta_y^2
    // this number's square root is found after all fractal parts have been checked.
    // closest2 is pronounced "closest squared"
    // set to a really high number initially so that everything else is smaller
    float closest2 = __FLT_MAX__;
    // this is used to store the calculation of distance between <x,y> and the current point being
    // checked dist2, like closest2, is pronounced "distance squared"
    float dist2;

    int s;
    int sp;
    int shapePointsUsed;

    //---------------------------------------------
    // check if the user selected parts of a shape
    //---------------------------------------------
    for (s = 0; s < FRACTAL_MAX_SHAPES; s++) {
        // figure out how many shape points are relevant to the specific shape
        // todo: there must be a better way - use a struct? and enum?
        switch (frac->shapes[s].type) {
            case fst_circle:
                shapePointsUsed = 1;
                break;
            case fst_line:
                shapePointsUsed = 2;
                break;
            case fst_pixel:
                shapePointsUsed = 1;
                break;
            case fst_rect:
                shapePointsUsed = 2;
                break;
            case fst_tri:
                shapePointsUsed = 3;
                break;
            default:
                shapePointsUsed = 1;
                break;
        }

        // todo: this belongs in a different file
        //---------------------------------------------
        // check if the user clicked near any xy points
        //---------------------------------------------
        // for each xy point, check if the user clicked near that point. In this case, "near" mean
        for (sp = 0; sp < shapePointsUsed; sp++) {
            // calculate the distance between where the user clicked and the xy point of this shape
            dist2 = sqr(xf - frac->shapes[s].x[sp]) + sqr(yf - frac->shapes[s].y[sp]);
            // if the current dist2 is less than the so-far closest found dist2 (closest2), we have
            // found a new closest point
            if (dist2 < closest2) {
                // forget all previously selected parts.
                fractal_select_deselect_all(&frac->select);
                // set closest squared to the current calculated distance squared.
                closest2 = dist2;
                // set the selected part to the appropriate child number
                frac->select.shapePoints[s][sp] = 1;
            }
        }

        // todo: this belongs in a different file
        //---------------------------------------------
        // check if the user clicked a radius distance
        // away from the first xy point
        //---------------------------------------------
        // only check for the user selecting the radius if the shape is a circle
        if (frac->shapes[s].type == fst_circle) {
            // calculate the dist2 between <xf,yf> and the first point <x[0],y[0]>
            dist2 = sqr(xf - frac->shapes[s].x[0]) + sqr(yf - frac->shapes[s].y[0]);
            // now calculate the difference between that and the radius to get how close the user is
            // to selecting that radius
            dist2 = dist2 + sqr(frac->shapes[s].radius);
            // if the <xf,yf> point is close to being one radius away from the shape,
            if (dist2 < closest2) {
                // forget all previously selected parts.
                fractal_select_deselect_all(&frac->select);
                // set closest squared to the current calculated distance squared.
                closest2 = dist2;
                // set the selected part to the appropriate child number
                frac->select.shapeRadius[s] = 1;
            }
        }
    }

    //---------------------------------------------
    // check if the user clicked near child points
    //---------------------------------------------

    int c;
    for (c = 0; c < FRACTAL_MAX_CHILDREN; c++) {
        // calculate the distance between where the user clicked and the child point
        dist2 = sqr(xf - frac->children[c].x) + sqr(yf - frac->children[c].y);
        // if the current dist2 is less than the reining champion closest2 value, we have found a
        // new closest point
        if (dist2 < closest2) {
            // forget all previously selected parts.
            fractal_select_deselect_all(&frac->select);
            // set closest squared to the current calculated distance squared.
            closest2 = dist2;
            // set the selected part to the appropriate child number
            frac->select.child[c] = 1;
        }
    }

    //---------------------------------------------
    // check if the closest click was close ENOUGH
    // to select a part of the fractal
    //---------------------------------------------
    // if the click was NOT close enough,
    if (!(closest2 < sqr(FRACTAL_SELECT_PIXEL_DISTANCE))) {
        // then we need to reset the selectedParts struct
        fractal_select_deselect_all(&frac->select);
        clickedPart = 0;
    } else {
        clickedPart = 1;
    }

    return clickedPart;
}

/// this function will input some fractal, and modify it based on a change in x and a change in y
// this function will modify all selected parts of the input fractal SIMULTANEOUSLY.
// this allows this function to modify a fractal that has a GROUP of parts selected at the same time
// (a group modification). this function does not care how many parts are selected. It just fucking
// works. Like a good function should. frac is the fractal to be modified x is the change in the x
// direction (in pixels, the unit of the fractal space) y is the change in the y direction (in
// pixels, the unit of the fractal space) returns 0 on successful modification returns -1 on NULL
// frac pointer
int fractal_select_modify(struct fractal *frac, float x, float y) {
    // check for NULL pointer
    if (frac == NULL) {
        error("fractal_select_modify() was sent a NULL [frac] pointer! Aborting function call.");
        return -1;
    }

    //---------------------------------------------
    // check if shapes need to be modified
    //---------------------------------------------

    int s, sp;
    // for all shapes, modify the selected ones
    for (s = 0; s < FRACTAL_MAX_SHAPES; s++) {
        //---------------------------------------------
        // check if the user wants to modify any xy points
        //---------------------------------------------

        // for each xy point, check if the user clicked near that point. In this case, "near" mean
        for (sp = 0; sp < FRACTAL_MAX_SHAPE_POINTS; sp++) {
            // if the current shape point is selected for modification,
            if (frac->select.shapePoints[s][sp]) {
                frac->shapes[s].x[sp] += x;
                frac->shapes[s].y[sp] += y;
            }
        }

        // if the radius is selected for modification,
        if (frac->select.shapeRadius[s]) {
            // modify the radius
            frac->shapes[s].radius += x; /// TODO: fix this stupid hack. The radius needs to be
                                         /// configurable is a user-friendly way.
        }
    }

    //---------------------------------------------
    // check if children need to be modifies
    //---------------------------------------------

    int c;
    // check all children
    for (c = 0; c < FRACTAL_MAX_CHILDREN; c++) {
        // if this child is selected for modification,
        if (frac->select.child[c]) {
            frac->children[c].x += x;
            frac->children[c].y += y;
        }
    }

    return 0;
}

void fractal_zoom(struct fractal *frac, float zoomFactor, float x, float y) {
    // make sure frac is a valid pointer
    if (frac == NULL) {
        error("fractal_zoom() was sent a NULL frac poitner! Aborting function call.");
        return;
    }

    // adjust the zoom of the fractal
    frac->zoom *= zoomFactor;

    // calculate the deltas between the <x,y> point of the fractal on the screen and the mouse click
    // on the screen
    float dx = frac->x - x;
    float dy = frac->y - y;
    // scale the deltas by the zoomFactor
    dx *= zoomFactor;
    dy *= zoomFactor;
    // modify the position of the fractal on the screen such that the user will feel as though the
    // zoom occurred centered on their mouse location
    frac->x = x + dx;
    frac->y = y + dy;
}
