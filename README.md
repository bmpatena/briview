briview
=======

This is an image and surface viewevr that is aimed at neuroimaging applications. This is all implemented in C++/OpenGL/Qt. The software provides a lot of flexibility in rendering options. This project and documentation is in development.

Some key features include:

- Surface rendering of VTK/GIFTI formats.
- Supports loading of NIFTI images in world coordinates and voxel coordinates.
- Orthoview screen capture as well as a flexible video capture facilities.
- Load and Saving of scenens (partially implemented).

#Requirements

* [libtiff](http://www.remotesensing.org/libtiff/v4.0.6.html)
* [mp4v2](https://code.google.com/p/mp4v2/)

#Manual 
====

##Menu
Most menus are dockable and hideable. This allows the flexibility to customs your interactive environment. For example, I like to have the camerca interactor floating.


##Menu Bar - File 
* **Load Image** : Loads a NIFTI 3D volumes for display (using orthogonal slices)
* **Load Surface** : Loads a surface file (GIFTI,PLY,VTK). A set of vertices, faces and other information.
* **Append Surface Data** : Appends surfaces  (GIFTI,PLY,VTK) to another. Can combine surfaces into a single file in this manner.
* **Save Surface** : Save surface to file.
* **save scene** : Save scene information. This is a shallow save. It includes the scene parameters (camera, lighting, background colour), surfaces (paths to surface files and rendering options), and images ( 3D volumes and rendering options).
* **load scene** : Loads scene include links and images. Requires that the files exist.
* **load graph** : Load a graph/network from file. Custom format (see graph section).



##Loading and Saving Data 

The "File" menu in the top left provides options for loading and saving surfaces, image and scene configuration files.

###comand line options
- -m <file> : load surface (GIFTI, VTK, PLY)
- -im <file> : load image volume (NIFTI, ANALYZE) 

##Navigating Image Volumes
Images are displayed using orthogonal volumes. Each orthogonal slice can be turn on/off. If viewing multiple images, it will uses the same slice and blend the images. How it is blended can be customizing. For example, one can have the image averaged.  
 
##To DO 
 
* Add shader selector and custom shaders, online compile
* glShadeModelFlat
* *toggle traingle sorting
* synchronize scalar sleectiomn, load proper scaklar selection when toggling surface, have embedde3d selected by default
=======
