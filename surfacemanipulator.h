#ifndef SURFACEMANIPULATOR_H
#define SURFACEMANIPULATOR_H

#include <QDockWidget>
#include <briview_structs.h>
#include "fslsurface/fslsurface_structs.h"
namespace Ui {
    class SurfaceManipulator_form;
}

class SurfaceManipulator : public QDockWidget {
    Q_OBJECT
public:
    SurfaceManipulator(QWidget *parent = 0);
    ~SurfaceManipulator();

    fslsurface_name::material getCurrentSurfaceMaterial();

    void addItemToList(const std::string & filename);
    void addItemToAppendedList(const std::string & filename);

    void addPresetSetMaterial(const std::string & name);
    void setColourTableIndex( const int & index);
    void addColourTable(const std::string & filename );
    void setColourTableNoSc( const fslsurface_name::colour_table & ctable );

    //void getSelectedSurfaces( std::vector<unsigned int> & indices );
    int getCurrentSurfaceIndex();
    int getCurrentAppendedSurfaceIndex();
    void getColourBarGeometry(briview::float3 & origin, float & length, float & width );
    bool getColourBarVisibility();
    void setCullFace(const bool & cull );
    void setPolyMode( const GLenum & front, const GLenum & back);
    void setPolyModeGlyph( const GLenum & front, const GLenum & back);

    void setFrontFace(const bool & CCW );
    void setMaterialIndex(const int & index);
    void setShaderIndex(const int & index);
    void setAppendedData( const std::vector< std::string > & names );
    void setScalarIndicesMinMax( const int & min, const int & max );
    int getCurrentScalarIndex();
    std::vector<int> getCurrentSurfaceIndices();

    //   void updatePageText();


public slots:
    void showDialog() { show(); }
//   void nextPage();
  //      void prevPage();
    //-----------DOCKING-------------//
    void showDockSurfaceSelector();
    void showDockMaterialProperties();
    void showDockColourBar();
    void showDockGlyphs();
    void showDockColourMap();
    void showDockPolygonMode();

    void doDockSurfaceSelector();
    void doDockMaterialProperties();
    void doDockColourBar();
    void doDockGlyphs();
    void doDockColourMap();
    void doDockPolygonMode();


    //--------------------------------//




    void setAmbientMag(int val);
    void setDiffuseMag(int val);
    void setSpecularMag(int val);
    void setShininess(int val);
    void setOpacity(int val);
    void setUseScalarMap( const int & on );
    //  void setPolygonFillMode(  );

    void setSurfaceMaterial(const fslsurface_name::material & material_in);

    fslsurface_name::colour_table getCurrentColourTable();
    std::string getColourBarTitle();


    void setColourTable( const fslsurface_name::colour_table & c_tab );
    fslsurface_name::glyph getCurrentGlyph();
    void setGlyphProperties( const fslsurface_name::glyph & glyph_in);

    void cbar_width_dec();
    void cbar_width_inc();
    void cbar_length_dec();
    void cbar_length_inc();

    void decreaseContrast();
    void increaseContrast();
    void decreaseLevel();
    void increaseLevel();

signals:


    void sig_changePolygonFillMode();
    void sig_changedCurrentSurface( int currentRow );
    void sig_changedSurfaceProperties();
    void sig_changedGlyphProperties();
    void sig_changedColourBar();
    void sig_appendSurfaceData();
    void sig_updateAppendedSurfaceInfo( int currentRow );

    void sig_changedColourTable();
    void sig_changedColourTable(int);
    void sig_changedColourTableSc();

    void sig_changeShaderProgram(bool map_scalars);
    void sig_changeSurfaceMaterial(int index);
    void sig_changedPolygonModeFront(int index);
    void sig_changedPolygonModeBack(int index);
    void sig_changedPolygonModeFrontGlyph(int index);
    void sig_changedPolygonModeBackGlyph(int index);
    void sig_changedScalarData();
    void sig_playSurfaceScalars();
    void sig_prevSurfaceScalars();
    void sig_nextSurfaceScalars();

    void sig_displayGlyphs( bool disp );
    void sig_enableCullFace( bool cull );
    void sig_ff_ccw( bool checked );
protected:
    void changeEvent(QEvent *e);

private:
    float max_shininess;

    Ui::SurfaceManipulator_form *ui;
};

#endif // SURFACEMANIPULATOR_H
