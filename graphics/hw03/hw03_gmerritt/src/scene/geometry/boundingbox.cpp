#include "boundingbox.h"

void createBbVertexNormals(glm::vec3 (&bb_vert_nor)[BB_VERT_COUNT]);
void createBbVertexPositions(glm::vec3 (&bb_vert_pos)[BB_VERT_COUNT]);
void createBbIndices(GLuint (&bb_idx)[BB_IDX_COUNT]);
BoundingBox::BoundingBox(): max_bounds (0.5f,0.5f,0.5f), min_bounds(-0.5f,-0.5f,-0.5f)
{
}

BoundingBox::BoundingBox(glm::vec3 max, glm::vec3 min)
{
    max_bounds = max;
    min_bounds = min;
}

glm::vec3 BoundingBox::getCenterPoint(glm::vec3 min, glm::vec3 max)
{
        float x_center = (min.x + max.x)/2.0f;
        float y_center = (min.y + max.y)/2.0f;
        float z_center = (min.z + max.z)/2.0f;
        return glm::vec3( x_center,y_center, z_center);
}

void BoundingBox::createBbVertexPositions(glm::vec3 (&bb_vert_pos)[BB_VERT_COUNT])
{
    float xmax = max_bounds.x;
    float xmin = min_bounds.x;
    float ymax = max_bounds.y;
    float ymin = min_bounds.y;
    float zmax = max_bounds.z;
    float zmin = min_bounds.z;

    int idx = 0;
//    //Front face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);

    //Right face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);

    //Left face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);

    //Back face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);

    //Top face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);

    //Bottom face
    //UR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
    //LR
    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
    //LL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);
    //UL
    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);


// Retry but actually thinking about what im doing

//    //Front face
//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);
//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);


//    //Top face
//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);
//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);
//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);

//    //Right face

//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmax);

//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);

//    //Back face

//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymax, zmin);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);
//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);

//    //Left face

//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmin);
//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymax, zmax);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);
//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);

//    //Bottom face
//    //UL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmin);
//    //UR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmin);
//    //LR
//    bb_vert_pos[idx++] = glm::vec3(xmax, ymin, zmax);
//    //LL
//    bb_vert_pos[idx++] = glm::vec3(xmin, ymin, zmax);

}


void createBbVertexNormals(glm::vec3 (&bb_vert_nor)[BB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(0,0,1);
    }
    //Right
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(1,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(-1,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(0,0,-1);
    }
    //Top
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(0,1,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        bb_vert_nor[idx++] = glm::vec3(0,-1,0);
    }
}

void createBbIndices(GLuint (&bb_idx)[BB_IDX_COUNT])
{
    int idx = 0;//01 12 23 30
//    for(int i = 0; i < 6; i++){
//        bb_idx[idx++] = i*4;
//        bb_idx[idx++] = i*4+1;
//        bb_idx[idx++] = i*4+2;
//        bb_idx[idx++] = i*4;
//        bb_idx[idx++] = i*4+2;
//        bb_idx[idx++] = i*4+3;

//    }

    for(int i = 0; i < 6; i++){
        bb_idx[idx++] = i*4;
        bb_idx[idx++] = i*4+1;
        bb_idx[idx++] = i*4 +1;
        bb_idx[idx++] = i*4+2;
        bb_idx[idx++] = i*4+2;
         bb_idx[idx++] = i*4+3;
         bb_idx[idx++] = i*4+3;
         bb_idx[idx++] = i*4;

    }
}

void BoundingBox::create()
{
    GLuint bb_idx[BB_IDX_COUNT];
    glm::vec3 bb_vert_pos[BB_VERT_COUNT];
    glm::vec3 bb_vert_nor[BB_VERT_COUNT];
    glm::vec3 bb_vert_col[BB_VERT_COUNT];

    createBbVertexPositions(bb_vert_pos);
    createBbVertexNormals(bb_vert_nor);
    createBbIndices(bb_idx);

    for(int i = 0; i < BB_VERT_COUNT; i++){
        bb_vert_col[i] = glm::vec3(1.0f,0.0f,1.0f);
    }

    count = BB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(bb_idx, BB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(bb_vert_pos,BB_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(bb_vert_nor, BB_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(bb_vert_col, BB_VERT_COUNT * sizeof(glm::vec3));

}
GLenum BoundingBox::drawMode()
{
    return GL_LINES;
}


