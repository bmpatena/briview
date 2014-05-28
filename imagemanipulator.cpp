#include "imagemanipulator.h"
#include "ui_imagemanipulator.h"
#include <iostream>

using namespace std;
using namespace briview;
using namespace fslsurface_name;
ImageManipulator::ImageManipulator(QWidget *parent) :
        QDockWidget(parent),
        ui(new Ui::ImageManipulator_form)
{
    ui->setupUi(this);
    //-----------these connection pass up signal from the menu bar to the image manipulator---------//

    //-----------these connection pass up signal from the menu bar to the image manipulator---------//

    connect(ui->x_slider,SIGNAL(sliderMoved(int)),this,SIGNAL(sig_changedSliceX(int)));
    connect(ui->y_slider,SIGNAL(sliderMoved(int)),this,SIGNAL(sig_changedSliceY(int)));
    connect(ui->z_slider,SIGNAL(sliderMoved(int)),this,SIGNAL(sig_changedSliceZ(int)));
    connect(ui->x_onoff,SIGNAL(clicked(bool)),this,SIGNAL(sig_toggledSliceX(bool)));
    connect(ui->y_onoff,SIGNAL(clicked(bool)),this,SIGNAL(sig_toggledSliceY(bool)));
    connect(ui->z_onoff,SIGNAL(clicked(bool)),this,SIGNAL(sig_toggledSliceZ(bool)));

    connect(ui->image_list, SIGNAL(currentRowChanged (int) ),this,SLOT(slot_currentImageChanged(int)));

    connect(ui->im_up,SIGNAL(pressed()),this,SLOT(shiftImageUpList()));
    connect(ui->im_down,SIGNAL(pressed()),this,SLOT(shiftImageDownList()));
     connect(ui->comboImageSpace,SIGNAL(activated (int)) ,this,SIGNAL(sig_changedImageSpace(int)));


    connect(ui->combo_opacity, SIGNAL(activated (int) ),this,SIGNAL(sig_changedOpacityMode(int)));
    connect(ui->combo_cmap, SIGNAL(activated (int) ),this,SIGNAL(sig_changedColourTable(int)));

    connect(ui->combo_blend, SIGNAL(activated (int) ),this,SIGNAL(sig_changedBlendFunc(int)));

      connect(ui->con_dec  ,SIGNAL(pressed()),this,SLOT(decreaseContrast()));
      connect(ui->con_inc  ,SIGNAL(pressed()),this,SLOT(increaseContrast()));
      connect(ui->level_dec  ,SIGNAL(pressed()),this,SLOT(decreaseLevel()));
      connect(ui->level_inc  ,SIGNAL(pressed()),this,SLOT(increaseLevel()));
       connect(ui->im_opacity,SIGNAL(textEdited(QString)),this,SLOT(setImageOpacitySlider(QString)));
 //   connect(ui->im_opacity_slider,SIGNAL(sliderMoved(int)),this, SLOT(setImageOpacityFromSlider(int)));
     connect(ui->im_opacity_slider,SIGNAL(sliderMoved(int)),this, SLOT(setImageOpacityFromSlider(int)));
  //  connect(ui->im_opacity_slider,SIGNAL(sliderPressed()),this, SLOT(setImageOpacityFromSlider()));

        connect(ui->x_voxel,SIGNAL(textEdited(QString)),this,SLOT(changeXVoxel(QString)));
        connect(ui->y_voxel,SIGNAL(textEdited(QString)),this,SLOT(changeYVoxel(QString)));
        connect(ui->z_voxel,SIGNAL(textEdited(QString)),this,SLOT(changeZVoxel(QString)));

 connect(ui->x_mm,SIGNAL(textEdited(QString)),this,SLOT(changeXmm(QString)));
        connect(ui->y_mm,SIGNAL(textEdited(QString)),this,SLOT(changeYmm(QString)));
        connect(ui->z_mm,SIGNAL(textEdited(QString)),this,SLOT(changeZmm(QString)));


    connect(ui->but_im_onoff,SIGNAL(clicked(bool)),this,SLOT(slot_toggleImage(bool)));

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


    //--------------------------Colour Bar------------------------------------
    connect(ui->cbar_origin_x, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_origin_y, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_origin_z, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_length, SIGNAL(textChanged(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->cbar_width, SIGNAL(textChanged(QString)),this,SIGNAL(sig_changedColourBar()));

 //   connect(ui->cbar_width_dec, SIGNAL(pressed()),this,SLOT(cbar_width_dec()));
   // connect(ui->cbar_width_inc, SIGNAL(pressed()),this,SLOT(cbar_width_inc()));
   // connect(ui->cbar_length_dec, SIGNAL(pressed()),this,SLOT(cbar_length_dec()));
   // connect(ui->cbar_length_inc, SIGNAL(pressed()),this,SLOT(cbar_length_inc()));

    connect(ui->cbar_title, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourBar()));
    connect(ui->im_colour_bar_onoff, SIGNAL(released()),this,SIGNAL(sig_changedColourBar()));

    //---------------------------------

    connect(ui->but_dock_improps,SIGNAL(clicked()),this,SLOT(doDockProperties()));
    connect(ui->but_dock_navi,SIGNAL(clicked()),this,SLOT(doDockImageNavigator()));
    connect(ui->but_dock_coord,SIGNAL(clicked()),this,SLOT(doDockCoordinateSystem()));
    connect(ui->but_dock_blend,SIGNAL(clicked()),this,SLOT(doDockBlending()));
    connect(ui->but_dock_cbar,SIGNAL(clicked()),this,SLOT(doDockColourBar()));
    connect(ui->but_dock_imlist,SIGNAL(clicked()),this,SLOT(doDockImList()));


    ///connnect the docking signal with showing the button to re-dock
    connect(ui->dockImageColourMap,SIGNAL(topLevelChanged(bool)),ui->but_dock_improps,SLOT(setVisible(bool)));
    connect(ui->dockImageNavigator,SIGNAL(topLevelChanged(bool)),ui->but_dock_navi,SLOT(setVisible(bool)));
    connect(ui->dock_coords,SIGNAL(topLevelChanged(bool)),ui->but_dock_coord,SLOT(setVisible(bool)));
    connect(ui->dock_im_blend,SIGNAL(topLevelChanged(bool)),ui->but_dock_blend,SLOT(setVisible(bool)));
    connect(ui->dock_im_colour_bar,SIGNAL(topLevelChanged(bool)),ui->but_dock_cbar,SLOT(setVisible(bool)));
    connect(ui->image_list_dock,SIGNAL(topLevelChanged(bool)),ui->but_dock_imlist,SLOT(setVisible(bool)));
    //-------initialize UI configuration
    ui->but_dock_coord->hide();
    ui->but_dock_improps->hide();
    ui->but_dock_cbar->hide();
    ui->but_dock_blend->hide();
    ui->but_dock_navi->hide();
    ui->but_dock_imlist->hide();

    ///----------------




}

ImageManipulator::~ImageManipulator()
{
    delete ui;
}

void ImageManipulator::changeEvent(QEvent *e)
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

void ImageManipulator::slot_currentImageChanged(int index )
{
        int index_rev = ui->image_list->count() - 1 - index ;
        cout<<"ImageManipulator::slot_currentImageChanged( "<<index<<" "<<index_rev<<endl;

    emit sig_changedCurrentImage(index_rev);
}

//void ImageManipulator::updatePageText()
//{
 //   int cur=ui->im_props_stackedWidget->currentIndex()-1;

  //  QString temp,temp2;
   // temp.setNum(cur+2);
   // temp2.setNum(ui->im_props_stackedWidget->count());
   // ui->im_page_label->setText("page "+temp+"/"+temp2);
//}

//void ImageManipulator::nextPage()
//{
 //   int cur=ui->im_props_stackedWidget->currentIndex();
  //  if ( cur <= (ui->im_props_stackedWidget->count()-2) )
   // {
     //   ui->im_props_stackedWidget->setCurrentIndex(cur+1);
       // updatePageText();
//    }/
//}
//void ImageManipulator::prevPage()
//{
//    int cur=ui->im_props_stackedWidget->currentIndex();
//    if ( cur >= 1 )
//    {
//        ui->im_props_stackedWidget->setCurrentIndex(cur-1);
//        updatePageText();
//    }
//}

void ImageManipulator::getColourBarGeometry(briview::float3 & origin, float & length, float & width )
{
    origin = briview::float3(ui->cbar_origin_x->text().toFloat(), ui->cbar_origin_y->text().toFloat(), ui->cbar_origin_z->text().toFloat());
    length = ui->cbar_length->text().toFloat();
    width = ui->cbar_width->text().toFloat();

}

    string ImageManipulator::getColourBarTitle()
    {
        return ui->cbar_title->text().toStdString();
    }

bool ImageManipulator::getColourBarVisibility()
{
       return (ui->im_colour_bar_onoff->isChecked());
}


void ImageManipulator::setCoordinateSpace( const int & index )
{
    ui->comboImageSpace->setCurrentIndex(index);
}

void ImageManipulator::setColourTableIndex( const int & index)
{
    //cout<<"set colour table index "<<index<<endl;
    ui->combo_cmap->setCurrentIndex(index);
}

void ImageManipulator::addColourTable(const string & filename )
{
    ui->combo_cmap->addItem(QString(filename.c_str()));
    //ui->image_list->setCurrentRow(ui->image_list->count()-1);
}



void ImageManipulator::increaseContrast()
{
  //  float inc_fac = 0.9;
      float inc_fac = ui->con_step->value();
    //cout<<"inc con "<<endl;
    float min = ui->ctab_sc1->text().toFloat();
    float max = ui->ctab_sc6->text().toFloat();
    float mean = (min+max)*0.5;

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
void ImageManipulator::decreaseContrast()
{
   // float inc_fac = 0.9;
      float inc_fac = ui->con_step->value();
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
void ImageManipulator::increaseLevel()
{
   // float inc_step = 100;
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
void ImageManipulator::decreaseLevel()
{
    //cout<<"dec level "<<endl;
    //float inc_step = 100;
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

colour_table ImageManipulator::getCurrentColourTable()
{
    //cout<<" get colonurT "<< ui->ctab_sc1->text().toFloat()<<endl;

    return colour_table( ui->ctab_r1->text().toFloat(), ui->ctab_r2->text().toFloat(), ui->ctab_r3->text().toFloat(), ui->ctab_r4->text().toFloat(), ui->ctab_r5->text().toFloat(), ui->ctab_r6->text().toFloat(), \
                         ui->ctab_g1->text().toFloat(), ui->ctab_g2->text().toFloat(), ui->ctab_g3->text().toFloat(), ui->ctab_g4->text().toFloat(), ui->ctab_g5->text().toFloat(), ui->ctab_g6->text().toFloat(), \
                         ui->ctab_b1->text().toFloat(), ui->ctab_b2->text().toFloat(), ui->ctab_b3->text().toFloat(), ui->ctab_b4->text().toFloat(), ui->ctab_b5->text().toFloat(), ui->ctab_b6->text().toFloat(), \
                         ui->ctab_a1->text().toFloat(), ui->ctab_a2->text().toFloat(), ui->ctab_a3->text().toFloat(), ui->ctab_a4->text().toFloat(), ui->ctab_a5->text().toFloat(), ui->ctab_a6->text().toFloat(), \
                         ui->ctab_sc1->text().toFloat(), ui->ctab_sc2->text().toFloat(), ui->ctab_sc3->text().toFloat(), ui->ctab_sc4->text().toFloat(), ui->ctab_sc5->text().toFloat(), ui->ctab_sc6->text().toFloat(),\
                         ui->ctab_r_lclamp->text().toFloat(),ui->ctab_g_lclamp->text().toFloat(),ui->ctab_b_lclamp->text().toFloat(),ui->ctab_a_lclamp->text().toFloat() );
}

void ImageManipulator::setColourTable( const colour_table & ctable )
{
    //cout<<" set colonur "<<ctable.sc_lut[0]<<" "<<ctable.sc_lut[1]<<" "<<ctable.sc_lut[2]<<" "<<ctable.sc_lut[3]<<" "<<ctable.sc_lut[4]<<" "<<ctable.sc_lut[5]<<endl;
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


void ImageManipulator::setColourTableNoSc( const colour_table & ctable )
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


void ImageManipulator::showDockImageNavigator(){
    ui->dockImageNavigator->show();
    ui->dockImageNavigator->setFloating(1);
}

void ImageManipulator::showDockProperties(){
    ui->dockImageColourMap->show();
    ui->dockImageColourMap->setFloating(1);
}
void ImageManipulator::showDockColourBar(){
    ui->dock_im_colour_bar->show();
    ui->dock_im_colour_bar->setFloating(1);
}
void ImageManipulator::showDockCoordinateSystem(){
    ui->dock_coords->show();
    ui->dock_coords->setFloating(1);
}
void ImageManipulator::showDockBlending(){
    ui->dock_im_blend->show();
    ui->dock_im_blend->setFloating(1);

}
void ImageManipulator::showDockImList(){
    ui->image_list_dock->show();
    ui->image_list_dock->setFloating(1);

}
void ImageManipulator::doDockImageNavigator(){
    ui->dockImageNavigator->setFloating(0);
}
void ImageManipulator::doDockProperties(){
    ui->dockImageColourMap->setFloating(0);

}
void ImageManipulator::doDockColourBar(){
    ui->dock_im_colour_bar->setFloating(0);

}
void ImageManipulator::doDockCoordinateSystem(){
    ui->dock_coords->setFloating(0);

}
void ImageManipulator::doDockBlending(){
    ui->dock_im_blend->setFloating(0);

}
void ImageManipulator::doDockImList(){
    ui->image_list_dock->setFloating(0);

}
/*void ImageManipulator::setColourTable(const float & rmin, const float & gmin, const float & bmin, const float & rmax, const float & gmax, const float & bmax )
{
    QString temp;
    temp.setNum(rmin,'g',3);
    ui->r_cmap_min->setText(temp);
    temp.setNum(gmin,'g',3);
    ui->g_cmap_min->setText(temp);
    temp.setNum(bmin,'g',3);
    ui->b_cmap_min->setText(temp);
    temp.setNum(rmax,'g',3);
    ui->r_cmap_max->setText(temp);
    temp.setNum(gmax,'g',3);
    ui->g_cmap_max->setText(temp);
    temp.setNum(bmax,'g',3);
    ui->b_cmap_max->setText(temp);

}
void ImageManipulator::setColourTable(const GLfloat *cmap)
{
        QString temp;
        temp.setNum(cmap[0],'g',3);
        ui->ctab_sc1->setText(temp);
        temp.setNum(cmap[1],'g',3);
        ui->ctab_sc2->setText(temp);
        temp.setNum(cmap[2],'g',3);
        ui->ctab_sc3->setText(temp);
        temp.setNum(cmap[3],'g',3);
        ui->ctab_sc4->setText(temp);
        temp.setNum(cmap[4],'g',3);
        ui->ctab_sc5->setText(temp);


        temp.setNum(cmap[5],'g',3);
        ui->ctab_r1->setText(temp);
        temp.setNum(cmap[6],'g',3);
        ui->ctab_r2->setText(temp);
        temp.setNum(cmap[7],'g',3);
        ui->ctab_r3->setText(temp);
        temp.setNum(cmap[8],'g',3);
        ui->ctab_r4->setText(temp);
        temp.setNum(cmap[9],'g',3);
        ui->ctab_r5->setText(temp);

        temp.setNum(cmap[10],'g',3);
        ui->ctab_g1->setText(temp);
        temp.setNum(cmap[11],'g',3);
        ui->ctab_g2->setText(temp);
        temp.setNum(cmap[12],'g',3);
        ui->ctab_g3->setText(temp);
        temp.setNum(cmap[13],'g',3);
        ui->ctab_g4->setText(temp);
        temp.setNum(cmap[14],'g',3);
        ui->ctab_g5->setText(temp);

        temp.setNum(cmap[15],'g',3);
        ui->ctab_b1->setText(temp);
        temp.setNum(cmap[16],'g',3);
        ui->ctab_b2->setText(temp);
        temp.setNum(cmap[17],'g',3);
        ui->ctab_b3->setText(temp);
        temp.setNum(cmap[18],'g',3);
        ui->ctab_b4->setText(temp);
        temp.setNum(cmap[19],'g',3);
        ui->ctab_b5->setText(temp);


        temp.setNum(cmap[20],'g',3);
        ui->ctab_r_lclamp->setText(temp);
        temp.setNum(cmap[21],'g',3);
        ui->ctab_g_lclamp->setText(temp);
        temp.setNum(cmap[22],'g',3);
        ui->ctab_b_lclamp->setText(temp);

}
*/
void ImageManipulator::setSlidersExtents(const int & xsize, const int & ysize, const int & zsize)
{
    ui->x_slider->setRange(0,xsize-1);
    ui->y_slider->setRange(0,ysize-1);
    ui->z_slider->setRange(0,zsize-1);

}
void ImageManipulator::setSlidersPosition(const float & x, const float & y, const float & z,const float & xdim,const float & ydim,const float & zdim )
{
    ui->x_slider->setSliderPosition(x);
    ui->y_slider->setSliderPosition(y);
    ui->z_slider->setSliderPosition(z);

    QString temp;
    temp.setNum(x);
    ui->x_voxel->setText(temp);
    temp.setNum(y);
    ui->y_voxel->setText(temp);
    temp.setNum(z);
    ui->z_voxel->setText(temp);
    temp.setNum(x*xdim);
    ui->x_mm->setText(temp);
    temp.setNum(y*ydim);
    ui->y_mm->setText(temp);
    temp.setNum(z*zdim);
    ui->z_mm->setText(temp);
}

void ImageManipulator::changeXVoxel(QString num)
{
    emit sig_changedSliceX(num.toInt());
}
void ImageManipulator::changeYVoxel(QString num)
{
    emit sig_changedSliceY(num.toInt());
}
void ImageManipulator::changeZVoxel(QString num)
{
    emit sig_changedSliceZ(num.toInt());
}
void ImageManipulator::changeXmm(QString num)
{
    emit sig_changedSliceX(num.toFloat());
}
void ImageManipulator::changeYmm(QString num)
{
    emit sig_changedSliceY(num.toFloat());
}
void ImageManipulator::changeZmm(QString num)
{
    emit sig_changedSliceZ(num.toFloat());
}

void ImageManipulator::setSlicesVisibility(const bool3 & vis)
{
    ui->x_onoff->setChecked ( vis.x );
    ui->y_onoff->setChecked ( vis.y );
    ui->z_onoff->setChecked ( vis.z );

}

void ImageManipulator::slot_toggleImage(bool state)
{
//    bool state = ui->but_im_onoff->isChecked();
    if (state)
        ui->but_im_onoff->setText("Image ON");
    else
        ui->but_im_onoff->setText("Image OFF");
    ui->but_im_onoff->setChecked(state);
    emit sig_toggled_ImOnOff(state);
}
void ImageManipulator::toggleImage(const bool & state)
{
    if (state)
        ui->but_im_onoff->setText("Image ON");
    else
        ui->but_im_onoff->setText("Image OFF");
    ui->but_im_onoff->setChecked(state);

}


void ImageManipulator::setImageOpacityFromSlider(int val)
{
    QString temp;
    temp.setNum(static_cast<float>(val)/ ui->im_opacity_slider->maximum());
    ui->im_opacity->setText(temp);
    emit sig_changedOpacity(temp);
}
void ImageManipulator::setImageOpacityFromSlider()
{
    QString temp;
    temp.setNum(static_cast<float>(ui->im_opacity_slider->value())/ ui->im_opacity_slider->maximum());
    ui->im_opacity->setText(temp);
    emit sig_changedOpacity(temp);
}


void ImageManipulator::setImageOpacity(const float & op)
{
    QString temp;
    temp.setNum(op);    
    ui->im_opacity->setText(temp);    
}
void ImageManipulator::setImageOpacitySlider(QString op)
{   
    ui->im_opacity_slider->setSliderPosition( static_cast<int>(op.toFloat() * ui->im_opacity_slider->maximum()) );
}
void ImageManipulator::setImageOpacitySlider(const float & op)
{
    ui->im_opacity_slider->setSliderPosition( static_cast<int>(op * ui->im_opacity_slider->maximum()) );
}

/*
void ImageManipulator::setImageMin(const float & min)
{
    QString temp;
    temp.setNum(min);
    ui->ImageContrast_Min->setText(temp);
}

void ImageManipulator::setImageMax(const float & max)
{
    QString temp;
    temp.setNum(max);
    ui->ImageContrast_Max->setText(temp);
}
*/
int ImageManipulator::getCurrentImageIndex()
{
    //cout<<"image index "<<ui->image_list->count() - 1 - ui->image_list->currentRow()<<endl;
   // return (ui->image_list->count() -1 - ui->image_list->currentRow());
    return ( ui->image_list->currentRow());

}

vector<int> ImageManipulator::getCurrentImageIndices()
{

    vector<int> indices;
    QList<QListWidgetItem *> sel_items = ui->image_list->selectedItems();
   //const QModelIndexList selectedIndices = ui->surface_list->selectedIndexes();
    for ( QList<QListWidgetItem *>::iterator i_s = sel_items.begin(); i_s != sel_items.end(); ++i_s)
    {
        cout<<"getCurrentImageindices "<<*i_s<<endl;
        //invert indicies
        indices.push_back( (ui->image_list->count() -1 ) - ui->image_list->row(*i_s));
    }//ui->image_list->count() -1 - ui->image_list->row(*i_s));
                          //ui->image_list->row(*i_s));

    return indices;
}
QString ImageManipulator::getImageName( const int & index ){
    return ui->image_list->item(index)->text();

}

QString ImageManipulator::getCurrentImageName()
{
 //   //cout<<"image index "<<ui->image_list->count() - 1 - ui->image_list->currentRow()<<endl;
    return ui->image_list->currentItem()->text();
}

    int  ImageManipulator::getNumberOfImages()
    {
        return ui->image_list->count();
    }

void ImageManipulator::addItemToList(const string & filename)
{
    //cout<<"addItem "<<filename<<endl;
 //   ui->image_list->insertItem(0,QString(filename.c_str()).section('/',-1));

   // ui->image_list->addItem(QString(filename.c_str()).section('/',-1));
   // ui->image_list->setCurrentRow(0);//ui->image_list->count()-1);
   // ui->image_list->addItem(QString(filename.c_str()).section('/',-1) );
        ////cout<<"add tien2 "<<ui->surface_list->count()<<endl;
    ui->image_list->insertItem(0,QString(filename.c_str()).section('/',-1) );

//    ui->image_list->setCurrentRow(ui->image_list->count()-1,QItemSelectionModel::Deselect);
//    ui->image_list->setCurrentRow(ui->image_list->count()-1);
}

void ImageManipulator::shiftImageDownList()
{
    int row= ui->image_list->currentRow();
    int N=ui->image_list->count();
    if ((N>1)&&(row!=N-1))
    {

        ui->image_list->insertItem (row+1 , ui->image_list->takeItem(row));
        ui->image_list->setCurrentRow(row+1);
        emit sig_swapImages(row);

    }
 //cout<<"down "<<row<<" "<<N<<" "<<ui->image_list->currentRow()<<endl;

    //emit sig_swapImages(N-1-row);
}

void ImageManipulator::shiftImageUpList()
{
    int row= ui->image_list->currentRow();
    int N=ui->image_list->count();

    if ((N>1)&&(row!=0))
    {
        ui->image_list->insertItem ( row -1 , ui->image_list->takeItem(row));
          ui->image_list->setCurrentRow(row-1);
            emit sig_swapImages(row);

    }
   // emit sig_swapImages(N-1-row);

}




