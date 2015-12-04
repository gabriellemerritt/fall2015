#include <scene/geometry/mesh.h>
#include <la.h>
#include <tinyobj/tiny_obj_loader.h>
#include <iostream>
// I added this function
float calcTriArea (const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
bool withinBounds(const float S);

bool withinBounds(const float S)
{
    if(fequal(S,0.0f))
    {
        return true;
    }
    else if (fequal(S,1.0f))
    {
        return true;
    }
    if( S <1.0f && S > 0.0f)
    {
        return true;
    }
    return false;

}

float Area(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
{
    return glm::length(glm::cross(p1 - p2, p3 - p2)) * 0.5f;
}

//Returns the interpolation of the triangle's three normals based on the point inside the triangle that is given.
glm::vec3 Triangle::GetNormal(const glm::vec3 &P)
{
    float A = Area(points[0], points[1], points[2]);
    float A0 = Area(points[1], points[2], P);
    float A1 = Area(points[0], points[2], P);
    float A2 = Area(points[0], points[1], P);
    return glm::normalize(normals[0] * A0/A + normals[1] * A1/A + normals[2] * A2/A);
}

float calcTriArea (const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
{
    return  glm::length(glm::cross((p2 - p1), (p3 -p1)))/ 2.0f;
}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3):
    Triangle(p1, p2, p3, glm::vec3(1), glm::vec3(1), glm::vec3(1), glm::vec2(0), glm::vec2(0), glm::vec2(0))
{
    for(int i = 0; i < 3; i++)
    {
        normals[i] = plane_normal;
    }
}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3):
    Triangle(p1, p2, p3, n1, n2, n3, glm::vec2(0), glm::vec2(0), glm::vec2(0))
{}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3)
{
    plane_normal = glm::normalize(glm::cross(p2 - p1, p3 - p2));
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;
    uvs[0] = t1;
    uvs[1] = t2;
    uvs[2] = t3;
}

//Returns the interpolation of the triangle's three normals based on the point inside the triangle that is given.
//glm::vec3 Triangle::GetNormal(const glm::vec3 &position)
//{
//    float d0 = glm::distance(position, points[0]);
//    float d1 = glm::distance(position, points[1]);
//    float d2 = glm::distance(position, points[2]);
//    return (normals[0] * d0 + normals[1] * d1 + normals[2] * d2)/(d0+d1+d2);
//}

glm::vec4 Triangle::GetNormal(const glm::vec4 &position)
{
    glm::vec3 result = GetNormal(glm::vec3(position));
    return glm::vec4(result, 0);
}

//HAVE THEM IMPLEMENT THIS
//The ray in this function is not transformed because it was *already* transformed in Mesh::GetIntersection
// ^^^ ????
Intersection Triangle::GetIntersection(Ray r)
{
    float t, S, S1, S2 ,S3;
    Intersection triangleIntersection;
    // check if it hits in the plane of the triangle


    glm::vec4 rw_origin = transform.T()*glm::vec4(r.origin,1.0f);
    glm::vec3 rtrans_origin = glm::vec3(rw_origin[0], rw_origin[1], rw_origin[2]);

    //calculate plane normal
    glm::vec3 planeNorm = glm::cross((points[1] - points[0]),( points[2] - points[0]));
    t  = glm::dot(planeNorm, (points[1] -r.origin)) / glm::dot(planeNorm, r.direction);

    // calculate intersection point
    glm::vec3 intersectionPoint = r.origin + t*r.direction;

    // world space point
//    glm::vec4 transPoint = transform.T()*glm::vec4(intersectionPoint, 1);
//    glm::vec3 worldPoint = glm::vec3(transPoint[0], transPoint[1], transPoint[2]);

    //then check if its inside the triangle
    S = calcTriArea(points[0], points[1], points[2]);
    S1 = calcTriArea(intersectionPoint, points[1], points[2]) / S;
    S2 = calcTriArea(intersectionPoint, points[2], points[0]) / S;
    S3 = calcTriArea(intersectionPoint, points[0], points[1]) / S;
     if(fequal((S1+S2+S3), 1.0f))
        {
            triangleIntersection.object_hit = this;
          triangleIntersection.normal = GetNormal(intersectionPoint);
             triangleIntersection.point = intersectionPoint;
            triangleIntersection.t = t;
            if(this->material->texture!= NULL)
           {
                triangleIntersection.intersectColor = this->material->GetImageColor( this->GetUVCoordinates(intersectionPoint),
                                                                                 this->material->texture);
            }
            else
            {
                triangleIntersection.intersectColor = this->material->base_color;
            }

            return triangleIntersection;
        }
    triangleIntersection.object_hit = NULL;

    return triangleIntersection;
}

