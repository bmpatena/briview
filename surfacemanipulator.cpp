#include "surfacemanipulator.h"
#include "ui_surfacemanipulator.h"
#include <iostream>
using namespace briview;
#include <cmath>
using namespace fslsurface_name;
using namespace  std;
SurfaceManipulator::SurfaceManipulator(QWidget *parent) :
        QDockWidget(parent),
        ui(new Ui::SurfaceManipulator_form)
{
    ui->setupUi(this);

    max_shininess=150;



    //    connect(ui->next_page,SIGNAL(released()),this,SLOT(nextPage()));
    //   connect(ui->prev_page,SIGNAL(released()),this,SLOT(prevPage()));

    //surfselector
    connect(ui->but_AppendSurfData, SIGNAL(released()),this,SIGNAL(sig_appendSurfaceData()));
    connect(ui->but_viewScData, SIGNAL(released()),this,SIGNAL(sig_changedScalarData()) );
    //glyph characteristic
    connect(ui->glyph_tip_radius, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    connect(ui->glyph_tip_height, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    connect(ui->glyph_base_radius, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    //  connect(ui->glyph_base_height, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    connect(ui->glyph_scale, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    connect(ui->glyph_N_faces, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));
    connect(ui->glyph_opacity, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedGlyphProperties()));


    connect(ui->combo_cmap, SIGNAL(currentIndexChanged (int) ),this,SIGNAL(sig_changedColourTable(int)));

    connect(ui->con_dec  ,SIGNAL(pressed()),this,SLOT(decreaseContrast()));
    connect(ui->con_inc  ,SIGNAL(pressed()),this,SLOT(increaseContrast()));
    connect(ui->level_dec  ,SIGNAL(pressed()),this,SLOT(decreaseLevel()));
    connect(ui->level_inc  ,SIGNAL(pressed()),this,SLOT(increaseLevel()));
    //surface material characteristics
    connect(ui->ambient_r, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->ambient_g, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->ambient_b, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->ambient_a, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->ambient_slider, SIGNAL(sliderMoved(int)),this,SLOT(setAmbientMag(int)));

    connect(ui->diffuse_r, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->diffuse_g, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->diffuse_b, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->diffuse_a, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->diffuse_slider, SIGNAL(sliderMoved(int)),this,SLOT(setDiffuseMag(int)));

    connect(ui->specular_r, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->specular_g, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->specular_b, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->specular_a, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->specular_slider, SIGNAL(sliderMoved(int)),this,SLOT(setSpecularMag(int)));

    connect(ui->shininess, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->shininess_slider, SIGNAL(sliderMoved(int)),this,SLOT(setShininess(int)));

    connect(ui->opacity, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedSurfaceProperties()));
    connect(ui->opacity_slider, SIGNAL(sliderMoved(int)),this,SLOT(setOpacity(int)));

    connect(ui->surface_list, SIGNAL(currentRowChanged (int) ),this,SIGNAL(sig_changedCurrentSurface(int)));
    connect(ui->cbox_surf_shaders , SIGNAL(currentIndexChanged (int) ),this,SLOT(changeShader(int)));

    connect(ui->surf_app_data, SIGNAL(currentRowChanged (int)),this,SIGNAL(sig_updateAppendedSurfaceInfo(int)));
    connect(ui->PlayScalars,SIGNAL(released()),this, SIGNAL(sig_playSurfaceScalars()));
    connect(ui->scalar_prev ,SIGNAL(pressed()),this, SIGNAL(sig_prevSurfaceScalars()));
    connect(ui->scalar_next, SIGNAL(pressed()),this, SIGNAL(sig_nextSurfaceScalars()));

    //colour table

    connect(ui->ctab_sc1, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));
    connect(ui->ctab_sc2, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));
    connect(ui->ctab_sc3, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));
    connect(ui->ctab_sc4, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));
    connect(ui->ctab_sc5, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));
    connect(ui->ctab_sc6, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableSc()));

    connect(ui->ctab_r1, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_r2, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_r3, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_r4, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_r5, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_r6, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));

    connect(ui->ctab_g1, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g2, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g3, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g4, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g5, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g6, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));

    connect(ui->ctab_b1, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b2, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b3, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b4, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b5, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b6, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));

    connect(ui->ctab_a1, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a2, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a3, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a4, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a5, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a6, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));

    connect(ui->ctab_r_lclamp, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_g_lclamp, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_b_lclamp, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));
    connect(ui->ctab_a_lclamp, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTable()));

