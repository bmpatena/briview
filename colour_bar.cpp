#include "colour_bar.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <iostream>
#include <sstream>
using namespace std;
using namespace fslsurface_name;
namespace briview{

    ColourBar::ColourBar()
    {
        //     vertexLoc=0;
        //         normalLoc=1;
        //         scalarLoc=2;
        origin = float3(-0.9,-0.9,-1);
        length = 0.8;
        width = 0.1;
        title="ColourBar";
    }
    ColourBar::~ColourBar()
    {

    }

    void ColourBar::renderBitmapString(  float x, float y,float z,void *font, const string & str )
    {
        char *ci = const_cast<char*>( str.c_str() );
        char *c;
        //cout<<"title "<<str<<endl;
        glRasterPos3f(x, y,z);
        for (c=ci; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
            //            glutStrokeCharacter(font, *c);

        }
    }
    void ColourBar::renderBitmapString(  float x, float y,float z,void *font,float & num)
    {
        stringstream ss;
                ss.precision(2);

        ss<<num;
        string stemp;
        ss>>stemp;
//glUseProgram(0);

           glColor4f(1.0,1.0,1.0,1.0);
        char *ci = const_cast<char*>(stemp.c_str());
         char *c;
         unsigned char* clen = (unsigned char*)(stemp.c_str());
  //      for (c=ci; *c != '\0'; c++) {
   //         glutBitmapCharacter(font, *c);
    //        //            glutStrokeCharacter(font, *c);

     //   }
        float width = glutBitmapLength(font,clen);//static_cast<const unsigned char*>(stemp.c_str()));


            GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        glRasterPos3f(x-width/viewport[2], y,z);
//cout<<"width "<<width<<" "<<width/viewport[2]/2.0<<endl;


            for (c=ci; *c != '\0'; c++) {
              glutBitmapCharacter(font, *c);
            //            glutStrokeCharacter(font, *c);

        }


    }
        void ColourBar::renderStrokeString(  float x, float y,float z,void *font,char *string)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_LINE_SMOOTH);
            glLineWidth(2.0);
            char *c;
            //             glRasterPos3f(x, y,z);
            glPushMatrix();
           glLoadIdentity();
            glTranslatef(0,+100,0);//x,y,z);
            for (c=string; *c != '\0'; c++) {
                //  glutBitmapCharacter(font, *c);
                //cout<<"draw chracter"<<endl;
                glutStrokeCharacter(font, *c);
            }
            glPopMatrix();
        }



 //   void ColourBar::setGLSLPrograms(const std::vector<GLuint> & progs)
  //  {
  //      glsl_programs=progs;
   // }
                        void ColourBar::setColourTable(const unsigned int & index, const colour_table & c_table)
                        {
v_c_table[index]=c_table;
                        }

            void ColourBar::addColourBar(GLuint* vbos, const colour_table & c_table)
            {
                vbos_c_table.push_back(vbos);
                v_c_table.push_back(c_table);
                generateColourBar(v_c_table.size()-1, c_table);
            }

