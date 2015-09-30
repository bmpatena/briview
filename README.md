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

#Usage

##Menu
Most menus are dockable and hideable. This allows the flexibility to customs your interactive environment. For example, I like to have the camerca interactor floating.

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