void Triangle::setBbox()
{
    glm::vec3 p[3];
      p[0] = glm::vec3(transform2*glm::vec4(points[0],1.0f));
      p[1] = glm::vec3(transform2*glm::vec4(points[1],1.0f));
      p[2] = glm::vec3(transform2*glm::vec4(points[2],1.0f));
//    points[2] = glm::vec3(transform2*glm::vec4(points[2],1.0f));

    glm::vec3 max = p[0];
    glm::vec3 min =p[0];
    for (int i = 0; i < 3; i ++)
    {
        max.x = glm::max (p[i].x, max.x);
        max.y = glm::max(p[i].y , max.y);
        max.z = glm::max(p[i].z , max.z);

        min.x = glm::min( p[i].x, min.x);
        min.y = glm::min(p[i].y, min.y);
        min.z = glm::min(p[i].z, min.z);
    }

   this->bb = new BoundingBox(max,min);
}
// change get intersection with mesh
Intersection Mesh::GetIntersection(Ray r)
{
  Intersection t_int;

   Ray rt = r.GetTransformedCopy(transform.invT());
   Intersection tri_near;

   for(Triangle *t : faces)
   {
      t_int = t->GetIntersection(rt);
      if(fequal(tri_near.t,-1.0f))
      {
          tri_near = t_int;
      }
      if (t_int.t > 0)
      {
         if( tri_near.t > t_int.t )
         {
             tri_near = t_int;
         }
      }
   }

//   tri_near.normal = this->:GetNormal(tri_near.point);
   glm::vec4 transPoint = transform.T()*glm::vec4(tri_near.point, 1);
   tri_near.point  = glm::vec3(transPoint[0], transPoint[1], transPoint[2]);
    tri_near.normal = glm::vec3(transform.T()*glm::vec4(tri_near.normal,0));
    return tri_near;
}

void Mesh::SetMaterial(Material *m)
{
    this->material = m;
    for(Triangle *t : faces)
    {
        t->SetMaterial(m);
    }
}
QList<Triangle*>Mesh::getFaces()
{
    return  faces;
}

void Mesh::setBbox()
{
//    qDebug() << "I am here";
   glm::vec3 bmax = glm::vec3(0);
   glm::vec3 bmin = glm::vec3(0);
    for(Triangle *t : faces)
    {
//        qDebug() << " I am looking at faces ";
        glm::vec3 tmax = t->bb->max_bounds;
        glm::vec3 tmin = t->bb->min_bounds;
        for(int i = 0; i < 3; i++)
        {
            if(tmax[i] > bmax[i])
            {
                bmax[i] = tmax[i];
            }

           if (tmin[i] < bmin[i])

            {
                bmin[i] = tmin[i];
            }
        }

     }
//    qDebug() << "mesh x coordinate bmax" << bmax[0];
//    bmax = glm::vec3(this->transform.T()*glm::vec4(bmax,1.0f));
//    bmin = glm::vec3(this->transform.T()*glm::vec4(bmin,1.0f));
    qDebug() << " max coords" << bmax[0];
    this->bb  = new BoundingBox(bmax, bmin);
    this->bb->center = this->bb->getCenterPoint(bmin,bmax);
}

void Triangle::getTransform( glm::mat4 transform)
{
    transform2 = transform;
}

//Intersection Mesh::GetIntersection(Ray r, BVHNode* mesh_root)
//{
//    glm::vec3 min = root->bb->min_bounds;
//    glm::vec3 max = root->bb->max_bounds;
//    if ( root->leftChild == NULL && root->rightChild == NULL )
//    {
//        return root->geo->GetIntersection(r);
//    }

//    float t_0, t_1, t_near, t_far;
//    t_near = -INFINITY;
//    t_far = INFINITY;

//    //calculate t0, t1 for all planes
//    for (int i = 0; i < 3; i++)
//    {
//        if(fequal(r.direction[i],0.0f))
//        {
//           if(r.origin[i] < root->bb->min_bounds[i]  || r.origin[i] >root->bb->max_bounds[i])
//           {
//               continue;
//           }
//        }
//        t_0 = (min[i]- r.origin[i])/ r.direction[i];
//        t_1 = (max[i] - r.origin[i])/ r.direction[i];

//        if(t_0 > t_1)
//        {
//            float swap = t_0;
//            t_0 = t_1;
//            t_1 = swap;

//        }

//        t_near = glm::max(t_0,t_near);
//        t_far  = glm::min(t_1, t_far);
//    }


//    if(t_near > t_far || t_near < 0 || t_far < 0)
//    {
//        Intersection isx;
//        isx.object_hit = NULL;
//        isx.t = -1.0f;
//        return isx;
//    }