void ColourBar::setGeometry( const float3 & origin_in, const float & length_in, const float & width_in )
{
    origin  =   origin_in;
    length  =   length_in;
    width   =   width_in;


}
void ColourBar::setTitle( const string & title_in )
{
    title=title_in;
}


     void ColourBar::generateColourBar(const unsigned int & index, const colour_table & c_table)
     {

         v_c_table[index]=c_table;
         int number_of_entries = 6 ;

        float dx=length/(number_of_entries-1);
        //cout<<"dx "<<dx<<endl;
        vector<vertexFloat>* vertices = new vector<vertexFloat>();
        vector<vertexFloat>* vertices_bg = new vector<vertexFloat>();

        vector<GLuint>* indices = new vector<GLuint>();
        vector<GLuint>* indices_bg = new vector<GLuint>();

//create background
        float bg_length_extra = 0.05;

        float bg_width_extra = 0.1;
//----------------BG------------------//

        vertexFloat vert;
            vert.x = origin.x - bg_length_extra;
            vert.y = origin.y - bg_width_extra ;
            vert.z = origin.z;
            vert.nx = 0;
            vert.ny = 0;
            vert.nz = -1;
            vert.scalar = 0 ;

        vertices_bg->push_back(vert);

       vert.x = origin.x - bg_length_extra;
            vert.y = origin.y + width + bg_width_extra ;
            vert.z = origin.z;
            vert.nx = 0;
            vert.ny = 0;
            vert.nz = -1;
            vert.scalar = 0 ;

            vertices_bg->push_back(vert);

      vert.x = origin.x+length +bg_length_extra ;
            vert.y = origin.y +width +bg_width_extra ;
            vert.z = origin.z;
            vert.nx = 0;
            vert.ny = 0;
            vert.nz = -1;
            vert.scalar = 0 ;

            vertices_bg->push_back(vert);

             vert.x = origin.x+length +bg_length_extra;
            vert.y = origin.y  - bg_width_extra ;
            vert.z = origin.z;
            vert.nx = 0;
            vert.ny = 0;
            vert.nz = -1;
            vert.scalar = 0 ;

            vertices_bg->push_back(vert);

        indices_bg->push_back(0);
        indices_bg->push_back(1);
        indices_bg->push_back(2);
        indices_bg->push_back(0);
        indices_bg->push_back(2);
        indices_bg->push_back(3);

            glBindBuffer(GL_ARRAY_BUFFER, vbos_c_table[index][2]);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertexFloat), &(vertices_bg->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbos_c_table[index][3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &(indices_bg->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    delete vertices_bg;
        delete indices_bg;



        unsigned int N_verts  = (number_of_entries-1) * 4  ;
        int mid=(number_of_entries-1)/2;
        int count=0;
        cout<<"mid "<<mid<<" "<<c_table.sc_lut[mid]<<" "<<c_table.sc_lut[mid+1]<<endl;
        bool ctr_thresh=false;
        if (fabs(c_table.sc_lut[mid]) == fabs(c_table.sc_lut[mid+1]) )
            ctr_thresh=1;

        for (int i=0; i<number_of_entries-1 ; i++)
        {
            cout<<"I "<<i<<" "<<mid<<endl;
         //   if ( i != mid )
            {
                cout<<"enter "<<i<<endl;
             vertexFloat vert;

             vert.x = origin.x + i*dx;
             vert.y = origin.y;
             vert.z = origin.z;
             vert.nx = 0;
             vert.ny = 0;
             vert.nz = -1;
             vert.scalar = c_table.sc_lut[i];
             if ((ctr_thresh)&&(i==mid))
             {
                 if (c_table.sc_lut[i]<0)
                     vert.scalar = c_table.sc_lut[i]+1e-7;
                 else
                     vert.scalar = c_table.sc_lut[i]-1e-7;
                 cout<<"alter scalara "<<vert.scalar<<endl;

             }

                 // if ( i == mid+1 )
            //     vert.scalar = c_table.sc_lut[i]+1e-12;

            cout<<"vertsc "<<vert.scalar<<endl;
             vertices->push_back(vert);

             vert.x = origin.x + i*dx;
             vert.y = origin.y + width;
             vert.z = origin.z;
             vert.nx = 0;
             vert.ny = 0;
             vert.nz = -1;
             vert.scalar = c_table.sc_lut[i];
             if ((ctr_thresh)&&(i==mid))
             {
                 if (c_table.sc_lut[i]<0)
                     vert.scalar = c_table.sc_lut[i]+1e-7;
                 else
                     vert.scalar = c_table.sc_lut[i]-1e-7;
                 cout<<"alter scalara "<<vert.scalar<<endl;

             }
             vertices->push_back(vert);
             cout<<"vertsc "<<vert.scalar<<endl;

             vert.x = origin.x + (i+1)*dx;
             vert.y = origin.y + width;
             vert.z = origin.z;
             vert.nx = 0;
             vert.ny = 0;
             vert.nz = -1;
             vert.scalar = c_table.sc_lut[i+1];
             if ((ctr_thresh)&&(i==mid))
             {
                 if (c_table.sc_lut[i+1]<0)
                     vert.scalar = c_table.sc_lut[i+1]+1e-7;
                 else
                     vert.scalar = c_table.sc_lut[i+1]-1e-7;
                 cout<<"alter scalara "<<vert.scalar<<endl;

             }
             vertices->push_back(vert);
             cout<<"vertsc "<<vert.scalar<<endl;


             vert.x = origin.x +  (i+1)*dx;
             vert.y = origin.y ;
             vert.z = origin.z;
             vert.nx = 0;
             vert.ny = 0;
             vert.nz = -1;
             vert.scalar = c_table.sc_lut[i+1];
             cout<<"mid+1 "<<i<<" "<<mid+1<<" "<<ctr_thresh<<endl;
      //       if ( i == mid-1 )
        //         vert.scalar = c_table.sc_lut[i+1]-1e-12;
             if ((ctr_thresh)&&(i==mid))
             {
                 cout<<"alter scalara"<<endl;
                 if (c_table.sc_lut[i+1]<0)
                     vert.scalar = c_table.sc_lut[i+1]+1e-7;
                 else
                     vert.scalar = c_table.sc_lut[i+1]-1e-7;
                 cout<<"alter scalara "<<vert.scalar<<endl;

             }
             vertices->push_back(vert);
             cout<<"vertsc "<<vert.scalar<<endl;

             indices->push_back(4*count+0);
             indices->push_back(4*count+2);
             indices->push_back(4*count+1);
             indices->push_back(4*count+0);
             indices->push_back(4*count+3);
             indices->push_back(4*count+2);

             ++count;
         }
        }

//--number_of_entries;
//N_verts-=4;
//----------------end BG------------------//
      N_triangles = ( number_of_entries - 1 ) * 2 ;//*4 + 8 ; //last 2 for background

        /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
        glBindBuffer(GL_ARRAY_BUFFER, vbos_c_table[index][0]);
      //  glBufferData(GL_ARRAY_BUFFER, ( 3*N_verts + 2) * sizeof(vertex), &(vertices.at(0)), GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, N_verts * sizeof(vertexFloat), &(vertices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbos_c_table[index][1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, N_triangles*3 * sizeof(unsigned int), &(indices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

         // renderBitmapString(0,0,0, GLUT_BITMAP_HELVETICA_18, "Hello World");

        delete vertices;
        delete indices;


     }

    void ColourBar::renderColourBar()
    {
//cout<<"RenderColourBar "<<vbos_c_table[0][0]<<" "<<vbos_c_table[0][1]<<" "<<endl;//glsl_programs.size()<<endl;
////cout<<"GLSL prg "<<glsl_programs.at(0)<<endl;
         //       glUseProgram(glsl_programs.at(2));
               // float mag = sqrt( surf_mat.diffuse[0] * surf_mat.diffuse[0] + surf_mat.diffuse[1] * surf_mat.diffuse[1] + surf_mat.diffuse[2] * surf_mat.diffuse[2]);
          //      surf_mat.ambient[0]+=surf_mat.diffuse[0];
          //      surf_mat.ambient[1]+=surf_mat.diffuse[1];
          //      surf_mat.ambient[2]+=surf_mat.diffuse[2];
//glFontBegin(&font);
//glScalef(8.0, 8.0, 8.0);
//glTranslatef(30, 30, 0);
//glFontTextOut("Test", 5, 5, 0);
//glFontEnd();
//glFlush();


//GLfloat ambient[4] =  { surf_mat.ambient[0], surf_mat.ambient[1], surf_mat.ambient[2], surf_mat.ambient[3]};
  //               ambient[0]+=surf_mat.diffuse[0];
    //             ambient[1]+=surf_mat.diffuse[1];
      //           ambient[2]+=surf_mat.diffuse[2];
//                 ambient[0]+=surf_mat.diffuse[0];

        //        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
          //      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  surf_mat.diffuse );
           //     glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,surf_mat.shininess);
            //    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, surf_mat.specular);

              //  glUseProgram(1);




                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho( -1.0, 1.0, -1.0, 1.0, 0, 100 );

                glMatrixMode(GL_MODELVIEW);

                glPushMatrix();
                glLoadIdentity();
//---------------BG---------------//
                GLint cur_prog;

                glGetIntegerv(GL_CURRENT_PROGRAM,&cur_prog);
//cout<<"cbar "<<cur_prog<<endl;

    glUseProgram(0);
           glColor4f(0.0,0.0,0.0,1.0);

           glBindBuffer(GL_ARRAY_BUFFER, vbos_c_table[0][2] );         // for vertex coordinates
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_c_table[0][3] );

                glEnableVertexAttribArray(vertexLoc);
                glEnableVertexAttribArray(normalLoc);
               glEnableVertexAttribArray(scalarLoc);
////cout<<"locs "<<vertexLoc<<" "<<normalLoc<<" "<<scalarLoc<<endl;

                glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), 0);
                glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(3*sizeof(float)));
                glVertexAttribPointer(scalarLoc, 1, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(6*sizeof(float)));
                //cout<<"draw cbar "<<N_triangles<<endl;
                glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,0);

                glDisableVertexAttribArray(vertexLoc);
                glDisableVertexAttribArray(normalLoc);
                glDisableVertexAttribArray(scalarLoc);
