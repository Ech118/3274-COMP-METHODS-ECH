#include "Box.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h>


Box::Box(float half) : Entity(), half(half)
{
    SoDrawStyle* style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;   // wireframe so the ball stays visible inside

    SoCube* cube = new SoCube;           // SoCube sizes are full widths
    cube->width  = 2 * half;
    cube->height = 2 * half;
    cube->depth  = 2 * half;

    root->addChild(style);
    root->addChild(cube);
}