//    connect(ui->radio_useScalars, SIGNAL(clicked(bool)),this,SIGNAL(sig_changeShaderProgram(bool)));
//connect(ui->cbox_surf_shaders, SIGNAL(currentIndexChanged(int)),this,SIGNAL(sig_changeShaderProgram(int)));
    connect(ui->combo_preset_materials, SIGNAL(currentIndexChanged(int)),this,SIGNAL(sig_changeSurfaceMaterial(int)));

    //--------------------------Colour Bar------------------------------------
    connect(ui->cbar_origin_x, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_origin_y, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_origin_z, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_length, SIGNAL(textChanged(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_width, SIGNAL(textChanged(QString)),this,SIGNAL(sig_changedColourBar()));

    connect(ui->cbar_width_dec, SIGNAL(pressed()),this,SLOT(cbar_width_dec()));
    connect(ui->cbar_width_inc, SIGNAL(pressed()),this,SLOT(cbar_width_inc()));
    connect(ui->cbar_length_dec, SIGNAL(pressed()),this,SLOT(cbar_length_dec()));
    connect(ui->cbar_length_inc, SIGNAL(pressed()),this,SLOT(cbar_length_inc()));

    connect(ui->cbar_title, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->surf_colour_bar_onoff, SIGNAL(released()),this,SIGNAL(sig_changedColourBar()));

    // ui->surface_props_stackedWidget->setCurrentIndex(0);


    connect(ui->polyMode_f, SIGNAL(activated( int )),this,SIGNAL(sig_changedPolygonModeFront(int)));
    connect(ui->polyMode_b, SIGNAL(activated( int )),this,SIGNAL(sig_changedPolygonModeBack(int)));

    connect(ui->polyMode_f_glyph, SIGNAL(activated( int )),this,SIGNAL(sig_changedPolygonModeFrontGlyph(int)));
    connect(ui->polyMode_b_glyph, SIGNAL(activated( int )),this,SIGNAL(sig_changedPolygonModeBackGlyph(int)));

    connect(ui->dispGlyphs,SIGNAL(clicked ( bool) ), this,SIGNAL(sig_displayGlyphs(bool)));
    connect(ui->culling,SIGNAL(clicked(bool)),this, SIGNAL(sig_enableCullFace(bool)));
    connect(ui->ff_ccw,SIGNAL(clicked(bool)),this, SIGNAL(sig_ff_ccw(bool)));


    connect(ui->combo_opacity, SIGNAL(activated (int) ),this,SIGNAL(sig_changedOpacityMode(int)));
   connect(ui->combo_blend, SIGNAL(activated (int) ),this,SIGNAL(sig_changedBlendFunc(int)));


    ///----------------

    connect(ui->but_doDockSurfaceList,SIGNAL(clicked()),this,SLOT(doDockSurfaceSelector()));
    connect(ui->but_doDockPolygon,SIGNAL(clicked()),this,SLOT(doDockPolygonMode()));
    connect(ui->but_doDockMaterial,SIGNAL(clicked()),this,SLOT(doDockMaterialProperties()));
    connect(ui->but_doDockColourMap,SIGNAL(clicked()),this,SLOT(doDockColourMap()));
    connect(ui->but_doDockGlyphs,SIGNAL(clicked()),this,SLOT(doDockGlyphs()));
    connect(ui->but_doDockColourBar,SIGNAL(clicked()),this,SLOT(doDockColourBar()));

    ///connnect the docking signal with showing the button to re-dock
    connect(ui->dock_surface_list,SIGNAL(topLevelChanged(bool)),ui->but_doDockSurfaceList,SLOT(setVisible(bool)));
    connect(ui->dockPolygons,SIGNAL(topLevelChanged(bool)),ui->but_doDockPolygon,SLOT(setVisible(bool)));
    connect(ui->dockMaterial,SIGNAL(topLevelChanged(bool)),ui->but_doDockMaterial,SLOT(setVisible(bool)));
    connect(ui->dockColourMap,SIGNAL(topLevelChanged(bool)),ui->but_doDockColourMap,SLOT(setVisible(bool)));
    connect(ui->dock_surf_glyphs,SIGNAL(topLevelChanged(bool)),ui->but_doDockGlyphs,SLOT(setVisible(bool)));
    connect(ui->dock_surf_colour_bar,SIGNAL(topLevelChanged(bool)),ui->but_doDockColourBar,SLOT(setVisible(bool)));
    //-------initialize UI configuration
    ui->but_doDockSurfaceList->hide();
    ui->but_doDockPolygon->hide();
    ui->but_doDockMaterial->hide();
    ui->but_doDockColourMap->hide();
    ui->but_doDockGlyphs->hide();
    ui->but_doDockColourBar->hide();

    ///----------------
    ui->surface_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //deafilts
    ui->radio_useScalars->setChecked(true);

}

void SurfaceManipulator::setShaders(const std::vector< pair<QString,GLuint> > & v_shaders)
{
cout<<"ste shaders "<<v_shaders.size()<<endl;
// ui->cbox_shaders->clear();
int index=0;
    for ( vector< pair<QString,GLuint> >::const_iterator i = v_shaders.begin(); i != v_shaders.end(); ++i, ++index )
    {
        cout<<"set.. "<<i->first.toStdString()<<" "<<ui->cbox_surf_shaders->count()<<endl;
     ui->cbox_surf_shaders->setItemText(index, i->first);
     //cout<<"set.. "<<i->first.toStdString()<<" "<<ui->cbox_surf_shaders->count()<<endl;
      //  ui->cbox_surf_shaders->addItem("BLH");//i->first);
        //ui->cbox_surf_shaders->setDisabled(false);

       // ui->cbox_shaders->insertItem(0,QString("BLH"),QString("BLH"));//i->first);

    }
    cout<<"doneste shaders"<<endl;

}


void SurfaceManipulator::setCullFace(const bool & cull )
{
    // cout<<"setcull "<<cull<<endl;
    ui->culling->setChecked(cull);
}

void SurfaceManipulator::setFrontFace( const bool & CCW )
{
    // cout<<"set CCW "<<CCW<<endl;
    ui->ff_ccw->setChecked(CCW);
    ui->ff_cw->setChecked(!CCW);
}
void SurfaceManipulator::setPolyMode( const GLenum & front, const GLenum & back)
{
   // cout<<"set poly mode "<<front<<" "<<back<<" "<<GL_POINT<<" "<<GL_LINE<<" "<<GL_FILL<<endl;
    int find=-1;
    if ( front == GL_POINT)
        find=0;
    else  if ( front == GL_LINE)
        find=1;
    else  if ( front == GL_FILL)
        find=2;

 ui->polyMode_f->setCurrentIndex(find);
 int bind=-1;
    if ( back == GL_POINT)
        bind=0;
    else  if ( back == GL_LINE)
        bind=1;
    else  if ( back == GL_FILL)
        bind=2;
    ui->polyMode_b->setCurrentIndex(bind);
}

void SurfaceManipulator::setPolyModeGlyph( const GLenum & front, const GLenum & back)
{
    int find=-1;
    if ( front == GL_POINT)
        find=0;
    else  if ( front == GL_LINE)
        find=1;
    else  if ( front == GL_FILL)
        find=2;

    ui->polyMode_f_glyph->setCurrentIndex(find);
    int bind=-1;
    if ( back == GL_POINT)
        bind=0;
    else  if ( back == GL_LINE)
        bind=1;
    else  if ( back == GL_FILL)
        bind=2;
    ui->polyMode_b_glyph->setCurrentIndex(bind);
}

void SurfaceManipulator::setColourTableIndex( const int & index)
{
    ui->combo_cmap->setCurrentIndex(index);
}

    void SurfaceManipulator::getColourBarGeometry(briview::float3 & origin, float & length, float & width )
    {
        origin = briview::float3(ui->cbar_origin_x->text().toFloat(), ui->cbar_origin_y->text().toFloat(), ui->cbar_origin_z->text().toFloat());
        length = ui->cbar_length->text().toFloat();
        width = ui->cbar_width->text().toFloat();

    }

        string SurfaceManipulator::getColourBarTitle()
        {
            return ui->cbar_title->text().toStdString();
        }

    bool SurfaceManipulator::getColourBarVisibility()
    {
           return (ui->surf_colour_bar_onoff->isChecked());
    }


    void SurfaceManipulator::cbar_width_dec()
    {
        float width = ui->cbar_width->text().toFloat();
        QString temp;
        temp.setNum(width-0.1);
        ui->cbar_width->setText(temp);
    }
    void SurfaceManipulator::cbar_width_inc()
    {
        float width = ui->cbar_width->text().toFloat();
        QString temp;
        temp.setNum(width+0.1);
        ui->cbar_width->setText(temp);
    }
    void SurfaceManipulator::cbar_length_dec()
    {
         float length = ui->cbar_length->text().toFloat();
        QString temp;
        temp.setNum(length-0.1);
        ui->cbar_length->setText(temp);
    }
    void SurfaceManipulator::cbar_length_inc()
    {
         float length = ui->cbar_length->text().toFloat();
        QString temp;
        temp.setNum(length+0.1);
        ui->cbar_length->setText(temp);
    }
    void SurfaceManipulator::setColourTableNoSc( const colour_table & ctable )
    {
        //cout<<" set colonur "<<ctable.r_lut[5]<<" "<<ctable.g_lut[5]<<" "<<ctable.b_lut[5]<<endl;
        QString temp;
        temp.setNum( ctable.r_lut[0],'g',4);
        ui->ctab_r1->setText(temp);
        temp.setNum( ctable.r_lut[1],'g',4);
        ui->ctab_r2->setText(temp);
        temp.setNum( ctable.r_lut[2],'g',4);
        ui->ctab_r3->setText(temp);
        temp.setNum( ctable.r_lut[3],'g',4);
        ui->ctab_r4->setText(temp);
        temp.setNum( ctable.r_lut[4],'g',4);
        ui->ctab_r5->setText(temp);
        temp.setNum( ctable.r_lut[5],'g',4);
        ui->ctab_r6->setText(temp);

        temp.setNum( ctable.g_lut[0],'g',4);
        ui->ctab_g1->setText(temp);
        temp.setNum( ctable.g_lut[1],'g',4);
        ui->ctab_g2->setText(temp);
        temp.setNum( ctable.g_lut[2],'g',4);
        ui->ctab_g3->setText(temp);
        temp.setNum( ctable.g_lut[3],'g',4);
        ui->ctab_g4->setText(temp);
        temp.setNum( ctable.g_lut[4],'g',4);
        ui->ctab_g5->setText(temp);
        temp.setNum( ctable.g_lut[5],'g',4);
        ui->ctab_g6->setText(temp);

        temp.setNum( ctable.b_lut[0],'g',4);
        ui->ctab_b1->setText(temp);
        temp.setNum( ctable.b_lut[1],'g',4);
        ui->ctab_b2->setText(temp);
        temp.setNum( ctable.b_lut[2],'g',4);
        ui->ctab_b3->setText(temp);
        temp.setNum( ctable.b_lut[3],'g',4);
        ui->ctab_b4->setText(temp);
        temp.setNum( ctable.b_lut[4],'g',4);
        ui->ctab_b5->setText(temp);
        temp.setNum( ctable.b_lut[5],'g',4);
        ui->ctab_b6->setText(temp);

    //cout<<"alpha "<< ctable.a_lut[0]<<endl;
        temp.setNum( ctable.a_lut[0],'g',4);
        ui->ctab_a1->setText(temp);
        temp.setNum( ctable.a_lut[1],'g',4);
        ui->ctab_a2->setText(temp);
        temp.setNum( ctable.a_lut[2],'g',4);
        ui->ctab_a3->setText(temp);
        temp.setNum( ctable.a_lut[3],'g',4);
        ui->ctab_a4->setText(temp);
        temp.setNum( ctable.a_lut[4],'g',4);
        ui->ctab_a5->setText(temp);
        temp.setNum( ctable.a_lut[5],'g',4);
        ui->ctab_a6->setText(temp);

    //cout<<"set alpha 5 "<<ctable.sc_lut[5]<<endl;
        temp.setNum( ctable.low_clamp[0],'g',4);
        ui->ctab_r_lclamp->setText(temp);
        temp.setNum( ctable.low_clamp[1],'g',4);
        ui->ctab_g_lclamp->setText(temp);
        temp.setNum( ctable.low_clamp[2],'g',4);
        ui->ctab_b_lclamp->setText(temp);
        temp.setNum( ctable.low_clamp[3],'g',4);
        ui->ctab_a_lclamp->setText(temp);
    }
    void SurfaceManipulator::increaseContrast()
    {
        float inc_fac = ui->con_step->value();//0.9;
      //  cout<<"inc con "<<endl;
        float min = ui->ctab_sc1->text().toFloat();
        float max = ui->ctab_sc6->text().toFloat();
        float mean = (min+max)*0.5;
       // cout<<"min mean "<<min<<" "<<mean<<endl;
        QString temp;
        temp.setNum((min - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc1->setText(temp);

        temp.setNum( (ui->ctab_sc2->text().toFloat() - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc2->setText(temp);

        temp.setNum( (ui->ctab_sc3->text().toFloat() - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc3->setText(temp);

        temp.setNum( (ui->ctab_sc4->text().toFloat() - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc4->setText(temp);

        temp.setNum( (ui->ctab_sc5->text().toFloat() - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc5->setText(temp);

        temp.setNum( (max - mean)*inc_fac + mean,'g',4);
        ui->ctab_sc6->setText(temp);

        emit sig_changedColourTableSc();

    }
    void SurfaceManipulator::decreaseContrast()
    {
        float inc_fac =ui->con_step->value();
        //cout<<"inc con "<<endl;
        float min = ui->ctab_sc1->text().toFloat();
        float max = ui->ctab_sc6->text().toFloat();
        float mean = (min+max)*0.5;

        QString temp;
        temp.setNum((min - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc1->setText(temp);

        temp.setNum( (ui->ctab_sc2->text().toFloat() - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc2->setText(temp);

        temp.setNum( (ui->ctab_sc3->text().toFloat() - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc3->setText(temp);

        temp.setNum( (ui->ctab_sc4->text().toFloat() - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc4->setText(temp);

        temp.setNum( (ui->ctab_sc5->text().toFloat() - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc5->setText(temp);

        temp.setNum( (max - mean)/inc_fac + mean,'g',4);
        ui->ctab_sc6->setText(temp);

        emit sig_changedColourTableSc();
    }
    void SurfaceManipulator::increaseLevel()
    {
        float inc_step = ui->lev_step->value();

        ////cout<<"inc level "<<endl;
        QString temp;
        temp.setNum(ui->ctab_sc1->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc1->setText(temp);

        temp.setNum(ui->ctab_sc2->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc2->setText(temp);

        temp.setNum(ui->ctab_sc3->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc3->setText(temp);

        temp.setNum(ui->ctab_sc4->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc4->setText(temp);

        temp.setNum(ui->ctab_sc5->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc5->setText(temp);

        temp.setNum(ui->ctab_sc6->text().toFloat() + inc_step,'g',4);
        ui->ctab_sc6->setText(temp);

        emit sig_changedColourTableSc();

    }
    void SurfaceManipulator::decreaseLevel()
    {
        //cout<<"dec level "<<endl;
        float inc_step = ui->lev_step->value();

        //cout<<"inc level "<<endl;
        QString temp;
        temp.setNum(ui->ctab_sc1->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc1->setText(temp);

        temp.setNum(ui->ctab_sc2->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc2->setText(temp);

        temp.setNum(ui->ctab_sc3->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc3->setText(temp);

        temp.setNum(ui->ctab_sc4->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc4->setText(temp);

        temp.setNum(ui->ctab_sc5->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc5->setText(temp);

        temp.setNum(ui->ctab_sc6->text().toFloat() - inc_step,'g',4);
        ui->ctab_sc6->setText(temp);

        emit sig_changedColourTableSc();
    }

void SurfaceManipulator::setMaterialIndex(const int & index)
{
    ui->combo_preset_materials->setCurrentIndex(index);
}
void SurfaceManipulator::setShaderIndex(const int & index)
{
    ui->radio_useScalars->setChecked(index);

}
void SurfaceManipulator::setShaderIndex(const int & surf_index, const int & glsl_index)
{
    v_glsl_index[surf_index] = glsl_index;
    ui->cbox_surf_shaders->setCurrentIndex(glsl_index);
}

/*
 void SurfaceManipulator::updatePageText()
    {
        int cur=ui->surface_props_stackedWidget->currentIndex()-1;

        QString temp,temp2;
        temp.setNum(cur+2);
        temp2.setNum(ui->surface_props_stackedWidget->count());
        ui->scene_page_label->setText("page "+temp+"/"+temp2);
    }

    void SurfaceManipulator::nextPage()
    {
        int cur=ui->surface_props_stackedWidget->currentIndex();
        if ( cur <= (ui->surface_props_stackedWidget->count()-2) )
        {
            ui->surface_props_stackedWidget->setCurrentIndex(cur+1);
            updatePageText();
        }
    }
    void SurfaceManipulator::prevPage()
    {
        int cur=ui->surface_props_stackedWidget->currentIndex();
        if ( cur >= 1 )
        {
            ui->surface_props_stackedWidget->setCurrentIndex(cur-1);
            updatePageText();
        }
    }
*/
void SurfaceManipulator::addPresetSetMaterial(const string & name)
{
ui->combo_preset_materials->addItem(QString(name.c_str()));

}

void SurfaceManipulator::setAmbientMag(int val)
{
    float r=ui->ambient_r->text().toFloat();
    float g=ui->ambient_g->text().toFloat();
    float b=ui->ambient_b->text().toFloat();
    float mag=val/100.0;//*sqrt(3);
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
    temp.setNum(r*sc);
    ui->ambient_r->setText(temp);
    temp.setNum(g*sc);
    ui->ambient_g->setText(temp);
    temp.setNum(b*sc);
    ui->ambient_b->setText(temp);
    emit sig_changedSurfaceProperties();
}


void SurfaceManipulator::setDiffuseMag(int val)
{
    float r=ui->diffuse_r->text().toFloat();
    float g=ui->diffuse_g->text().toFloat();
    float b=ui->diffuse_b->text().toFloat();
    float mag=val/100.0;
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
    temp.setNum(r*sc);
    ui->diffuse_r->setText(temp);
    temp.setNum(g*sc);
    ui->diffuse_g->setText(temp);
    temp.setNum(b*sc);
    ui->diffuse_b->setText(temp);
    emit sig_changedSurfaceProperties();

}


void SurfaceManipulator::setSpecularMag(int val)
{
    float r=ui->specular_r->text().toFloat();
    float g=ui->specular_g->text().toFloat();
    float b=ui->specular_b->text().toFloat();
    float mag=val/100.0;
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
    temp.setNum(r*sc);
    ui->specular_r->setText(temp);
    temp.setNum(g*sc);
    ui->specular_g->setText(temp);
    temp.setNum(b*sc);
    ui->specular_b->setText(temp);
    emit sig_changedSurfaceProperties();

}

void SurfaceManipulator::setShininess(int val)
{
    QString temp;
    temp.setNum(static_cast<float>(val)/ui->shininess_slider->maximum()*max_shininess);
    ui->shininess->setText(temp);
    emit sig_changedSurfaceProperties();


}

void SurfaceManipulator::setOpacity(int val)
{
    QString temp;
    temp.setNum(static_cast<float>(val)/ui->opacity_slider->maximum());
    ui->opacity->setText(temp);
    ui->ambient_a->setText(temp);
    ui->diffuse_a->setText(temp);
    ui->specular_a->setText(temp);
    emit sig_changedSurfaceProperties();

}


material SurfaceManipulator::getCurrentSurfaceMaterial()
{
    return material(  ui->ambient_r->text().toFloat(),ui->ambient_g->text().toFloat(),ui->ambient_b->text().toFloat(),ui->ambient_a->text().toFloat(),  \
                      ui->diffuse_r->text().toFloat(),ui->diffuse_g->text().toFloat(),ui->diffuse_b->text().toFloat(),ui->diffuse_a->text().toFloat(), \
                      ui->specular_r->text().toFloat(),ui->specular_g->text().toFloat(),ui->specular_b->text().toFloat(),ui->specular_a->text().toFloat(),  \
                      ui->shininess->text().toFloat(), ui->opacity->text().toFloat() );
}



void SurfaceManipulator::setSurfaceMaterial( const material & material_in)
{
    QString temp;
    temp.setNum( material_in.ambient[0],'g',4);
    ui->ambient_r->setText(temp);
    temp.setNum( material_in.ambient[1],'g',4);
    ui->ambient_g->setText(temp);
    temp.setNum( material_in.ambient[2],'g',4);
    ui->ambient_b->setText(temp);
    temp.setNum( material_in.ambient[3],'g',4);
    ui->ambient_a->setText(temp);

    temp.setNum( material_in.diffuse[0],'g',4);
    ui->diffuse_r->setText(temp);
    temp.setNum( material_in.diffuse[1],'g',4);
    ui->diffuse_g->setText(temp);
    temp.setNum( material_in.diffuse[2],'g',4);
    ui->diffuse_b->setText(temp);
    temp.setNum( material_in.diffuse[3],'g',4);
    ui->diffuse_a->setText(temp);

    temp.setNum( material_in.specular[0],'g',4);
    ui->specular_r->setText(temp);
    temp.setNum( material_in.specular[1],'g',4);
    ui->specular_g->setText(temp);
    temp.setNum( material_in.specular[2],'g',4);
    ui->specular_b->setText(temp);
    temp.setNum( material_in.specular[3],'g',4);
    ui->specular_a->setText(temp);

    temp.setNum( material_in.shininess,'g',4);
    ui->shininess->setText(temp);
    ui->shininess_slider->setSliderPosition(material_in.shininess*ui->shininess_slider->maximum());
    temp.setNum( material_in.opacity,'g',4);
    ui->opacity->setText(temp);
    ui->opacity_slider->setSliderPosition(material_in.opacity*ui->opacity_slider->maximum());
}

glyph SurfaceManipulator::getCurrentGlyph()
{
    return glyph(  ui->glyph_tip_radius->text().toFloat() , ui->glyph_tip_height->text().toFloat(), \
                      ui->glyph_base_radius->text().toFloat(), \
                      ui->glyph_scale->text().toFloat(),       ui->glyph_N_faces->text().toFloat(), \
                      ui->glyph_opacity->text().toFloat());
}

void SurfaceManipulator::setGlyphProperties( const glyph & glyph_in)
{
    QString temp;
    temp.setNum( glyph_in.tip[0],'g',4);
    ui->glyph_tip_radius->setText(temp);
    temp.setNum( glyph_in.tip[1],'g',4);
    ui->glyph_tip_height->setText(temp);


    temp.setNum( glyph_in.base_radius,'g',4);
    ui->glyph_base_radius->setText(temp);

    temp.setNum( glyph_in.scale_factor,'g',4);
    ui->glyph_scale->setText(temp);



    temp.setNum( glyph_in.N_faces);
    ui->glyph_N_faces->setText(temp);

    temp.setNum( glyph_in.opacity,'g',4);
    ui->glyph_opacity->setText(temp);
   // ui->glyph_opacity_slider->setSliderPosition(glyph_in.opacity*ui->opacity_slider->maximum());
}

SurfaceManipulator::~SurfaceManipulator()
{
    delete ui;
}
void SurfaceManipulator::changeShader( int index )
{
    if (ui->surface_list->currentRow() >= 0 )
    {
    v_glsl_index[ ui->surface_list->currentRow() ] = ui->cbox_surf_shaders->currentIndex();
    cout<<"change shader " <<v_glsl_index[ ui->surface_list->currentRow() ] <<endl;
    emit sig_changeShaderProgram(ui->cbox_surf_shaders->currentIndex());
}
    }

void SurfaceManipulator::addItemToList(const string & filename)
{
    ////cout<<"add tien"<<endl;
    ui->surface_list->addItem(QString(filename.c_str()).section('/',-1) );
        ////cout<<"add tien2 "<<ui->surface_list->count()<<endl;

    ui->surface_list->setCurrentRow(ui->surface_list->count()-1,QItemSelectionModel::Deselect);
    ui->surface_list->setCurrentRow(ui->surface_list->count()-1);
v_glsl_index.push_back(0);
        //cout<<"add tien3"<<endl;

}
void SurfaceManipulator::addItemToAppendedList(const string & filename)
{
    ////cout<<"add tien"<<endl;
    ui->surf_app_data->addItem(QString(filename.c_str()).section('/',-1) );
        //cout<<"add tien2 "<<ui->surface_list->count()<<endl;

    ui->surf_app_data->setCurrentRow(ui->surface_list->count()-1);
        //cout<<"add tien3"<<endl;

}
int SurfaceManipulator::getCurrentSurfaceIndex()
{
    return ui->surface_list->currentRow();
}
vector<int> SurfaceManipulator::getCurrentSurfaceIndices()
{
    vector<int> indices;
    QList<QListWidgetItem *> sel_items = ui->surface_list->selectedItems();
   //const QModelIndexList selectedIndices = ui->surface_list->selectedIndexes();
    for ( QList<QListWidgetItem *>::iterator i_s = sel_items.begin(); i_s != sel_items.end(); ++i_s)
        indices.push_back(ui->surface_list->row(*i_s));

    return indices;
}

int SurfaceManipulator::getCurrentScalarIndex()
{
    return ui->MinScalarIndex->text().toInt();
}


void SurfaceManipulator::setScalarIndicesMinMax( const int & min, const int & max )
{

    //cout<<"set scalar indices min/max "<<min<<" "<<max<<endl;
    QString temp;
    temp.setNum(min);
    ui->MinScalarIndex->setText(temp);
    temp.setNum(max);
    ui->MaxScalarIndex->setText(temp);
//ui->MinScalarIndex->update();
//ui->MinScalarIndex->repaint();
}


void SurfaceManipulator::setAppendedData( const vector< string > & names )
{
    ui->surf_app_data->clear();
    for (vector<string>::const_iterator i = names.begin(); i != names.end(); ++i)
    {
     //cout<<"setting appended data "<<endl;
        ui->surf_app_data->addItem(QString(i->c_str()) );
    }
   }
int SurfaceManipulator::getCurrentAppendedSurfaceIndex()
{
  //  if (ui->surf_app_data->count()==0)
    //    return -1;
    return ui->surf_app_data->currentRow();
}

colour_table SurfaceManipulator::getCurrentColourTable()
{
    return colour_table( ui->ctab_r1->text().toFloat(), ui->ctab_r2->text().toFloat(), ui->ctab_r3->text().toFloat(), ui->ctab_r4->text().toFloat(), ui->ctab_r5->text().toFloat(), ui->ctab_r6->text().toFloat(),\
                         ui->ctab_g1->text().toFloat(), ui->ctab_g2->text().toFloat(), ui->ctab_g3->text().toFloat(), ui->ctab_g4->text().toFloat(), ui->ctab_g5->text().toFloat(), ui->ctab_g6->text().toFloat(),\
                         ui->ctab_b1->text().toFloat(), ui->ctab_b2->text().toFloat(), ui->ctab_b3->text().toFloat(), ui->ctab_b4->text().toFloat(), ui->ctab_b5->text().toFloat(), ui->ctab_b6->text().toFloat(),\
                         ui->ctab_a1->text().toFloat(), ui->ctab_a2->text().toFloat(), ui->ctab_a3->text().toFloat(), ui->ctab_a4->text().toFloat(), ui->ctab_a5->text().toFloat(),  ui->ctab_a6->text().toFloat(),\
                         ui->ctab_sc1->text().toFloat(), ui->ctab_sc2->text().toFloat(), ui->ctab_sc3->text().toFloat(), ui->ctab_sc4->text().toFloat(), ui->ctab_sc5->text().toFloat(),ui->ctab_sc6->text().toFloat(), \
                         ui->ctab_r_lclamp->text().toFloat(),ui->ctab_g_lclamp->text().toFloat(),ui->ctab_b_lclamp->text().toFloat(),ui->ctab_a_lclamp->text().toFloat());
}
void SurfaceManipulator::setUseScalarMap( const int & on )
{
    ui->radio_useScalars->setChecked(on);
}
void SurfaceManipulator::addColourTable(const string & filename )
{
    ui->combo_cmap->addItem(QString(filename.c_str()));
    //ui->image_list->setCurrentRow(ui->image_list->count()-1);
}
void SurfaceManipulator::setColourTable( const colour_table & ctable )
{
    QString temp; 
    temp.setNum( ctable.r_lut[0],'g',4);
    ui->ctab_r1->setText(temp);
    temp.setNum( ctable.r_lut[1],'g',4);
    ui->ctab_r2->setText(temp);
    temp.setNum( ctable.r_lut[2],'g',4);
    ui->ctab_r3->setText(temp);
    temp.setNum( ctable.r_lut[3],'g',4);
    ui->ctab_r4->setText(temp);
    temp.setNum( ctable.r_lut[4],'g',4);
    ui->ctab_r5->setText(temp);
    temp.setNum( ctable.r_lut[5],'g',4);
    ui->ctab_r6->setText(temp);

    temp.setNum( ctable.g_lut[0],'g',4);
    ui->ctab_g1->setText(temp);
    temp.setNum( ctable.g_lut[1],'g',4);
    ui->ctab_g2->setText(temp);
    temp.setNum( ctable.g_lut[2],'g',4);
    ui->ctab_g3->setText(temp);
    temp.setNum( ctable.g_lut[3],'g',4);
    ui->ctab_g4->setText(temp);
    temp.setNum( ctable.g_lut[4],'g',4);
    ui->ctab_g5->setText(temp);
    temp.setNum( ctable.g_lut[5],'g',4);
    ui->ctab_g6->setText(temp);

    temp.setNum( ctable.b_lut[0],'g',4);
    ui->ctab_b1->setText(temp);
    temp.setNum( ctable.b_lut[1],'g',4);
    ui->ctab_b2->setText(temp);
    temp.setNum( ctable.b_lut[2],'g',4);
    ui->ctab_b3->setText(temp);
    temp.setNum( ctable.b_lut[3],'g',4);
    ui->ctab_b4->setText(temp);
    temp.setNum( ctable.b_lut[4],'g',4);
    ui->ctab_b5->setText(temp);
    temp.setNum( ctable.b_lut[5],'g',4);
    ui->ctab_b6->setText(temp);

    temp.setNum( ctable.sc_lut[0],'g',4);
    ui->ctab_sc1->setText(temp);
    temp.setNum( ctable.sc_lut[1],'g',4);
    ui->ctab_sc2->setText(temp);
    temp.setNum( ctable.sc_lut[2],'g',4);
    ui->ctab_sc3->setText(temp);
    temp.setNum( ctable.sc_lut[3],'g',4);
    ui->ctab_sc4->setText(temp);
    temp.setNum( ctable.sc_lut[4],'g',4);
    ui->ctab_sc5->setText(temp);
    temp.setNum( ctable.sc_lut[5],'g',4);
    ui->ctab_sc6->setText(temp);

    temp.setNum( ctable.a_lut[0],'g',4);
    ui->ctab_a1->setText(temp);
    temp.setNum( ctable.a_lut[1],'g',4);
    ui->ctab_a2->setText(temp);
    temp.setNum( ctable.a_lut[2],'g',4);
    ui->ctab_a3->setText(temp);
    temp.setNum( ctable.a_lut[3],'g',4);
    ui->ctab_a4->setText(temp);
    temp.setNum( ctable.a_lut[4],'g',4);
    ui->ctab_a5->setText(temp);
    temp.setNum( ctable.a_lut[5],'g',4);
    ui->ctab_a6->setText(temp);


    temp.setNum( ctable.low_clamp[0],'g',4);
    ui->ctab_r_lclamp->setText(temp);
    temp.setNum( ctable.low_clamp[1],'g',4);
    ui->ctab_g_lclamp->setText(temp);
    temp.setNum( ctable.low_clamp[2],'g',4);
    ui->ctab_b_lclamp->setText(temp);
    temp.setNum( ctable.low_clamp[3],'g',4);
    ui->ctab_a_lclamp->setText(temp);

}
//-----------DOCKING-------------//
void SurfaceManipulator::showDockSurfaceSelector(){
    ui->dock_surface_list->show();
    ui->dock_surface_list->setFloating(1);
}
void SurfaceManipulator::showDockMaterialProperties(){
    ui->dockMaterial->show();
    ui->dockMaterial->setFloating(1);
}
void SurfaceManipulator::showDockColourBar(){
    ui->dock_surf_colour_bar->show();
    ui->dock_surf_colour_bar->setFloating(1);
}
void SurfaceManipulator::showDockGlyphs(){
    ui->dock_surf_glyphs->show();
    ui->dock_surf_glyphs->setFloating(1);
}
void SurfaceManipulator::showDockColourMap(){
    ui->dockColourMap->show();
    ui->dockColourMap->setFloating(1);
}
void SurfaceManipulator::showDockPolygonMode(){
    ui->dockPolygons->show();
   ui->dockPolygons->setFloating(1);
}

void SurfaceManipulator::doDockSurfaceSelector(){
    ui->dock_surface_list->setFloating(0);
}
void SurfaceManipulator::doDockMaterialProperties(){
    ui->dockMaterial->setFloating(0);
}
void SurfaceManipulator::doDockColourBar(){
    ui->dock_surf_colour_bar->setFloating(0);
}
void SurfaceManipulator::doDockGlyphs(){
    ui->dock_surf_glyphs->setFloating(0);
}
void SurfaceManipulator::doDockColourMap(){
    ui->dockColourMap->setFloating(0);
}
void SurfaceManipulator::doDockPolygonMode(){
    ui->dockPolygons->setFloating(0);
}


//--------------------------------//

//void SurfaceManipulator::setPolygonFillMode(  )
//{


//}



void SurfaceManipulator::changeEvent(QEvent *e)
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
