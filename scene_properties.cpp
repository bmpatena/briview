#include <sstream>
#include <fstream>
#include <iostream>
#include "scene_properties.h"
#include "ui_scene_properties.h"
#include <OpenGL/glu.h>

using namespace std;

using namespace fslsurface_name;
namespace briview{
    scene_properties::scene_properties(QWidget *parent) :
            QDockWidget(parent),
            ui(new Ui::scene_properties_form)
    {
        ui->setupUi(this);
        //Mouse interactions
        connect(ui->sensitivity_slider,SIGNAL(valueChanged(int)),this,SIGNAL(sig_changedMouseSensitivity(int)));
        connect(ui->tx_mode,SIGNAL(released()),this,SLOT(emitMouseMode()));
        connect(ui->scale_mode,SIGNAL(released()),this,SLOT(emitMouseMode()));
        connect(ui->rot_mode_roll,SIGNAL(released()),this,SLOT(emitMouseMode()));
        connect(ui->rot_mode_azi_ele,SIGNAL(released()),this,SLOT(emitMouseMode()));
        connect(ui->rot_mode_pit_yaw,SIGNAL(released()),this,SLOT(emitMouseMode()));
        connect(ui->rot_ortho,SIGNAL(released()),this,SLOT(emitMouseMode()));

   //     connect(ui->next_page,SIGNAL(released()),this,SLOT(nextPage()));
     //   connect(ui->prev_page,SIGNAL(released()),this,SLOT(prevPage()));

        connect( ui->gluLookAt_Set,SIGNAL(pressed()),this,SLOT(setCameraParameters()));
        connect( ui->gluLookAt_Set,SIGNAL(pressed()),this,SIGNAL(sig_updateGL()));

        connect(ui->pos_x, SIGNAL(textEdited(QString)),this,SLOT(set_LightPosition()));
        connect(ui->pos_y, SIGNAL(textEdited(QString)),this,SLOT(set_LightPosition()));
        connect(ui->pos_z, SIGNAL(textEdited(QString)),this,SLOT(set_LightPosition()));

        connect(ui->global_amb_r, SIGNAL(textEdited(QString)),this,SLOT(set_GlobalLightAmbient()));
        connect(ui->global_amb_g, SIGNAL(textEdited(QString)),this,SLOT(set_GlobalLightAmbient()));
        connect(ui->global_amb_b, SIGNAL(textEdited(QString)),this,SLOT(set_GlobalLightAmbient()));
        connect(ui->global_amb_a, SIGNAL(textEdited(QString)),this,SLOT(set_GlobalLightAmbient()));

        connect(ui->bg_color_r, SIGNAL(textEdited(QString)),this,SLOT(set_ClearColor()));
        connect(ui->bg_color_g, SIGNAL(textEdited(QString)),this,SLOT(set_ClearColor()));
        connect(ui->bg_color_b, SIGNAL(textEdited(QString)),this,SLOT(set_ClearColor()));
        connect(ui->bg_color_a, SIGNAL(textEdited(QString)),this,SLOT(set_ClearColor()));
        connect(ui->bg_slider, SIGNAL(sliderMoved(int)),this,SLOT(set_ClearColorMag(int)));



        connect(ui->glob_amb_slider, SIGNAL(sliderMoved(int)),this,SLOT(set_GlobalLightAmbientMag(int)));

        connect(ui->amb_r, SIGNAL(textEdited(QString)),this,SLOT(set_LightAmbient()));
        connect(ui->amb_g, SIGNAL(textEdited(QString)),this,SLOT(set_LightAmbient()));
        connect(ui->amb_b, SIGNAL(textEdited(QString)),this,SLOT(set_LightAmbient()));
        connect(ui->amb_a, SIGNAL(textEdited(QString)),this,SLOT(set_LightAmbient()));

        connect(ui->dif_r, SIGNAL(textEdited(QString)),this,SLOT(set_LightDiffuse()));
        connect(ui->dif_g, SIGNAL(textEdited(QString)),this,SLOT(set_LightDiffuse()));
        connect(ui->dif_b, SIGNAL(textEdited(QString)),this,SLOT(set_LightDiffuse()));
        connect(ui->dif_a, SIGNAL(textEdited(QString)),this,SLOT(set_LightDiffuse()));

        connect(ui->spec_r, SIGNAL(textEdited(QString)),this,SLOT(set_LightSpecular()));
        connect(ui->spec_g, SIGNAL(textEdited(QString)),this,SLOT(set_LightSpecular()));
        connect(ui->spec_b, SIGNAL(textEdited(QString)),this,SLOT(set_LightSpecular()));
        connect(ui->spec_a, SIGNAL(textEdited(QString)),this,SLOT(set_LightSpecular()));

        connect(ui->gluPersp_fovy,SIGNAL(textEdited(QString)),this,SLOT(updatePerspectiveFOVY()));
        connect(ui->gluPersp_aspect,SIGNAL(textEdited(QString)),this,SLOT(updatePerspectiveAspect()));
        connect(ui->gluPersp_near,SIGNAL(textEdited(QString)),this,SLOT(updatePerspectiveNear()));
        connect(ui->gluPersp_far,SIGNAL(textEdited(QString)),this,SLOT(updatePerspectiveFar()));

        connect(ui->disableLight, SIGNAL(toggled(bool)),this,SLOT(en_disableLight(bool)));
        connect(ui->addLight, SIGNAL(clicked()),this,SLOT(addLight()));
        connect(ui->lights_list, SIGNAL( currentIndexChanged ( int )),this,SLOT(changeCurrentLight(int)));

        connect(ui->but_doDockMouse,SIGNAL(clicked()),this,SLOT(doDockMouse()));
        connect(ui->but_doDockCamera,SIGNAL(clicked()),this,SLOT(doDockCamera()));
        connect(ui->but_doDockLights,SIGNAL(clicked()),this,SLOT(doDockLights()));
        connect(ui->but_doDockBackground,SIGNAL(clicked()),this,SLOT(doDockBackground()));

        ///connnect the docking signal with showing the button to re-dock
        connect(ui->dock_mouse_ui,SIGNAL(topLevelChanged(bool)),ui->but_doDockMouse,SLOT(setVisible(bool)));
        connect(ui->dock_lighting_ui,SIGNAL(topLevelChanged(bool)),ui->but_doDockLights,SLOT(setVisible(bool)));
        connect(ui->dock_camera_proj_ui,SIGNAL(topLevelChanged(bool)),ui->but_doDockCamera,SLOT(setVisible(bool)));
        connect(ui->dock_background_ui,SIGNAL(topLevelChanged(bool)),ui->but_doDockBackground,SLOT(setVisible(bool)));

        //-------initialize UI configuration
        ui->but_doDockMouse->hide();
        ui->but_doDockLights->hide();
        ui->but_doDockCamera->hide();
        ui->but_doDockBackground->hide();
        ui->dock_lighting_ui->hide();

        ///----------------


      //  connect( ui->dock_mouse_ui, SIGNAL(topLevelChanged(bool)), this,SLOT(removeMouseFromLayout(bool)));
      //  connect( ui->dock_camera_proj_ui, SIGNAL(topLevelChanged(bool)), this,SLOT(removeCameraFromLayout(bool)));
       // connect( ui->dock_lighting_ui, SIGNAL(topLevelChanged(bool)), this,SLOT(removeLightingFromLayout(bool)));

        eye=float3(0,-100,0);
        center=float3(0,0,0);
        vup=float3(0,0,1);
        gluPersp_params = float4(45.0,1.0,1.0,1000.0);
        Nlights=0;

        addLight();

        global_ambient = new GLfloat[4];
        global_ambient[0]=0.2;
        global_ambient[1]=0.2;
        global_ambient[2]=0.2;
        global_ambient[3]=1.0;

        background_color = new GLfloat[4];
        background_color[0]=0.0;
        background_color[1]=0.0;
        background_color[2]=0.0;
        background_color[3]=0.0;


        set_GlobalLightAmbientText();
        setCameraParameters(eye,center,vup);


        proj_mode=PERSPECTIVE;
      //  ui->dock_lighting_ui->close();
    //    dock_camera_ui_page=1;
  //      dock_lighting_ui_page=2;
//        dock_mouse_ui_page=0;

     //   ui->scene_props_stackedWidget->setCurrentIndex(0);

    }

