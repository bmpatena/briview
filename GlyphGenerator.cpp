#include "GlyphGenerator.h"

#include <iostream>
#include "vectorFunctions.h"
#define PI 3.14159265358979323846

using namespace std;
using namespace fslsurface_name;

namespace briview{

    void  generateArrowGlyph( vector< vertex<float> > & vertices, vector<GLuint> & indices,  const float & head_radius, const float & shaft_radius, const float & height, const float & head_height, const unsigned int & N_faces )
    {
            float d_theta = 2*PI/N_faces ; //radians

      //  //cout<<"NVERTS "<<N_verts.at(surf_index)<<endl;


        unsigned int N_verts_head = N_faces;//floor(2*PI/(d_theta));
        vertex<float> vert;
    //    vertices.assign(3*N_verts_head + 2, vert);
      //  indices.assign(N_verts_head * 6 * 3,0);

        vertices.clear();
        for (unsigned int i = 0 ; i< 3*N_verts_head + 2 ; i++)
            vertices.push_back(vert);

        for (unsigned int i = 0 ; i< N_verts_head * 6 * 3 ; i++)
            indices.push_back(0);

       // vertex*  vertices    = new vertex[4*N_verts_head + 2 ];
      //  GLuint* indices = new GLuint[N_verts_head * 6 * 3 ];

        vertices[0].x = 0;
        vertices[0].y = 0;
        vertices[0].z = height;
         vertices[0].nx = 0;
        vertices[0].ny = 0;
        //vertices[0].nz = 0;

        vertices[0].sc = 1 ;
        //---------create head -----------------//

        float theta = 0 ;
        float z = height - head_height * height ;
        for ( unsigned int i = 1  ; i <= N_verts_head ; i++, theta += d_theta)
        {
          // cout<<"create verts"<<endl;
            vertices[i].x = head_radius * cosf(theta);
            vertices[i].y = head_radius * sinf(theta);
            vertices[i].z = z;
            vertices[i].nx = 0;
            vertices[i].ny = 0;
            vertices[i].nz = 0;

             vertices[i].sc=1;

        }

        for ( unsigned int i = 0  ; i < N_verts_head ; i++ )
        {
            indices[3*i] = 0 ;
            indices[3*i+1] = i + 2 ;
            indices[3*i+2] = i + 1;
        }
        indices[3*N_verts_head-2] = 1 ;
//---------end create head -----------------//
        //--------------under part of head-----------/

        theta = 0;

        for ( unsigned int i = N_verts_head + 1 ; i <= 2*N_verts_head ; i++, theta += d_theta)
        {
            vertices[i].x = shaft_radius * cosf(theta);
            vertices[i].y = shaft_radius * sinf(theta);
            vertices[i].z = z;

             vertices[i].nx = 0;
            vertices[i].ny = 0;
            vertices[i].nz = 0;

             vertices[i].sc=1;
          //    cout<<"create verts "<< i <<" "<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<endl;
        }


        unsigned int i_outer = 1;
        unsigned int index = 3*N_verts_head;
        for ( unsigned int i = N_verts_head ; i < 2*N_verts_head ; i++, i_outer++ , index+=6)
        {
            indices[ index   ] =  i +1 ;
            indices[ index+1 ] =  i_outer  ;
            indices[ index+2 ] = i + 2 ;

            indices[ index+3 ] =  i_outer;
            indices[ index+4 ] =  i_outer + 1 ;
            indices[ index+5 ] =  i + 2 ;

        }

       indices[3*3*N_verts_head-4] = N_verts_head + 1 ;
       indices[3*3*N_verts_head-1] = N_verts_head + 1 ;
       indices[3*3*N_verts_head-2] =  1 ;
//--------create shaft -----------------//

//-------------shaft itself--------------//
         theta = 0;
        for ( unsigned int i = 2*N_verts_head + 1 ; i <= 3*N_verts_head ; i++, theta += d_theta)
        {
            vertices[i].x = shaft_radius * cosf(theta);
            vertices[i].y = shaft_radius * sinf(theta);
            vertices[i].z = 0;

             vertices[i].nx = 0;
            vertices[i].ny = 0;
            vertices[i].nz = 0;

             vertices[i].sc=1;
          //   cout<<"create verts shaft"<< i <<" "<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<endl;
        }


        i_outer = N_verts_head + 1;
        index = 3*3*N_verts_head;
        for ( unsigned int i = 2*N_verts_head ; i < 3*N_verts_head ; i++, i_outer++ , index+=6)
        {
            indices[ index   ] =  i +1 ;
            indices[ index+1 ] =  i_outer ;
            indices[ index+2 ] = i + 2 ;

            indices[ index+3 ] =  i_outer;
            indices[ index+4 ] =  i_outer + 1 ;
            indices[ index+5 ] =  i + 2 ;

        }
       indices[3*5*N_verts_head-4] =  2*N_verts_head +1 ;
       indices[3*5*N_verts_head-1] = 2*N_verts_head + 1 ;
       indices[3*5*N_verts_head-2] =  N_verts_head + 1 ;

//--------bottom of shaft -----------------//


        vertices[3*N_verts_head+1].x=0;
        vertices[3*N_verts_head+1].y=0;
        vertices[3*N_verts_head+1].z=0;
    vertices[3*N_verts_head+1].nx=0;
        vertices[3*N_verts_head+1].ny=0;
        vertices[3*N_verts_head+1].nz=0;

        index = 3*5*N_verts_head;
        for ( unsigned int i = 2*N_verts_head  ; i < 3*N_verts_head ; i++ , index += 3)
        {
            indices[index] = 3*N_verts_head +1;
            indices[index+1] = i + 1 ;
            indices[index+2] = i + 2 ;
        }
        indices[3*6*N_verts_head-1] =  2*N_verts_head + 1;

//--------end of bottom of shaft -----------------//


 //       for ( unsigned int i = 0  ; i < 3*N_verts_head ; i++ )
   //     {
     //        //cout<<"indices "<< i <<" "<<indices[3*i]<<" "<<indices[3*i+1]<<" "<<indices[3*i+2]<<endl;

       // }

    }