//---------------BG---------------//
                glUseProgram(cur_prog);

                glBindBuffer(GL_ARRAY_BUFFER, vbos_c_table[0][0] );         // for vertex coordinates
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_c_table[0][1] );

                glEnableVertexAttribArray(vertexLoc);
                glEnableVertexAttribArray(normalLoc);
                glEnableVertexAttribArray(scalarLoc);
////cout<<"locs "<<vertexLoc<<" "<<normalLoc<<" "<<scalarLoc<<endl;

                glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), 0);
                glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(3*sizeof(float)));
                glVertexAttribPointer(scalarLoc, 1, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(6*sizeof(float)));
                //cout<<"draw cbar "<<N_triangles<<endl;
                glDrawElements(GL_TRIANGLES,N_triangles*3,GL_UNSIGNED_INT,0);

                glDisableVertexAttribArray(vertexLoc);
                glDisableVertexAttribArray(normalLoc);
                glDisableVertexAttribArray(scalarLoc);



                glBindBuffer(GL_ARRAY_BUFFER, 0 );         // for vertex coordinates
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );


                glUseProgram(0);

            int number_of_entries = 6 ;
            float dx=length/(number_of_entries-1);


                   renderBitmapString(origin.x, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[0]);
                   renderBitmapString(origin.x + dx, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[1]);
                   renderBitmapString(origin.x + dx*2, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[2]);
                   renderBitmapString(origin.x + dx*3, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[3]);
                   renderBitmapString(origin.x + dx*4, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[4]);
                   renderBitmapString(origin.x + dx*5, origin.y -0.07,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, v_c_table[0].sc_lut[5]);

                   renderBitmapString(origin.x, origin.y + width +0.03,origin.z, GLUT_BITMAP_TIMES_ROMAN_24, title );


                   //renderBitmapString(origin.x, origin.y -0.05,origin.z, GLUT_BITMAP_HELVETICA_18, v_c_table[0].sc_lut[0]);

                   //renderStrokeString(origin.x, origin.y + 2*width ,origin.z, GLUT_STROKE_ROMAN, "Hello World");

                    glPopMatrix();

                    glMatrixMode(GL_PROJECTION);

                 glPopMatrix();

                glMatrixMode(GL_MODELVIEW);
                 // renderStrokeString(origin.x, origin.y + 2*width ,origin.z, GLUT_STROKE_ROMAN, "Hello World");



            }
}