//   Intersection i1  = BbIsx(r, root->leftChild);
//   Intersection i2 = BbIsx(r, root->rightChild);
//   if ( i1.object_hit == NULL)
//   {
//       return i2;
//   }
//   if(i1.object_hit != NULL && i2.object_hit != NULL)
//   {
//       if (i2.t < i1.t)
//       {
//           return i2;
//       }
//   }
//   return i1;
//}

void Mesh::LoadOBJ(const QStringRef &filename, const QStringRef &local_path)
{
    QString filepath = local_path.toString(); filepath.append(filename);
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<float> &positions = shapes[i].mesh.positions;
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;
            std::vector<unsigned int> &indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                glm::vec3 p1(positions[indices[j]*3], positions[indices[j]*3+1], positions[indices[j]*3+2]);
                glm::vec3 p2(positions[indices[j+1]*3], positions[indices[j+1]*3+1], positions[indices[j+1]*3+2]);
                glm::vec3 p3(positions[indices[j+2]*3], positions[indices[j+2]*3+1], positions[indices[j+2]*3+2]);

                Triangle* t = new Triangle(p1, p2, p3);
                if(normals.size() > 0)
                {
                    glm::vec3 n1(normals[indices[j]*3], normals[indices[j]*3+1], normals[indices[j]*3+2]);
                    glm::vec3 n2(normals[indices[j+1]*3], normals[indices[j+1]*3+1], normals[indices[j+1]*3+2]);
                    glm::vec3 n3(normals[indices[j+2]*3], normals[indices[j+2]*3+1], normals[indices[j+2]*3+2]);
                    t->normals[0] = n1;
                    t->normals[1] = n2;
                    t->normals[2] = n3;
                }
                if(uvs.size() > 0)
                {
                    glm::vec2 t1(uvs[indices[j]*2], uvs[indices[j]*2+1]);
                    glm::vec2 t2(uvs[indices[j+1]*2], uvs[indices[j+1]*2+1]);
                    glm::vec2 t3(uvs[indices[j+2]*2], uvs[indices[j+2]*2+1]);
                    t->uvs[0] = t1;
                    t->uvs[1] = t2;
                    t->uvs[2] = t3;
                }
                this->faces.append(t);
            }
        }
        std::cout << "" << std::endl;
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }
}

void Mesh::create(){

    //Count the number of vertices for each face so we can get a count for the entire mesh
        std::vector<glm::vec3> vert_pos;
        std::vector<glm::vec3> vert_nor;
        std::vector<glm::vec3> vert_col;
        std::vector<GLuint> vert_idx;

        unsigned int index = 0;

        for(int i = 0; i < faces.size(); i++){
            Triangle* tri = faces[i];
            tri->getTransform(this->transform.T());
            tri->setBbox();
            vert_pos.push_back(tri->points[0]); vert_nor.push_back(tri->normals[0]); vert_col.push_back(tri->material->base_color);
            vert_pos.push_back(tri->points[1]); vert_nor.push_back(tri->normals[1]); vert_col.push_back(tri->material->base_color);
            vert_pos.push_back(tri->points[2]); vert_nor.push_back(tri->normals[2]); vert_col.push_back(tri->material->base_color);
            vert_idx.push_back(index++);vert_idx.push_back(index++);vert_idx.push_back(index++);
        }

        count = vert_idx.size();
        int vert_count = vert_pos.size();

       this->setBbox();
//        this->bb->create();
        bufIdx.create();
        bufIdx.bind();
        bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufIdx.allocate(vert_idx.data(), count * sizeof(GLuint));

        bufPos.create();
        bufPos.bind();
        bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufPos.allocate(vert_pos.data(), vert_count * sizeof(glm::vec3));

        bufCol.create();
        bufCol.bind();
        bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufCol.allocate(vert_col.data(), vert_count * sizeof(glm::vec3));

        bufNor.create();
        bufNor.bind();
        bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bufNor.allocate(vert_nor.data(), vert_count * sizeof(glm::vec3));
}

//This does nothing because individual triangles are not rendered with OpenGL; they are rendered all together in their Mesh.
void Triangle::create()
{
}

glm::vec2  Triangle::GetUVCoordinates(const glm::vec3 &point)
{
    float t, S, S1, S2 ,S3;
    S = calcTriArea(points[0], points[1], points[2]);
    S1 = calcTriArea(point, points[1], points[2]) / S;
    S2 = calcTriArea(point, points[2], points[0]) / S;
    S3 = calcTriArea(point, points[0], points[1]) / S;
    glm::vec2 uv_coord  = (uvs[0]*S1/S)  + (uvs[1] *S2/S) + (uvs[2]*S3/S);
    return uv_coord;
}

glm::vec2 Mesh::GetUVCoordinates(const glm::vec3 &point)
{

    return glm::vec2(0,0);

}