    scene_properties::~scene_properties()
    {

        for (vector<GLfloat*>::iterator i=ambient.begin();i!=ambient.end();i++)
            delete[] *i;
        for (vector<GLfloat*>::iterator i=diffuse.begin();i!=diffuse.end();i++)
            delete[] *i;
        for (vector<GLfloat*>::iterator i=specular.begin();i!=specular.end();i++)
            delete[] *i;
        for (vector<GLfloat*>::iterator i=position.begin();i!=position.end();i++)
            delete[] *i;
        delete ui;

        delete[] global_ambient;
        delete[] background_color;
    }

    void  scene_properties::writeSceneProperties(   ofstream * fout)
    {
        *fout<<"scene"<<endl;
        *fout<<"camera"<<endl;
        *fout<<eye.x<<" "<<eye.y<<" "<<eye.z<<" ";
        *fout<<center.x<<" "<<center.y<<" "<<center.z<<" ";
        *fout<<vup.x<<" "<<vup.y<<" "<<vup.z<<endl;

        //use 0 for perspective, incase want to remember in future
        *fout<<"projection "<<proj_mode<<endl;
        *fout<<gluPersp_params.x<<" "<<gluPersp_params.y<<" "<<gluPersp_params.z<<" "<<gluPersp_params.w<<endl;
        *fout<<"globalAmbient "<<global_ambient[0]<<" "<<global_ambient[1]<<" "<<global_ambient[2]<<" "<<global_ambient[3]<<endl;
        *fout<<"lights "<<Nlights<<endl;
        for (int i=0;i<Nlights;i++)
        {
            *fout<<ambient.at(i)[0]<<" "<<ambient.at(i)[1]<<" "<<ambient.at(i)[2]<<" "<<ambient.at(i)[3]<<endl;
            *fout<<diffuse.at(i)[0]<<" "<<diffuse.at(i)[1]<<" "<<diffuse.at(i)[2]<<" "<<diffuse.at(i)[3]<<endl;
            *fout<<specular.at(i)[0]<<" "<<specular.at(i)[1]<<" "<<specular.at(i)[2]<<" "<<specular.at(i)[3]<<endl;
            *fout<<ambient.at(i)[0]<<" "<<position.at(i)[1]<<" "<<position.at(i)[2]<<" "<<position.at(i)[3]<<endl;

        }


    }

