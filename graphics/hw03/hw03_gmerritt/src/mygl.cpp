#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <tbb/tbb.h>
#include <QDebug>
#include <QTextStream>
#include <QtAlgorithms>



using namespace tbb;
bool txMinLessThan(const Triangle* g1, const Triangle* g2);
bool tyMinLessThan(const Triangle* g1, const Triangle* g2);
bool tzMinLessThan(const Triangle* g1, const Triangle* g2);

bool zLessThan(const Geometry* g1, const Geometry* g2);
bool yLessThan(const Geometry* g1, const Geometry* g2);
bool xLessThan(const Geometry* g1, const Geometry* g2);
bool zMinLessThan(const Geometry* g1, const Geometry* g2);
bool zMaxLessThan(const Geometry* g1, const Geometry* g2);
bool yMinLessThan(const Geometry* g1, const Geometry* g2);
bool yMaxLessThan(const Geometry* g1, const Geometry* g2);
bool xMinLessThan(const Geometry* g1, const Geometry* g2);
bool xMaxLessThan(const Geometry* g1, const Geometry* g2);



MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent)
{
    setFocusPolicy(Qt::ClickFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    vao.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat-color shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    //Test scene data initialization
    scene.CreateTestScene();
    integrator.scene = &scene;
    integrator.intersection_engine = &intersection_engine;
    intersection_engine.scene = &scene;
    ResizeToSceneCamera();
}

void MyGL::resizeGL(int w, int h)
{
    gl_camera = Camera(w, h);

    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());
    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    GLDrawScene();

}

void MyGL::GLDrawScene()
{
    for(Geometry *g : scene.objects)
    {
        if(g->drawMode() == GL_TRIANGLES)
        {
            prog_lambert.setModelMatrix(g->transform.T());
            prog_lambert.draw(*this, *g);
        }
        else if(g->drawMode() == GL_LINES)
        {
            prog_flat.setModelMatrix(g->transform.T());
            prog_flat.draw(*this, *g);
        }
//        if(g->bb != NULL)
//        {
//            prog_flat.draw(*this, *(g->bb));
//        }

    }
    for(Geometry *l : scene.lights)
    {
        prog_flat.setModelMatrix(l->transform.T());
        prog_flat.draw(*this, *l);
    }
//    glm::vec3 testmax = glm::vec3 ( 10, 10, 10);
//    glm::vec3 testmin = glm::vec3 (-5,-5, -5);
//    BVHNode* root = new BVHNode(testmax, testmin);
//    root = createBVHTree(root,scene.objects,0);
//    BVHNode* test1 = createBVHTree(NULL, scene.objects,0);
//    prog_flat.draw(*this, *(test1->bb));

    prog_flat.setModelMatrix(glm::mat4(1.0f));
    prog_flat.draw(*this, scene.camera);
    if(intersection_engine.rootNode != NULL){
        DrawBb(intersection_engine.rootNode);
    }
}