    void  generateArrowGlyphs( std::vector<fslsurface_name::vertex<float> > & v_vertices, vector<GLuint> & v_indices, const  fslSurface< float, unsigned int >* surf,  const unsigned int & vec_index, const float & head_radius, const float & shaft_radius, const float & height, const float & head_height, const unsigned int & N_faces )
    {


  //      unsigned int N_verts = surf->getNumberOfVertices();
        if (surf->getNumberOfVectorData() == 0){
            cerr<<"Cannot generate glyphs without vector data"<<endl;
            return;
        }
   //   cout<<"NVERTYS "<<surf->getNumberOfVertices()<<" "<<surf->getNumberOfScalarData()<<endl;

      //  vertices_org = vertices_one;
       // indices_org = indices_one;

     //   for (unsigned int i = 0 ; i < N_verts; i++, ++i_v)
        //iterator to given vector
        vector<float>::const_iterator i_vec = surf->const_vecbegin(vec_index);
      for (vector< vertex<float> >::const_iterator i_v = surf->const_vbegin(); i_v != surf->const_vend();++i_v, i_vec+=3)
        {
           // vertices_one = vertices_org;
          //  indices_one = indices_org;
             vector< vertex<float> > vertices_one;
        vector<GLuint> indices_one;
//  v_vertices.clear();
//v_indices.clear();
      //  cout<<"height "<<height<<endl;
             float3 v= float3(*i_vec,*(i_vec+1), *(i_vec+2));//vecs[i];
             float sc=sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);

              // cout<<"v "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
        generateArrowGlyph( vertices_one, indices_one, sc*head_radius, sc*shaft_radius, sc*height, head_height, N_faces );

//             generateArrowGlyph( vertices_one, indices_one, i_v->sc*head_radius, i_v->sc*shaft_radius, i_v->sc*height, head_height, N_faces );
//    cout<<"i: "<<" "<<vertices_one.size()<<" "<<indices_one.size()<<endl;


              //  v.x*=-1;
              //  v.y*=-1;
               // v.z*=-1;
            normalize(v);
            float theta=PI/2.0;


            if (v.x!=0)
            {
                theta = atan(v.y/v.x) ;//+ PI/2.0;
                if (v.x < 0)
                       theta+=PI;
            }

                //            float theta = acos(v.x);
  //          if (v.y < 0 ) theta*=-1;//+=PI;
           //if ( v.y < 0 )
             //   theta += PI;
               //     theta *= -1; //+= PI;

            float phi = PI/2.0 - asin(v.z);
//cout<<"theta/phi "<<v.x<<" "<<v.y<<" "<<v.z<<" "<<theta<<" "<<phi<<endl;
            rotateVertices(vertices_one, theta, phi);

            shiftVertices( vertices_one, i_v->x, i_v->y, i_v->z  );
            calculateNormals(vertices_one, indices_one );
            shiftIndices( indices_one , v_vertices.size() );



            setVertexScalars(vertices_one, i_v->sc);



////cout<<"cum normals "<<endl;

            v_vertices.insert( v_vertices.end(),  vertices_one.begin(), vertices_one.end());
            v_indices.insert( v_indices.end(),  indices_one.begin(), indices_one.end());


        }
    }


}
