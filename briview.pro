# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:18:12
# -------------------------------------------------
QT += opengl
QT       += core gui widgets designer
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
QMAKE_MAC_SDK=macosx
QMAKE_MAC_SDK.macosx.path=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk

TARGET = briview

#QMAKE_CXXFLAGS = -arch x86_64
!macx {
DEFINES = GL_GLEXT_PROTOTYPES
}
#CONFIG+= qt release i386 x86_64
CONFIG+= qt release
#x86_64
 #CONFIG -= i386
#x86_64 static
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    myopenglwidget.cpp \
    imagecontainer.cpp \
    surfacecontainer.cpp \
    imagemanipulator.cpp \
    surfacemanipulator.cpp \
    scene_properties.cpp \
    scenewriter.cpp \
    struct_io.cpp \
    GlyphGenerator.cpp \
    vectorFunctions.cpp \
    colour_bar.cpp \
    surfaceslicer.cpp \
    marchingcubes_form.cpp \
    imagevideocapture.cpp \
    graphmanipulator.cpp \
    graphcontainer.cpp \
    glsl_editor.cpp
FSLDIR = /usr/local/fsl
INCLUDEPATH = ${FSLDIR}/include/ \
    ${HOME}/fslsrc/ \
    ${FSLDIR}/include/nifti \
    ${FSLDIR}/extras/include/newmat \
    ..//fslsurface \
#${FSLDIR}/include/giftiio \
    ${HOME}/fslsrc/expat/lib \
    /opt/local/include \
    /usr/local/include

  #  ${HOME}/fslsrc/nifti/include \

macx {
    QMAKE_INFO_PLIST = briview.plist
  #  LIBS +=-L/usr/local/Trolltech/Qt-4.7.2/lib/
    LIBS += -framework GLUT
    LIBS +=-L./\
    -L/usr/local/lib \
    -L./fslsurface/ \
    -L${HOME}/fslsrc/fslvtkio/ \
    -L${HOME}/fslsrc/expat/ \
    -L${HOME}/fslsrc/lib/ \
    -L${FSLDIR}/lib/ \
   # -L${HOME}/fslsrc/nifti/lib \
    -L${FSLDIR}/extras/lib/ \
    -L/opt/local/lib \
    #-L${GLUT_HOME}/lib/glut  \
    #-lfslsurfaceio \
    #-lfslsurfacefns \
    -lfslsurfacegl \
    -lfslsurface \
    -lgiftiio \
    -lnewimage \
    -lfslio \
    -lniftiio \
    -lfslvtkio \
    -lfirst_lib \
    -lmeshclass \
    -lmiscmaths \
    -lznz \
    -lnewmat \
    -lcprob \
    -lutils \
    -lz -lexpat \
    -ltiff
}

!macx {
    INCLUDEPATH += ${GLUT_HOME}/include
LIBS =-L./ \
    -L${FSLDIR}/lib/ \
    -L${HOME}/fslsrc/fslsurface/ \
    -L${HOME}/fslsrc/gifti/ \
    -L${HOME}/fslsrc/expat/ \
    -L${HOME}/fslsrc/nifti/lib \
    -L${FSLDIR}/extras/lib/ \
    -L${GLUT_HOME}/lib/glut  \
    -lfslsurface \
    -lgiftiio \
    -lnewimage \
    -lfslvtkio \
    -lfirst_lib \
    -lmeshclass \
    -lmiscmaths \
    -lfslio \
    -lniftiio \
    -lznz \
    -lnewmat \
    -lutils \
    -lz -lexpat
    LIBS += -L${GLUT_HOME}/lib/glut -lglut
}

HEADERS += mainwindow.h \
    myopenglwidget.h \
    surfacecontainer.h \
    imagecontainer.h \
    surfacecontainer.h \
    imagemanipulator.h \
    my_structs.h \
    misc.h \
    surfacemanipulator.h \
    scene_properties.h \
    scenewriter.h \
    struct_io.h \
    GlyphGenerator.h \
    briview_structs.h \
    vectorFunctions.h \
    colour_bar.h \
    surfaceslicer.h \
    marchingcubes_form.h \
    imagevideocapture.h \
    graphmanipulator.h \
    graphcontainer.h \
    glsl_editor.h
FORMS += mainwindow.ui \
    imagemanipulator.ui \
    surfacemanipulator.ui \
    surfacemanipulator.ui \
    scene_properties.ui \
    marchingcubes_form.ui \
    imagevideocapture.ui \
    graphmanipulator.ui \
    glsl_editor.ui
OTHER_FILES += glsl_shaders/image_plane_texture.vert \
    glsl_shaders/image_plane_texture.frag \
    glsl_shaders/surface_dir_light.frag \
    glsl_shaders/surface_dir_light_map_scalars.frag \
    glsl_shaders/surface_dir_light.vert \
    glsl_shaders/surface_dir_light_map_scalars.vert \
    glsl_shaders/image_plane_texture_cmap.frag \
    glsl_shaders/image_plane_texture_cmap.vert \
    glsl_shaders/colour_bar_map_scalars.vert \
    glsl_shaders/colour_bar_map_scalars.frag \
    glsl_shaders/surf_encode_rgb.vert \
    glsl_shaders/surf_encode_rgb.frag
GLSL_PROGRAMS.files = glsl_shaders/surface_dir_light.frag \
    glsl_shaders/surface_dir_light.vert
GLSL_PROGRAMS.files += glsl_shaders/surface_dir_light_map_scalars.frag \
    glsl_shaders/surface_dir_light_map_scalars.vert glsl_shaders/surface_dir_light_map_scalars_ctrthresh.vert
GLSL_PROGRAMS.files += glsl_shaders/image_plane_texture.frag \
    glsl_shaders/image_plane_texture.vert
GLSL_PROGRAMS.files += glsl_shaders/image_plane_texture_with_vertex.frag \
    glsl_shaders/image_plane_texture_with_vertex.vert

GLSL_PROGRAMS.files += glsl_shaders/image_plane_texture_cmap.frag \
    glsl_shaders/image_plane_texture_cmap.vert
GLSL_PROGRAMS.files += glsl_shaders/colour_bar_map_scalars.frag \
    glsl_shaders/colour_bar_map_scalars.vert
GLSL_PROGRAMS.files += glsl_shaders/image_plane_texture_ctrthresh.frag
GLSL_PROGRAMS.path = Contents/MacOS/glsl_shaders
PRESET_MATERIALS.files = assets/preset_materials.txt
PRESET_MATERIALS.path = Contents/MacOS/assets/
COLOUR_MAPS.files = assets/colour_maps.txt
COLOUR_MAPS.path = Contents/MacOS/assets/
QMAKE_BUNDLE_DATA += PRESET_MATERIALS
QMAKE_BUNDLE_DATA += GLSL_PROGRAMS
QMAKE_BUNDLE_DATA += COLOUR_MAPS
