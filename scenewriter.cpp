#include "scenewriter.h"
#include <iostream>


using namespace std;


namespace briview
{
sceneWriter::sceneWriter()
{
    f_scene = new ofstream();
}
sceneWriter::sceneWriter(const string & filename)
{
    f_scene = new ofstream(filename.c_str());

}

void sceneWriter::writeSceneProperties( scene_properties * scene)
{
    if (f_scene->is_open())
    {
        cout<<"write scene properties"<<endl;
                scene->writeSceneProperties(f_scene);
    }

}
     void sceneWriter::writeSceneSurfaces( SurfaceContainer* surfs )
     {
        if (f_scene->is_open())
          {
                cout<<"write scene properties"<<endl;
                surfs->writeSurfaces(f_scene);
          }
     }
     void sceneWriter::writeSceneImages( briview::ImageContainer* ims )
     {
    	    if (f_scene->is_open())
    	          {
    	                cout<<"write scene properties"<<endl;
    	                ims->writeImages(f_scene);
    	          }
     }


sceneWriter::~sceneWriter()
{
        if (f_scene->is_open())
              f_scene->close();
        delete f_scene;
}

//--------------------------------SCENE READER NOW---------------------------//
sceneReader::sceneReader()
{

//    f_scene = new ifstream();
}
sceneReader::sceneReader(const string & filename)
{
	filename_ = filename;
//    f_scene = new ifstream(filename.c_str());

}

void sceneReader::readSceneProperties( scene_properties * scene)
{

	if (! filename_.empty())
	{
		ifstream* f_scene = new ifstream(filename_.c_str());
		    if (f_scene->is_open())
		    {
		        cout<<"read scene properties"<<endl;
		        scene->readSceneProperties(f_scene);
		    }
			f_scene->close();
				delete f_scene;
	}

}
void sceneReader::readSceneSurfaces( SurfaceContainer* surfs )
{
	if (! filename_.empty())
	{
		ifstream* f_scene = new ifstream(filename_.c_str());
		if (f_scene->is_open())
		{
			cout<<"read scene surface (not enabled yet)"<<endl;
			surfs->readSurfacesFromScene(f_scene);
		}
		f_scene->close();
		delete f_scene;
	}

}

void sceneReader::readSceneImages( ImageContainer* surfs )
{
	if (! filename_.empty())
	{
		ifstream* f_scene = new ifstream(filename_.c_str());

		if (f_scene->is_open())
		{
			cout<<"read scene images (not enabled yet)"<<endl;
			//  ims->readSurfaces(f_scene);
		}
		f_scene->close();
			delete f_scene;	}

}

sceneReader::~sceneReader()
{
//        if (f_scene->is_open())
//              f_scene->close();
//        delete f_scene;
}





}