void MyGL::ResizeToSceneCamera()
{
    this->setFixedWidth(scene.camera.width);
    this->setFixedHeight(scene.camera.height);
//    if(scene.camera.aspect <= 618/432)
//    {
//        this->setFixedWidth(432 / scene.camera.aspect);
//        this->setFixedHeight(432);
//    }
//    else
//    {
//        this->setFixedWidth(618);
//        this->setFixedHeight(618 * scene.camera.aspect);
//    }
    gl_camera = Camera(scene.camera);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_F) {
        gl_camera.CopyAttributes(scene.camera);
    } else if (e->key() == Qt::Key_R) {
        scene.camera = Camera(gl_camera);
        scene.camera.recreate();
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::SceneLoadDialog()
{
    QString filepath = QFileDialog::getOpenFileName(0, QString("Load Scene"), QString("../scene_files"), tr("*.xml"));
    if(filepath.length() == 0)
    {
        return;
    }

    QFile file(filepath);
    int i = filepath.length() - 1;
    while(QString::compare(filepath.at(i), QChar('/')) != 0)
    {
        i--;
    }
    QStringRef local_path = filepath.leftRef(i+1);
     if(intersection_engine.rootNode != NULL)
     {
         deleteBVHTree(intersection_engine.rootNode);
     }
    //Reset all of our objects
    scene.Clear();
    integrator = Integrator();
    intersection_engine = IntersectionEngine();
    //Make BVH tree
//    glm::vec3 testmax = glm::vec3 ( 10, 10, 10);
//    glm::vec3 testmin = glm::vec3 (-5,-5, -5);
//    BVHNode* bvh_root = new BVHNode(testmax, testmin);

//    intersection_engine.rootNode = MyGL::createBVHTree(bvh_root, scene.objects, 0);

    //Load new objects based on the XML file chosen.
    xml_reader.LoadSceneFromFile(file, local_path, scene, integrator);
    integrator.scene = &scene;
    integrator.intersection_engine = &intersection_engine;
    intersection_engine.scene = &scene;
    intersection_engine.rootNode = this->createBVHTree(NULL, scene.objects, 0);
//    deleteBVHTree(intersection_engine.rootNode);


}

// ~~~~~~~~~~~~~ Sorting algo  ~~~~~~~~~~~//
bool zLessThan(const Geometry* g1, const Geometry* g2)
{
    return  g1->bb->center[2]  < g2->bb->center[2];

}

bool yLessThan (const Geometry* g1, const Geometry* g2)
{

        return g1->bb->center[1]  < g2->bb->center[1];
}

bool xLessThan(const Geometry* g1, const Geometry* g2)
{
    return  g1->bb->center[0]  <  g2->bb->center[0];

}
bool xMinLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->min_bounds[0] < g2->bb->min_bounds[0];
}
bool xMaxLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->max_bounds[0] < g2->bb->max_bounds[0];
}
bool yMinLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->min_bounds[1] < g2->bb->min_bounds[1];
}
bool yMaxLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->max_bounds[1] < g2->bb->max_bounds[1];
}
bool zMinLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->min_bounds[2] < g2->bb->min_bounds[2];
}
bool zMaxLessThan(const Geometry* g1, const Geometry* g2)
{
    return g1->bb->max_bounds[2] < g2->bb->max_bounds[2];
}

bool tyMinLessThan(const Triangle* g1, const Triangle* g2)
{
    return g1->bb->min_bounds[1] < g2->bb->min_bounds[1];
}


bool txMinLessThan(const Triangle* g1, const Triangle* g2)
{
    return g1->bb->min_bounds[0] < g2->bb->min_bounds[0];
}

bool tzMinLessThan(const Triangle* g1, const Triangle* g2)
{
    return g1->bb->min_bounds[2] < g2->bb->min_bounds[2];
}

// ~~~~~~~~~~mesh Tree ~~~~~~~~~~~~~~~//
BVHNode* MyGL::createBVHTree(BVHNode* root, QList<Triangle*> &tri, int depth)
{

    if(tri.length() <= 1)
    {
        if(tri.isEmpty())
        {
            return root;
        }
        root->geo = tri.last();
        root->bb = tri.last()->bb;
        root->geo->setBbox();
        root->bb->create();
        return root;
    }

    QList<Triangle*>xbox_geom = tri;
    QList<Triangle*>ybox_geom = tri;
    QList<Triangle*>zbox_geom  = tri;

    qSort(xbox_geom.begin(), xbox_geom.end(), txMinLessThan);
    qSort(ybox_geom.begin(), ybox_geom.end(), tyMinLessThan);
    qSort(zbox_geom.begin(), zbox_geom.end(), tzMinLessThan);





  qDebug() << "stabley sorted";
//   Geometry* min_g = zbox_geom.first();
//   Geometry* max_g = zbox_geom.last();
    glm::vec3 min =glm::vec3( xbox_geom.first()->bb->min_bounds.x,
                                               ybox_geom.first()->bb->min_bounds.y,
                                               zbox_geom.first()->bb->min_bounds.z);

    glm::vec3 max = glm::vec3(xbox_geom.last()->bb->max_bounds.x,
                                                ybox_geom.last()->bb->max_bounds.y,
                                                zbox_geom.last()->bb->max_bounds.z);



    if(root == NULL) // DONT DO THIS
    {
       root = new BVHNode();
       root->bb = new BoundingBox(max,min);
       root->bb->center = root->bb->getCenterPoint(min,max);
    }
    BVHNode* nodeL = new BVHNode(max,min);
    nodeL->bb = NULL;
    BVHNode* nodeR = new BVHNode(max,min);
    nodeR->bb = NULL;

    root->bb  = new BoundingBox(max, min);
    root->bb->center = root->bb->getCenterPoint(min,max);
    root->bb->create();



    if(depth%3 == 0)
    {
        tri = xbox_geom;
     }
    else if(depth%3 == 1)
    {
       tri =ybox_geom;
    }

    else if (depth%3 == 2)
    {
        tri = zbox_geom;
    }

       int idx = tri.size()/2.0f;

    QList<Triangle*> left_geom = tri.mid(0, idx);
    QList<Triangle*> right_geom = tri.mid(idx, idx+1);



    root->leftChild = createBVHTree(nodeL, left_geom, depth+1);
    root->rightChild =  createBVHTree(nodeR, right_geom, depth+1);

    return root;


}