    void  scene_properties::readSceneProperties( ifstream * fin)
    {
        cout<<"reading scene properties "<<endl;
        //max 256 characters on comment lines
        char line[256];
        fin->getline(line,256);//scene
        fin->getline(line,256);//camera
        *fin>>eye.x>>eye.y>>eye.z;
        *fin>>center.x>>center.y>>center.z;
        *fin>>vup.x>>vup.y>>vup.z;
        cout<<"eye "<<eye.x<<" "<<eye.y<<" "<<eye.z<<endl;
        //use 0 for perspective, incase want to remember in future
        int proj_mode_temp;
        *fin>>proj_mode_temp;//projection
        proj_mode=static_cast<PROJECTION_METHOD>(proj_mode_temp);
        *fin>>gluPersp_params.x>>gluPersp_params.y>>gluPersp_params.z>>gluPersp_params.w;
        *fin>>global_ambient[0]>>global_ambient[1]>>global_ambient[2]>>global_ambient[3];
        *fin>>Nlights;
        for (int i=0;i<Nlights;i++)
        {
            *fin>>ambient.at(i)[0]>>ambient.at(i)[1]>>ambient.at(i)[2]>>ambient.at(i)[3];
            *fin>>diffuse.at(i)[0]>>diffuse.at(i)[1]>>diffuse.at(i)[2]>>diffuse.at(i)[3];
            *fin>>specular.at(i)[0]>>specular.at(i)[1]>>specular.at(i)[2]>>specular.at(i)[3];
            *fin>>ambient.at(i)[0]>>position.at(i)[1]>>position.at(i)[2]>>position.at(i)[3];

        }




    }



