#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <openGL/drawable.h>

static const int BB_IDX_COUNT = 48;
static const int BB_VERT_COUNT = 24;

class BoundingBox: public Drawable
{
public:
    BoundingBox();
    BoundingBox(glm::vec3 max, glm::vec3 min);

    void create();
    GLenum drawMode();
    glm::vec3 center;

    glm::vec3 max_bounds;
    glm::vec3 min_bounds; // By default, these are stored in world space coordinates.
                                        //It should also have a way to get its centerpoint (the average of its bounds).
    glm::vec3 getCenterPoint( glm::vec3 min, glm::vec3 max);
private:
    glm::vec3 center_point;
    void createBbVertexPositions(glm::vec3 (&bb_vert_pos)[BB_VERT_COUNT]);

};

#endif // BOUNDINGBOX_H