// ~~~~~~~~~ creating Tree  ~~~~~~~~~~~~~~~~//
BVHNode* MyGL::createBVHTree(BVHNode* root, QList<Geometry*> &box_geom, int depth)
{

    qDebug() << "Creating Tree";

    if (box_geom.length() <= 1)
    {
        if(box_geom.isEmpty())
        {
            return root;
        }

        Mesh* check_mesh = dynamic_cast<Mesh*>(box_geom.first());
        if(check_mesh != NULL)
        {
            QList<Triangle *> trys = check_mesh->getFaces();
            return createBVHTree( root,trys, 0);
        }
        root->geo = box_geom.last();
        root->bb = box_geom.last()->bb;
        root->bb->create();
        QString qname;
        for(int i = 0; i < box_geom.size(); i++)
        {
            qname.append(box_geom[i]->name);
            qname.append(QString(", "));
        }
        root->name = qname;
        qDebug() << "Leaf" << box_geom.last()->name;
        return root;
    }
//    if(root == NULL) // DONT DO THIS
//    {

//    }

    QList<Geometry*>xbox_geom = box_geom;
    QList<Geometry*>ybox_geom = box_geom;
    QList<Geometry*>zbox_geom  = box_geom;

//    qDebug() << "list size" << box_geom.size();
    qSort(xbox_geom.begin(), xbox_geom.end(), xLessThan);
    qSort(ybox_geom.begin(), ybox_geom.end(), yLessThan);
    qSort(zbox_geom.begin(), zbox_geom.end(), zLessThan);

    QList<Geometry*>xmax_geom = box_geom;
    QList<Geometry*>xmin_geom = box_geom;
    qSort(xmax_geom.begin(), xmax_geom.end(), xMaxLessThan);
    qSort(xmin_geom.begin(), xmin_geom.end(), xMinLessThan);

    QList<Geometry*>ymax_geom = box_geom;
    QList<Geometry*>ymin_geom = box_geom;
    qSort(ymax_geom.begin(), ymax_geom.end(), yMaxLessThan);
    qSort(ymin_geom.begin(), ymin_geom.end(), yMinLessThan);

    QList<Geometry*>zmax_geom = box_geom;
    QList<Geometry*>zmin_geom = box_geom;
    qSort(zmax_geom.begin(), zmax_geom.end(), zMaxLessThan);
    qSort(zmin_geom.begin(), zmin_geom.end(), zMinLessThan);



  qDebug() << "stabley sorted";
//   Geometry* min_g = zbox_geom.first();
//   Geometry* max_g = zbox_geom.last();
    glm::vec3 min =glm::vec3( xmin_geom.first()->bb->min_bounds.x,
                                               ymin_geom.first()->bb->min_bounds.y,
                                               zmin_geom.first()->bb->min_bounds.z);

    glm::vec3 max = glm::vec3(xmax_geom.last()->bb->max_bounds.x,
                                                ymax_geom.last()->bb->max_bounds.y,
                                                zmax_geom.last()->bb->max_bounds.z);

    Geometry* min_g = zmin_geom.first();
    Geometry* max_g = zmax_geom.last();



    if(root == NULL) // DONT DO THIS
    {
       root = new BVHNode();
       root->bb = new BoundingBox(max,min);
       root->bb->center = root->bb->getCenterPoint(min,max);
    }
    BVHNode* nodeL = new BVHNode(max,min);
    nodeL->bb = NULL;
    BVHNode* nodeR = new BVHNode(max,min);
    nodeR->bb = NULL;

    root->bb  = new BoundingBox(max, min);
    root->bb->center = root->bb->getCenterPoint(min,max);
    root->bb->create();

    QString qname;
    for(int i = 0; i < box_geom.size(); i++)
    {
        qname.append(box_geom[i]->name);
        qname.append(QString(", "));
    }
    root->name = qname;

    if(depth%3 == 0)
    {
        box_geom = xbox_geom;
     }
    else if(depth%3 == 1)
    {
       box_geom =ybox_geom;
    }

    else if (depth%3 == 2)
    {
        box_geom = zbox_geom;
    }
    else
    {
      qDebug() << "I've Made a terrible mistake";
    }
       int idx = box_geom.size()/2.0f;
       qDebug() << "splitting the list" << idx;

    QList<Geometry*> left_geom = box_geom.mid(0, idx);
    QList<Geometry*> right_geom = box_geom.mid(idx, idx+1);
  qDebug() <<" Right geom size:"<< right_geom.size();
  qDebug() <<"Left geom size: " << left_geom.size();


    root->leftChild = createBVHTree(nodeL, left_geom, depth+1);
    root->rightChild =  createBVHTree(nodeR, right_geom, depth+1);

    return root;
}