    void scene_properties::changeEvent(QEvent *e)
    {
        QDockWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }


    void scene_properties::setCameraCenter(const float3 & cog)
    {
        QString temp;
        temp.setNum(cog.x,'g',4);
        ui->gluCenterX->setText(temp);
        temp.setNum(cog.y,'g',4);
        ui->gluCenterY->setText(temp);
        temp.setNum(cog.z,'g',4);
        ui->gluCenterZ->setText(temp);

        center=cog;

    }

    void scene_properties::setCameraEye(const float3 & eye_in)
    {
        QString temp;
        temp.setNum(eye_in.x,'g',4);
        ui->gluEyeX->setText(temp);
        temp.setNum(eye_in.y,'g',4);
        ui->gluEyeY->setText(temp);
        temp.setNum(eye_in.z,'g',4);
        ui->gluEyeZ->setText(temp);

        eye=eye_in;

    }


    void scene_properties::showDockMouse()
    {
        ui->dock_mouse_ui->show();
        ui->dock_mouse_ui->setFloating(true);
    }
    void scene_properties::showDockLights()
    {
       // cout<<"show dock lights"<<endl;
        ui->dock_lighting_ui->show();
        ui->dock_lighting_ui->setFloating(true);
    }
    void scene_properties::showDockCamera()
    {
        ui->dock_camera_proj_ui->show();
        ui->dock_camera_proj_ui->setFloating(true);
    }
    void scene_properties::showDockBackground()
    {
        ui->dock_background_ui->show();
        ui->dock_background_ui->setFloating(true);
    }
    void scene_properties::doDockMouse()
    {
        ui->dock_mouse_ui->setFloating(false);
      //  ui->but_doDockMouse->hide();
    }

    void scene_properties::doDockCamera()
    {
        ui->dock_camera_proj_ui->setFloating(false);
    }

    void scene_properties::doDockLights()
    {
        ui->dock_lighting_ui->setFloating(false);
    }
    void scene_properties::doDockBackground()
    {
        ui->dock_background_ui->setFloating(false);
    }

  /*  void scene_properties::removeMouseFromLayout(bool floating)
    {
        if (floating)
        {
       //     ui->scene_props_stackedWidget->removeWidget( ui->scene_props_stackedWidget->widget(dock_mouse_ui_page));
         //   updatePageText();
        }
    }
    void scene_properties::removeCameraFromLayout(bool floating)
    {
        if (floating)
        {
       //     ui->scene_props_stackedWidget->removeWidget( ui->scene_props_stackedWidget->widget(dock_camera_ui_page));
           // updatePageText();
        }
    }

    void scene_properties::removeLightingFromLayout(bool floating)
    {
        if (floating)
        {
         //   ui->scene_props_stackedWidget->removeWidget( ui->scene_props_stackedWidget->widget(dock_lighting_ui_page));
  //          updatePageText();
        }
    }*/
/*
    void scene_properties::updatePageText()
    {
        int cur=ui->scene_props_stackedWidget->currentIndex()-1;

        QString temp,temp2;
        temp.setNum(cur+2);
        temp2.setNum(ui->scene_props_stackedWidget->count());
        ui->scene_page_label->setText("page "+temp+"/"+temp2);
    }

    void scene_properties::nextPage()
    {
        int cur=ui->scene_props_stackedWidget->currentIndex();
        if ( cur <= (ui->scene_props_stackedWidget->count()-2) )
        {
            ui->scene_props_stackedWidget->setCurrentIndex(cur+1);
            updatePageText();
        }
    }
    void scene_properties::prevPage()
    {
        int cur=ui->scene_props_stackedWidget->currentIndex();
        if ( cur >= 1 )
        {
            ui->scene_props_stackedWidget->setCurrentIndex(cur-1);
            updatePageText();
        }
    }
*/


