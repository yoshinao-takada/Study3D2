#ifndef PMATRIXUTDATA_H_
#define PMATRIXUTDATA_H_
#define AXIS_AND_POINT0 { /* refline is along x-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { 1.0f, 0.0f, 0.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT1 { /* refline is along y-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { 0.0f, 1.0f, 0.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 1.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT2 { /* refline is along z-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { 0.0f, 0.0f, 1.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 1.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT3 { /* refline is along negative x-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { -1.0f, 0.0f, 0.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 0.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT4 { /* refline is along negative y-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { 0.0f, -1.0f, 0.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 1.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT5 { /* refline is along negative z-axis */ \
    { 1.0f, 1.0f, 0.0f, 1.0f }, /* P0 */ \
    { 0.0f, 0.0f, -1.0f, 2.0f}, /* dir */ \
    { -1.5f, 1.0f, 1.0f, 1.0f } /* P1 */ \
}

#define AXIS_AND_POINT6 { /* refline is along (1,1,1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { 1.0f, 1.0f, 1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT7 { /* refline is along (-1,1,1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { -1.0f, 1.0f, 1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT8 { /* refline is along (1,-1,1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { 1.0f, -1.0f, 1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT9 { /* refline is along (-1,-1,1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { -1.0f, -1.0f, 1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT10 { /* refline is along (1,1,-1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { 1.0f, 1.0f, -1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT11 { /* refline is along (-1,1,-1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { -1.0f, 1.0f, -1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT12 { /* refline is along (1,-1,-1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { 1.0f, -1.0f, -1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}
#define AXIS_AND_POINT13 { /* refline is along (-1,-1,-1) */ \
    { -1.0f, -1.5f, 0.0f, 0.2f }, /* P0 */ \
    { -1.0f, -1.0f, -1.0f, 1.0f }, /* dir */ \
    { 0.5f, -1.5f, -2.0f, 1.0f } /* P1 */ \
}

#define AXES_AND_POINTS { \
    AXIS_AND_POINT0, AXIS_AND_POINT1, AXIS_AND_POINT2, AXIS_AND_POINT3, AXIS_AND_POINT4, AXIS_AND_POINT5, \
    AXIS_AND_POINT6, AXIS_AND_POINT7, AXIS_AND_POINT8, AXIS_AND_POINT9, \
    AXIS_AND_POINT10, AXIS_AND_POINT11, AXIS_AND_POINT12, AXIS_AND_POINT13 }
#define AXES_AND_POINTS_COUNT   14
#endif /* PMATRIXUTDATA_H_ */