// ~~~~~~~~~deleting Tree ~~~~~~~~~~~~~~~~//
void MyGL::deleteBVHTree(BVHNode* root)
{
    if(root->leftChild !=NULL)
    {
        deleteBVHTree(root->leftChild);
    }
    if(root->rightChild !=NULL)
    {
        deleteBVHTree(root->rightChild);
    }
   if( root != NULL)
   {
       qDebug() << "Deleting leaf";
       root = NULL;
       delete root;
   }
}
void MyGL::DrawBb(BVHNode* root_node)
{
//    qDebug() << "I am rendering";
    if( (root_node == NULL))
    {
        return;
    }
    prog_flat.draw(*this, *(root_node->bb));

    DrawBb(root_node->leftChild);
    DrawBb(root_node->rightChild);
//    prog_flat.draw(*this, *(root_node->bb));
//   DrawBb(root_node->leftChild);
//   DrawBb(root_node->rightChild);

}

void MyGL::RaytraceScene()
{
    QString filepath = QFileDialog::getSaveFileName(0, QString("Save Image"), QString("../rendered_images"), tr("*.bmp"));
    if(filepath.length() == 0)
    {
        return;
    }
    //#define TBB //Uncomment this line out to render your scene with multiple threads.
    //This is useful when debugging your raytracer with breakpoints.
    #ifdef TBB
        parallel_for(0, (int)scene.camera.width, 1, [=](unsigned int i)
        {
            for(unsigned int j = 0; j < scene.camera.height; j++)
            {
                //TODO
            }
        });
    #else
        for(unsigned int i = 0; i < scene.camera.width; i++)
        {
            for(unsigned int j = 0; j < scene.camera.height; j++)
            {
                //cast a ray through every pixel
                Ray r = scene.camera.Raycast(i,j);
                Intersection intersect = intersection_engine.GetIntersection(r);
//                intersection_engine.rootNode = new BVHNode();
//                MyGL::createBVHTree(intersection_engine.rootNode, scene.objects, 0);

                // get color from object hit
//               scene.film.pixels[i][j] = intersect.normal;
                  if(i == 0 && j ==0)
                  {
                        int test = 10 ;
                        test ++;
                  }

                   scene.film.pixels[i][j]= integrator.TraceRay(r,5);
            }

        }
    #endif
    scene.film.WriteImage(filepath);
}
