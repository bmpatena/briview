#ifndef SCENEWRITER_H
#define SCENEWRITER_H
#include <string>
#include <fstream>

#include "scene_properties.h"
#include "surfacecontainer.h"

namespace briview
{

class sceneWriter
{
public:
    sceneWriter();
    sceneWriter(const std::string & filename);
    void writeSceneProperties( briview::scene_properties * scene);
     void writeSceneSurfaces( briview::SurfaceContainer* surfs );


    ~sceneWriter();

private:
    std::ofstream *f_scene;

};


class sceneReader
{
public:
    sceneReader();
    sceneReader(const std::string & filename);
    ~sceneReader();

    void readSceneProperties( briview::scene_properties * scene);
     void readSceneSurfaces( briview::SurfaceContainer* surfs );



private:
    std::ifstream *f_scene;

};


}
#endif // SCENEWRITER_H
