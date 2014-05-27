#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H
#include <QtOpenGL>

#include <cmath>

#define MAX_NUMBER_OF_IMAGES 4

namespace surface_structs{

    struct float4{
        float4(float xin, float yin, float zin, float win)
        {
            x=xin;
            y=yin;
            z=zin;
            w=win;

        }
        float4()
        {

        }
        float x,y,z,w;
    };

    struct float3{
        float3(float xin, float yin, float zin)
        {
            x=xin;
            y=yin;
            z=zin;
        }
        float3()
        {

        }
        float x,y,z;
    };
    struct float2{
        float2(float xin, float yin)
        {
            x=xin;
            y=yin;
        }
        float2()
        {

        }
        float x,y;
    };
    struct bool3{
        bool3(bool xin, bool yin, bool zin)
        {
            x=xin;
            y=yin;
            z=zin;
        }
        bool3()
        {

        }
        bool x,y,z;
    };



    struct int2{
        int2(int xin,int yin)
        {
            x=xin;
            y=yin;
        }
        int2()
        {

        }

        float x,y;
    };

    struct material{

        material(float  ar,float  ag,float  ab,float  aa, \
                 float  dr,float  dg,float  db,float  da, \
                 float  sr,float  sg,float  sb,float  sa, \
                 float  sh, float op )
        {
            ambient[0]=ar; ambient[1]=ag; ambient[2]=ab; ambient[3]=aa;
            diffuse[0]=dr; diffuse[1]=dg; diffuse[2]=db; diffuse[3]=da;
            specular[0]=sr; specular[1]=sg; specular[2]=sb; specular[3]=sa;
            shininess=sh; opacity=op;
        }
        material(){

        }

        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat shininess;
        GLfloat opacity;
    };


    struct colour_table{
        colour_table( float r1, float r2, float r3, float r4, \
                      float g1, float g2, float g3, float g4, \
                      float b1, float b2, float b3, float b4, \
                      float sc1, float sc2, float sc3, float sc4 )
        {
            r_lut[0]=r1; r_lut[1]=r2; r_lut[2]=r3; r_lut[3]=r4; \
            g_lut[0]=g1; g_lut[1]=g2; g_lut[2]=g3; g_lut[3]=g4; \
            b_lut[0]=b1; b_lut[1]=b2; b_lut[2]=b3; b_lut[3]=b4; \
            sc_lut[0]=sc1; sc_lut[1]=sc2; sc_lut[2]=sc3; sc_lut[3]=sc4;
        }

        GLfloat r_lut[4];
        GLfloat g_lut[4];
        GLfloat b_lut[4];
        GLfloat sc_lut[4];
    };

    struct bounds{
        bounds( GLfloat xmin_in, GLfloat xmax_in,  GLfloat ymin_in, GLfloat ymax_in, GLfloat zmin_in, GLfloat zmax_in)
        {
            xmin=xmin_in;
            xmax=xmax_in;
            ymin=ymin_in;
            ymax=ymax_in;
            zmin=zmin_in;
            zmax=zmax_in;
        }
        GLfloat xmin;
        GLfloat xmax;
        GLfloat ymin;
        GLfloat ymax;
        GLfloat zmin;
        GLfloat zmax;


    };


}


#endif // MY_STRUCTS_H
