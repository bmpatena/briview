#ifndef BRIVIEW_STRUCTS_H
#define BRIVIEW_STRUCTS_H

#include <QtOpenGL>
#include <cmath>
namespace briview{
    //enums
    enum AnimationType { Start, CameraMotion, SliceMoving };


    struct conn{
        float strength;
        int src,dest;
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
/*    struct float2{
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
    //structs
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



    struct glyph{

        glyph(float  tip_r,float  tip_h,float  base_r, \
                 float  sc, int n_faces, float op )
        {
            tip[0]=tip_r; tip[1]=tip_h; base_radius=base_r;
            scale_factor=sc; N_faces= n_faces; opacity = op;
        }
        glyph(){

        }

        GLfloat tip[2]; //radius, height
        GLfloat base_radius; //radius
        GLfloat scale_factor;
        GLfloat opacity;
        GLint N_faces;


    };
    struct colour_table{
        colour_table( float r1, float r2, float r3, float r4, float r5, \
                      float g1, float g2, float g3, float g4, float g5, \
                      float b1, float b2, float b3, float b4, float b5, \
                      float sc1, float sc2, float sc3, float sc4, float sc5, \
                      float low_clamp_r, float low_clamp_g, float low_clamp_b )
        {
            r_lut[0]=r1; r_lut[1]=r2; r_lut[2]=r3; r_lut[3]=r4; r_lut[4]=r5;
            g_lut[0]=g1; g_lut[1]=g2; g_lut[2]=g3; g_lut[3]=g4; g_lut[4]=g5;
            b_lut[0]=b1; b_lut[1]=b2; b_lut[2]=b3; b_lut[3]=b4; b_lut[4]=b5;
            sc_lut[0]=sc1; sc_lut[1]=sc2; sc_lut[2]=sc3; sc_lut[3]=sc4; sc_lut[4]=sc5;
            low_clamp[0] = low_clamp_r; low_clamp[1] = low_clamp_g; low_clamp[2] = low_clamp_b;
        }

        GLfloat r_lut[5];
        GLfloat g_lut[5];
        GLfloat b_lut[5];
        GLfloat sc_lut[5];
        GLfloat low_clamp[3];
    };

  struct vertex
    {

        float x, y, z;        //Vertex
        float nx, ny, nz;     //Normal
        float scalar;
    };
*/
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

  struct animationParams
  {
      animationParams(float tx_i,float ty_i,float tz_i,float theta_i,float phi_i, float duration_i){
          type=CameraMotion;
          tx=tx_i;
          ty=ty_i;
          tz=tz_i;
          theta=theta_i;
          phi=phi_i;
          duration=duration_i;

      }
      animationParams(float3 eye_i, float3 centre_i,float3 vup_i, float duration_i){
          type=Start;
            centre=centre_i;
            eye=eye_i;
            vup=vup_i;
            duration=duration_i;

      }
      animationParams(unsigned int x_1_vox_i, unsigned int y_1_vox_i, unsigned int z_1_vox_i, bool x_1_on_i, bool y_1_on_i, bool z_1_on_i, \
                      unsigned int x_2_vox_i, unsigned int y_2_vox_i, unsigned int z_2_vox_i, bool x_2_on_i, bool y_2_on_i, bool z_2_on_i, float duration_i ){
          type=SliceMoving;
          x_1_vox=x_1_vox_i;
          y_1_vox=y_1_vox_i;
          z_1_vox=z_1_vox_i;
          x_2_vox=x_2_vox_i;
          y_2_vox=y_2_vox_i;
          z_2_vox=z_2_vox_i;
          x_1_on=x_1_on_i;
          y_1_on=y_1_on_i;
          z_1_on=z_1_on_i;
          x_2_on=x_2_on_i;
          y_2_on=y_2_on_i;
          z_2_on=z_2_on_i;
          duration=duration_i;
      }

      AnimationType type;
      float theta, phi;
      float tx,ty,tz;
        float3 vup, eye,centre;
        unsigned int x_1_vox,y_1_vox,z_1_vox;
        unsigned int x_2_vox,y_2_vox,z_2_vox;
        bool x_1_on, y_1_on,z_1_on;
        bool x_2_on, y_2_on,z_2_on;
        float duration;



  };


}
#endif // BRIVIEW_STRUCTS_H