    //*****************************MOUSE***********************************//
            void scene_properties::emitMouseMode()
    {
        if (ui->tx_mode->isChecked())
            emit sig_changedMouseMode(0);
        else if (ui->scale_mode->isChecked())
            emit sig_changedMouseMode(1);
        else if (ui->rot_mode_roll->isChecked())
            emit sig_changedMouseMode(2);
        else if (ui->rot_mode_azi_ele->isChecked())
            emit sig_changedMouseMode(3);
        else if (ui->rot_mode_pit_yaw->isChecked())
            emit sig_changedMouseMode(4);
        else if (ui->rot_ortho->isChecked())
            emit sig_changedMouseMode(5);

    }

    //*****************************END MOUSE***********************************//

            //*****************************LIGHTING***********************************//
            void scene_properties::addLight()
    {
        //setup light0
        stringstream ss;
        ss<<ui->lights_list->count();
        string num;
        ss>>num;

        GLfloat* pos0 = new GLfloat[4];
        GLfloat* dif0 = new GLfloat[4];
        GLfloat* spec0 = new GLfloat[4];
        GLfloat* amb0 = new GLfloat[4];
        float mag=1.0;///sqrt(3);
        dif0[0]=mag;
        dif0[1]=mag;
        dif0[2]=mag;
        dif0[3]=1.0;

        spec0[0]=mag;
        spec0[1]=mag;
        spec0[2]=mag;
        spec0[3]=1.0;
        //  *spec0=*dif0;

        amb0[0]=mag;
        amb0[1]=mag;
        amb0[2]=mag;
        amb0[3]=1.0;

        pos0[0]=0.0;
        pos0[1]=0.0;
        pos0[2]=-1.0;
        pos0[3]=0.0;


        ambient.push_back(amb0);
        diffuse.push_back(dif0);
        specular.push_back(spec0);
        position.push_back(pos0);
        enabled.push_back(true);
        Nlights++;

        ui->lights_list->addItem(QString(("GL_LIGHT"+num).c_str()));

    }
    //Lighting
    void scene_properties::changeCurrentLight(int index)
    {
        set_LightDiffuseText(index);
        set_LightAmbientText(index);
        set_LightSpecularText(index);
        set_LightPositionText(index);
    }

    void scene_properties::setCameraOrientation()
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
   //     glScalef (-1., 1., 1.);

