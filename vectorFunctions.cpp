#include "vectorFunctions.h"
#include <iostream>
using namespace std;
#define PI 3.14159265
using namespace fslsurface_name;
namespace briview {

    void normalize(float3 & v)
    {
          float norm = sqrt( v.x*v.x  + v.y*v.y + v.z*v.z );

            v.x /= norm;
            v.y /= norm;
            v.z /= norm;
    }

    float3 subtract(const float3 & a, const float3 & b){

        float3 c;
        c.x=a.x-b.x;
        c.y=a.y-b.y;
        c.z=a.z-b.z;

        return c;
    }

    float3 normal(const float3 & v1, const float3 & v2 )
    {
        float3 normal;
        normal.x=v1.y*v2.z - v1.z*v2.y;
        normal.y=v1.z*v2.x - v1.x*v2.z;
        normal.z=v1.x*v2.y - v1.y*v2.x;
        float l=sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x/=l;
        normal.y/=l;
        normal.z/=l;
        return normal;
    }

   void rotateVertices( std::vector< vertex<float> > & vertices, const float & theta, const float & phi )
        {
            for (vector< vertex<float> >::iterator i = vertices.begin(); i!= vertices.end(); i++)
              {
//cout<<"do rotate "<<i->x<<" "<<i->y<<" "<<i->z<<endl;
              //  float phi2=PI/4.0;
                  //  float x = cosf(phi) * i->x + sinf(phi) * i->z;
                   // float z = -sinf(phi) * i->x + cosf(phi) * i->z;

                     //     i->x = cosf(theta) * x - sinf(theta) * i->y;
                      //   i->y = sinf(theta) * x + cosf(theta) * i->y;

      //                   float x = cosf(phi) * i->x + sinf(phi) * i->z;
    //                i->z = -sinf(phi) * i->x + cosf(phi) * i->z;

  //                        i->x = cosf(theta) * x - sinf(theta) * i->y;
//                         i->y = sinf(theta) * x + cosf(theta) * i->y;

                         float x = cosf(phi) * i->x + sinf(phi) * i->z;
                    i->z = -sinf(phi) * i->x + cosf(phi) * i->z;

                          i->x = cosf(theta) * x - sinf(theta) * i->y;
                         i->y = sinf(theta) * x + cosf(theta) * i->y;

                      //   cout<<"do rotate2 "<<i->x<<" "<<i->y<<" "<<i->z<<endl;




                     //i->z = z ;

              }
    }


    void shiftVertices( vector< vertex<float> > & vertices, const float & dx, const float & dy , const float & dz )
    {
        for (vector< vertex<float> >::iterator i = vertices.begin(); i!= vertices.end(); i++)
        {
            i->x += dx;
            i->y += dy;
            i->z += dz;
        }


    }

        void setVertexScalars( std::vector< vertex<float> > & vertices, const float & scalar )
        {
            for (vector< vertex<float> >::iterator i = vertices.begin(); i!= vertices.end(); i++)
          {
             i->sc = scalar;
          }

        }



      void shiftIndices( std::vector<GLuint> & indices, const unsigned int & index_shift)
      {
            for (vector<GLuint>::iterator i = indices.begin(); i != indices.end() ; i++)
                *i += index_shift;

      }


    void calculateNormals(  vertex<float> * verts, GLuint* indices, const unsigned int & N_verts,  const unsigned int & N_indices){
        for (unsigned int i=0;  i < N_indices; i+=3){
            float3 p0 = float3( (verts+*(indices+i))->x        , (verts+*(indices+i) )->y      , (verts+ *(indices+i) )->z       );
            float3 p1 = float3((verts+*(indices+i+1) )->x, (verts+*(indices+i+1) )->y, (verts+ *(indices+i+1) )->z );
            float3 p2 = float3((verts+*(indices+i+2) )->x, (verts+*(indices+i+2) )->y, (verts+ *(indices+i+2) )->z );
            float3 n = normal(subtract(p0,p1),subtract(p0,p2));

            verts[*(indices+i)   ].nx  +=   n.x;
            verts[*(indices+i)   ].ny  +=   n.y;
            verts[*(indices+i)   ].nz  +=   n.z;

            verts[*(indices+i+1) ].nx  +=   n.x;
            verts[*(indices+i+1) ].ny  +=   n.y;
            verts[*(indices+i+1) ].nz  +=   n.z;

            verts[*(indices+i+2) ].nx  +=   n.x;
            verts[*(indices+i+2) ].ny  +=   n.y;
            verts[*(indices+i+2) ].nz  +=   n.z;
          //  cout<<"cumularte nroms "<<i<<" "<<*(indices+i)<<" "<<*(indices+i+1)<<" "<<*(indices+i+2)<<endl;
        }

        for (unsigned int i=0;  i < N_verts; i++){
            float norm = sqrt( verts[i].nx* verts[i].nx +  verts[i].ny* verts[i].ny + verts[i].nz* verts[i].nz);
            verts[i].nx   /= norm;
            verts[i].ny /= norm;
            verts[i].nz /= norm;
       // cout<<"verts "<<verts[i].nx<<" "<<verts[i].ny<<" "<<verts[i].nz<<endl;
        }


    }


 void calculateNormals( vector< vertex<float> > &  verts, const vector<GLuint> & indices){
    //   unsigned int N_indices = indices.size();
    //   unsigned int N_verts = verts.size();
//cout<<"cum normals "<<endl;
       for (vector<GLuint>::const_iterator i = indices.begin(); i!=indices.end(); i+=3)
       {
            float3 p0 = float3( verts[*(i)   ].x, verts[*(i)   ].y, verts[*(i)   ].z   );
            float3 p1 = float3( verts[*(i+1) ].x, verts[*(i+1) ].y, verts[*(i+1) ].z );
            float3 p2 = float3( verts[*(i+2) ].x, verts[*(i+2) ].y, verts[*(i+2) ].z );
            float3 n = normal(subtract(p0,p1),subtract(p0,p2));

            verts[*(i)   ].nx  +=   n.x;
            verts[*(i)   ].ny  +=   n.y;
            verts[*(i)   ].nz  +=   n.z;

            verts[*(i+1) ].nx  +=   n.x;
            verts[*(i+1) ].ny  +=   n.y;
            verts[*(i+1) ].nz  +=   n.z;

            verts[*(i+2) ].nx  +=   n.x;
            verts[*(i+2) ].ny  +=   n.y;
            verts[*(i+2) ].nz  +=   n.z;
      //      cout<<"noralize1 "<<*(i)<<" "<<*(i+1)<<" "<<*(i+2)<<endl;
       //     cout<<"noralize "<<n.x<<" "<<n.y<<" "<<n.z<<endl;
         //   cout<<"noralize "<<p0.x<<" "<<p0.y<<" "<<p0.z<<endl;
          //  cout<<"noralize "<<p1.x<<" "<<p1.y<<" "<<p1.z<<endl;
//            cout<<"noralize5 "<<p2.x<<" "<<p2.y<<" "<<p2.z<<endl;

       }

        for (vector< vertex<float> >::iterator i = verts.begin(); i!= verts.end(); i++)
       {
             float norm = sqrt( i->nx* i->nx +  i->ny* i->ny + i->nz* i->nz);

            i->nx /= norm;
            i->ny /= norm;
            i->nz /= norm;
  //      cout<<"done normals "<< i->nx<<" "<< i->ny<<" "<< i->nz<<endl;

        }

    }


}
