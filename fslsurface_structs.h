#ifndef fslsurface_STRUCTS_H
#define fslsurface_STRUCTS_H

#ifdef __linux
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <OpenGL/gl.h>
#endif
namespace fslsurface_name{
	enum MarchingCubesMode {EQUAL_TO, GREATER_THAN, GREATER_THAN_EQUAL_TO};

	template <class T>
	struct vertex {
		
		vertex(T xin, T yin, T zin){
			x=xin;
			y=yin;
			z=zin;
			nx = 0;
			ny = 0;
			nz = 0;
			sc=0;
		}	
		vertex(T sc_in){
			x = 0;
			y = 0;
			z = 0;
			nx = 0;
			ny = 0;
			nz = 0;
			sc=sc_in;
		}
		vertex(){
			
		}
		
		
		T x;
		T y;
		T z;
		T nx;
		T ny;
		T nz;
		T sc;
	};

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

	template<class T>
	struct vec3 {
		vec3(T xin, T yin, T zin)
        {
            x=xin;
            y=yin;
            z=zin;
        }
        vec3()
        {
			
        }
        T x,y,z;
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
        colour_table( float r1, float r2, float r3, float r4, float r5,float r6, \
                      float g1, float g2, float g3, float g4, float g5,float g6, \
                      float b1, float b2, float b3, float b4, float b5,float b6, \
                      float a1, float a2, float a3, float a4, float a5, float a6, \
                      float sc1, float sc2, float sc3, float sc4, float sc5, float sc6, \
                      float low_clamp_r, float low_clamp_g, float low_clamp_b ,  float low_clamp_a)
        {
            r_lut[0]=r1; r_lut[1]=r2; r_lut[2]=r3; r_lut[3]=r4; r_lut[4]=r5; r_lut[5]=r6; \
            g_lut[0]=g1; g_lut[1]=g2; g_lut[2]=g3; g_lut[3]=g4; g_lut[4]=g5; g_lut[5]=g6; \
            b_lut[0]=b1; b_lut[1]=b2; b_lut[2]=b3; b_lut[3]=b4; b_lut[4]=b5; b_lut[5]=b6; \
                        a_lut[0]=a1; a_lut[1]=a2; a_lut[2]=a3; a_lut[3]=a4; a_lut[4]=a5; a_lut[5]=a6;\
            sc_lut[0]=sc1; sc_lut[1]=sc2; sc_lut[2]=sc3; sc_lut[3]=sc4; sc_lut[4]=sc5; sc_lut[5]=sc6;\
            low_clamp[0] = low_clamp_r; low_clamp[1] = low_clamp_g; low_clamp[2] = low_clamp_b;low_clamp[3] = low_clamp_a;
        }
                colour_table( float4 r, float2 r5, \
                                         float4 g, float2 g5, \
                                         float4 b, float2 b5, \
                                         float4 a, float2 a5, \
                                         float4 sc, float2 sc5, \
					 float low_clamp_r, float low_clamp_g, float low_clamp_b ,  float low_clamp_a)
        {
            r_lut[0]=r.x; r_lut[1]=r.y; r_lut[2]=r.z; r_lut[3]=r.w; r_lut[4]=r5.x; r_lut[5]=r5.y; \
            g_lut[0]=g.x; g_lut[1]=g.y; g_lut[2]=g.z; g_lut[3]=g.w; g_lut[4]=g5.x; g_lut[5]=g5.y;\
            b_lut[0]=b.x; b_lut[1]=b.y; b_lut[2]=b.z; b_lut[3]=b.w; b_lut[4]=b5.x; b_lut[5]=b5.y;\
            a_lut[0]=a.x; a_lut[1]=a.y; a_lut[2]=a.z; a_lut[3]=a.w; a_lut[4]=a5.x; a_lut[5]=a5.y;\
            sc_lut[0]=sc.x; sc_lut[1]=sc.y; sc_lut[2]=sc.z; sc_lut[3]=sc.w; sc_lut[4]=sc5.x; sc_lut[5]=sc5.y;  \
            low_clamp[0] = low_clamp_r; low_clamp[1] = low_clamp_g; low_clamp[2] = low_clamp_b;low_clamp[3] = low_clamp_a;
        }

		colour_table()
		{
                        r_lut[0]=0; r_lut[1]=0; r_lut[2]=0; r_lut[3]=0; r_lut[4]=0; r_lut[5]=0; \
            g_lut[0]=0; g_lut[1]=0; g_lut[2]=0; g_lut[3]=0; g_lut[4]=0; g_lut[5]=0;\
            b_lut[0]=0; b_lut[1]=0; b_lut[2]=0; b_lut[3]=0; b_lut[4]=0; b_lut[5]=0;\
                        a_lut[0]=0; a_lut[1]=0; a_lut[2]=0; a_lut[3]=0; a_lut[4]=0; a_lut[5]=0;\
            sc_lut[0]=0; sc_lut[1]=0; sc_lut[2]=0; sc_lut[3]=0; sc_lut[4]=0; sc_lut[5]=0; \
            low_clamp[0] = 0; low_clamp[1] = 0; low_clamp[2] = 0; low_clamp[3] = 0;
		}
		
                  GLfloat r_lut[6];
        GLfloat g_lut[6];
        GLfloat b_lut[6];
                GLfloat a_lut[6];
        GLfloat sc_lut[6];
        GLfloat low_clamp[6];
    };
	struct image_dims{
        image_dims ( float xs, float ys, float zs, float xd, float yd, float zd )
        {
            xsize=xs;
            ysize=ys;
            zsize=zs;
            xdim=xd;
            ydim=yd;
            zdim=zd;		
        }
		
        float xsize,ysize,zsize;
        float xdim,ydim,zdim;
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

  struct vertexFloat
    {
        vertexFloat(float sc_in){
			x = 0;
			y = 0;
			z = 0;
			nx = 0;
			ny = 0;
			nz = 0;
			scalar=sc_in;
		}
        vertexFloat(){
			x = 0;
			y = 0;
			z = 0;
			nx = 0;
			ny = 0;
			nz = 0;
			scalar=0;
		}
        
        float x, y, z;        //Vertex
        float nx, ny, nz;     //Normal
        float scalar;
    };

}
#endif // BRIVIEW_STRUCTS_H