        gluLookAt(eye.x,eye.y,eye.z,center.x,center.y,center.z,vup.x,vup.y,vup.z);
    }

    void scene_properties::setAspectRatio()
    {
        gluPersp_params.y = window_size.x/window_size.y;
        QString temp;
        temp.setNum(gluPersp_params.y,'g',4);
        ui->gluPersp_aspect->setText(temp);
    }

    void scene_properties::setWindowSize(const int & w, const int & h)
    {
        window_size=int2(w,h);
    }

    void scene_properties::setProjectionProperties()
    {
        if (proj_mode == PERSPECTIVE)
        {//allow for orth projections in future
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective( gluPersp_params.x,gluPersp_params.y,gluPersp_params.z,gluPersp_params.w );
           glMatrixMode(GL_MODELVIEW);
        }
    }


    void scene_properties::setLights()
    {
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

        for (int i=0;i<ui->lights_list->count();i++)
        {
            if  (enabled.at(i))
            {
                glLightfv(GL_LIGHT0+i, GL_POSITION , position.at(i) );
                glLightfv(GL_LIGHT0+i, GL_AMBIENT  , ambient.at(i)  );
                glLightfv(GL_LIGHT0+i, GL_DIFFUSE  , diffuse.at(i)  );
                glLightfv(GL_LIGHT0+i, GL_SPECULAR , specular.at(i) );
                glEnable(GL_LIGHT0+i);
            }else
            {
                glDisable(GL_LIGHT0+i);
            }
        }
    }

    void scene_properties::set_LightPosition()
    {
        position.at( ui->lights_list->currentIndex() )[0]=ui->pos_x->text().toFloat();
        position.at( ui->lights_list->currentIndex() )[1]=ui->pos_y->text().toFloat();
        position.at( ui->lights_list->currentIndex() )[2]=ui->pos_z->text().toFloat();
        position.at( ui->lights_list->currentIndex() )[3]=ui->pos_w->text().toFloat();

        emit sig_updateGL();
    }

    void scene_properties::set_LightAmbient()
    {
        ambient.at( ui->lights_list->currentIndex() )[0]=ui->amb_r->text().toFloat();
        ambient.at( ui->lights_list->currentIndex() )[1]=ui->amb_g->text().toFloat();
        ambient.at( ui->lights_list->currentIndex() )[2]=ui->amb_b->text().toFloat();
        ambient.at( ui->lights_list->currentIndex() )[3]=ui->amb_a->text().toFloat();

        emit sig_updateGL();
    }

    void scene_properties::set_GlobalLightAmbient()
    {
        global_ambient[0]=ui->global_amb_r->text().toFloat();
        global_ambient[1]=ui->global_amb_g->text().toFloat();
        global_ambient[2]=ui->global_amb_b->text().toFloat();
        global_ambient[3]=ui->global_amb_a->text().toFloat();

        emit scene_properties::sig_updateGL();
    }
    void scene_properties::set_ClearColor()
    {
     //   //cout<<"set cleacolor"<<endl;
        background_color[0]=ui->bg_color_r->text().toFloat();
        background_color[1]=ui->bg_color_g->text().toFloat();
        background_color[2]=ui->bg_color_b->text().toFloat();
        background_color[3]=ui->bg_color_a->text().toFloat();
         ui->bg_slider->setValue(static_cast<int>(sqrt(background_color[0]*background_color[0] + background_color[1]*background_color[1]  + background_color[2]*background_color[2]) *  ui->bg_slider->maximum()  ));

        glClearColor(background_color[0],background_color[1],background_color[2],background_color[3]);



        emit scene_properties::sig_updateGL();

    }
    void scene_properties::set_ClearColorMag( int mag )
    {
         //       //cout<<"set cleacolor mag"<<endl;

        float mag_init = sqrt(background_color[0]*background_color[0] + background_color[1]*background_color[1]  + background_color[2]*background_color[2] );

        float mag_f = static_cast<float>(mag) / ui->bg_slider->maximum();

        QString temp;
        if (mag_init != 0)
        {

            mag_f/=mag_init;
            temp.setNum(  background_color[0] *mag_f ,'g',3);
            ui->bg_color_r->setText(temp);

            temp.setNum(  background_color[1] *mag_f ,'g',3);
            ui->bg_color_g->setText(temp);

            temp.setNum(  background_color[2] *mag_f ,'g',3);
            ui->bg_color_b->setText(temp);


        }else{
            temp.setNum( mag_f ,'g',3);
            ui->bg_color_r->setText(temp);

            //temp.setNum(  mag_f ,'g',3);
            ui->bg_color_g->setText(temp);

            //temp.setNum( mag_f ,'g',3);
            ui->bg_color_b->setText(temp);

        }

         set_ClearColor();

    }



    void scene_properties::set_GlobalLightAmbientMag(int val)
    {
        float r=ui->global_amb_r->text().toFloat();
        float g=ui->global_amb_g->text().toFloat();
        float b=ui->global_amb_b->text().toFloat();
        float mag=static_cast<float>(val)/ui->glob_amb_slider->maximum();
        float mag_cur=sqrt(r*r+g*g+b*b);
        float sc=mag/mag_cur;

        if (mag_cur==0)
        {
            r=1;
            g=1;
            b=1;
            sc=mag;
        }

        QString temp;
        temp.setNum(r*sc,'g',3);
        ui->global_amb_r->setText(temp);
        temp.setNum(g*sc,'g',3);
        ui->global_amb_g->setText(temp);
        temp.setNum(b*sc,'g',3);
        ui->global_amb_b->setText(temp);

        set_GlobalLightAmbient();
    }


    void scene_properties::updatePerspectiveFOVY()
    {
        gluPersp_params.x = ui->gluPersp_fovy->text().toFloat();
        setProjectionProperties();
        emit sig_updateGL();
    }
    void scene_properties::updatePerspectiveAspect()
    {
        gluPersp_params.y = ui->gluPersp_aspect->text().toFloat();
        setProjectionProperties();
        emit sig_updateGL();
    }
    void scene_properties::updatePerspectiveNear()
    {
        gluPersp_params.z = ui->gluPersp_near->text().toFloat();
        setProjectionProperties();
        emit sig_updateGL();
    }
    void scene_properties::updatePerspectiveFar()
    {
        gluPersp_params.w = ui->gluPersp_far->text().toFloat();
        setProjectionProperties();
        emit sig_updateGL();
    }
    void scene_properties::set_LightDiffuse()
    {
        diffuse.at( ui->lights_list->currentIndex() )[0]=ui->dif_r->text().toFloat();
        diffuse.at( ui->lights_list->currentIndex() )[1]=ui->dif_g->text().toFloat();
        diffuse.at( ui->lights_list->currentIndex() )[2]=ui->dif_b->text().toFloat();
        diffuse.at( ui->lights_list->currentIndex() )[3]=ui->dif_a->text().toFloat();

        emit sig_updateGL();


    }
    void scene_properties::set_LightSpecular()
    {
        specular.at( ui->lights_list->currentIndex() )[0]=ui->spec_r->text().toFloat();
        specular.at( ui->lights_list->currentIndex() )[1]=ui->spec_g->text().toFloat();
        specular.at( ui->lights_list->currentIndex() )[2]=ui->spec_b->text().toFloat();
        specular.at( ui->lights_list->currentIndex() )[3]=ui->spec_a->text().toFloat();

        emit sig_updateGL();

    }

    void scene_properties::set_LightPositionText( const int & index )
    {
        QString temp;

        temp.setNum( position.at( index )[0],'g',4);
        ui->pos_x->setText(temp);
        temp.setNum( position.at( index )[1],'g',4);
        ui->pos_y->setText(temp);
        temp.setNum( position.at( index )[2],'g',4);
        ui->pos_z->setText(temp);
        temp.setNum( position.at( index )[3],'g',4);
        ui->pos_w->setText(temp);
    }

    void scene_properties::set_LightAmbientText( const int & index )
    {
        QString temp;

        temp.setNum( ambient.at( index )[0],'g',4);
        ui->amb_r->setText(temp);
        temp.setNum( ambient.at( index )[1],'g',4);
        ui->amb_g->setText(temp);
        temp.setNum( ambient.at( index )[2],'g',4);
        ui->amb_b->setText(temp);
        temp.setNum( ambient.at( index )[3],'g',4);
        ui->amb_a->setText(temp);
    }
    void scene_properties::set_GlobalLightAmbientText( )
    {
        QString temp;

        temp.setNum( global_ambient[0],'g',4);
        ui->global_amb_r->setText(temp);
        temp.setNum( global_ambient[1],'g',4);
        ui->global_amb_g->setText(temp);
        temp.setNum( global_ambient[2],'g',4);
        ui->global_amb_b->setText(temp);
        temp.setNum( global_ambient[3],'g',4);
        ui->global_amb_a->setText(temp);
        float mag=sqrt(global_ambient[0]*global_ambient[0]+global_ambient[1]*global_ambient[1]+global_ambient[2]*global_ambient[2]);
        ui->glob_amb_slider->setSliderPosition(mag*ui->glob_amb_slider->maximum());
    }

    void scene_properties::set_LightDiffuseText( const int & index )
    {
        QString temp;

        temp.setNum( diffuse.at( index )[0],'g',4);
        ui->dif_r->setText(temp);
        temp.setNum( diffuse.at( index )[1],'g',4);
        ui->dif_g->setText(temp);
        temp.setNum( diffuse.at( index )[2],'g',4);
        ui->dif_b->setText(temp);
        temp.setNum( diffuse.at( index )[3],'g',4);
        ui->dif_a->setText(temp);
    }

    void scene_properties::set_LightSpecularText( const int & index )
    {
        QString temp;

        temp.setNum( specular.at( index )[0],'g',4);
        ui->spec_r->setText(temp);
        temp.setNum( specular.at( index )[1],'g',4);
        ui->spec_g->setText(temp);
        temp.setNum( specular.at( index )[2],'g',4);
        ui->spec_b->setText(temp);
        temp.setNum( specular.at( index )[3],'g',4);
        ui->spec_a->setText(temp);
    }

    void scene_properties::en_disableLight(bool state ){
        enabled.at(ui->lights_list->currentIndex())=state;
    }

    //*****************************END LIGHTING***********************************//
            //*****************************CAMERA PROJECTIONS***********************************//

     void  scene_properties::getCameraParameters(float3 & eye_in, float3 & centre_in, float3 & vup_in)
    {
        eye_in=eye;
        centre_in=center;
        vup_in=vup;

    }

    void scene_properties::setCameraParameters()
    {
        eye=float3(ui->gluEyeX->text().toFloat(),ui->gluEyeY->text().toFloat(),ui->gluEyeZ->text().toFloat());
        center=float3( ui->gluCenterX->text().toFloat(), ui->gluCenterY->text().toFloat(), ui->gluCenterZ->text().toFloat());
        vup=float3(ui->gluUpX->text().toFloat(),ui->gluUpY->text().toFloat(),ui->gluUpZ->text().toFloat());
    }
    void scene_properties::setCameraParameters(const float3 & eye_in, const float3 & centre_in, const float3 & vup_in)
    {
        QString temp;

        temp.setNum(eye_in.x,'g',4);
        ui->gluEyeX->setText(temp);
        temp.setNum(eye_in.y,'g',4);
        ui->gluEyeY->setText(temp);
        temp.setNum(eye_in.z,'g',4);
        ui->gluEyeZ->setText(temp);
        temp.setNum(centre_in.x,'g',4);
        ui->gluCenterX->setText(temp);
        temp.setNum(centre_in.y,'g',4);
        ui->gluCenterY->setText(temp);
        temp.setNum(centre_in.z,'g',4);
        ui->gluCenterZ->setText(temp);

        temp.setNum(vup_in.x,'g',4);
        ui->gluUpX->setText(temp);
        temp.setNum(vup_in.y,'g',4);
        ui->gluUpY->setText(temp);
        temp.setNum(vup_in.z,'g',4);
        ui->gluUpZ->setText(temp);

        eye=eye_in;
        center=centre_in;
        vup=vup_in;
    }

    void scene_properties::set_gluPersp_params(const float4 & gluPersp_params)
    {
        QString temp;

        temp.setNum(gluPersp_params.x,'g',4);
        ui->gluPersp_fovy->setText(temp);
        temp.setNum(gluPersp_params.y,'g',4);
        ui->gluPersp_aspect->setText(temp);
        temp.setNum(gluPersp_params.z,'g',4);
        ui->gluPersp_near->setText(temp);
        temp.setNum(gluPersp_params.w,'g',4);
        ui->gluPersp_far->setText(temp);

    }

    void  scene_properties::get_gluPersp_params(float4 & gluPersp_params)
    {
        gluPersp_params.x    = ui->gluPersp_fovy->text().toFloat();
        gluPersp_params.y    = ui->gluPersp_aspect->text().toFloat();
        gluPersp_params.z    = ui->gluPersp_near->text().toFloat();
        gluPersp_params.w = ui->gluPersp_far->text().toFloat();
    }
    //*****************************END CAMERA PROJECTIONS***********************************//
        }
