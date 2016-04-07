GIMP Plugin Engine Template
===========================

A template for a plugin for GIMP, where the plugin comprises:
- Python GUI wrapper plugin
- C-language engine plugin
- C++ language adaptor from GIMP to some other image processing library/framework (here OpenCV)
- inner crux of the algorithm (not included)

Engine plugin:
- without a GUI
- without menu item in GIMP
- doesn't run in interactive mode
- written in C/C++
- not concerned with last values
- registers in the PDB

Wrapper plugin:
- written in Python.
- has GUI provided by PyGimp
- deals with last values
- calls the C-language engine plugin via the PDB
- registers in the PDB

That is, the architecture is:

    Python plugin 
       => engine plugin 
          => adaptor from GIMP to image processing framework using C++ 
          => inner algorithm
          => image processing framework library (say OpenCV)
          => libgimp
      
This architecture separates concerns.
It is easier to express the GUI in a Python plugin using PyGimp.
The engine plugin is only concerned with registering itself,
adapting Gimp structures, and calling the image processing library.
The engine plugin is not concerned with GUI or internationalization.
The adaptor is only concerned with differences in image encoding and calling conventions.
The inner algorithm is not concerned with GIMP.
The inner algorithm could well be built as a separate library (in another project)
and linked with this plugin.

The template for the C-language engine plugin is a reduced version of gimp-plugin-template at gimp.org.
It is reduced:
- no internationalization (1l8n)
- no checking GTK versions for GUI
- no LASTVALS and defaults support
- no autogen.sh (smaller autotools input files: configure.ac, Makefile.am, etc.)

It is expanded:
- supports building with OpenCV

If you don't need or want this architecture (e.g. want GUI in your C-language plugin)
or if you want to write a plugin that might be accepted into the GIMP main repository,
use the original gimp-plugin-template.

With this architecture, you have choices where to implement many steps.
For example, you can preprocess in Python using PyGimp.
Some notions, such as 'user's selection' are best dealt with there.
You can also call image processing libraries that have Python bindings, such as OpenCV.
You can implement inner algorithms in C using the GIMP framework,
or using another framework that might be nearer to multi-processing.
You use C for low level algorithms that need speed.


Building using Eclipse
----------------------

This repository has an Eclipse project file.
To use the Eclipse project requires installing a JRE and Eclipse (from the Eclipse website.)
The Eclipse nature of the project is 'C/C++ autotools'.
You can build from within Eclipse, but to install, at a command line, cd to the project directory and 'sudo make install'
which installs to /usr/lib/gimp/2.0/plug-ins.
Or you can manually install to your personal GIMP at ~/.gimp-2.8/plug-ins, by copying gimp-plugin-engine-template.exe and plugin-engine-template.py, and making the latter executable.


Building without using Eclipse, from a command line
---------------------------------------------------

There is no autogen.sh in this repository.
Invoke autotools manually.
This MIGHT work:

    autoreconf
    ./configure
    make
    sudo make install

That might not work, because I often use Eclipse and I don't understand autotools well.

Dependencies
------------

Building requires packages for:
- autotools
- GIMP dev 
- OpenCV v2 dev

You can hack out the dependency on OpenCV by editing (to remove anything mentioning OpenCV):
- configure.ac, 
- src/Makefile.am


Substituting
------------

As a template, you can substitute to make a 'real' plugin that does something.
Places of interest are commented with //< or #<

To convert this template into a real plugin (substitute for the template placeholders) edit:
- src/main.c, redefine the macro definition placeholders
- src/render.c, substitute a call to your inner algorithm
- OR src/adaptOpenCV.cpp, insert a call to your inner algorithm
- pluginScripts/plugin-engine-template.py, substitute wherever you find #<

To change the lowermost image processing library, see above about removing the dependency on OpenCV,
and change the adaptor code.
Similarly if your innermost algorithm is built by you in a separate project as a library.

Also, changing the parameters (the API) to your inner engine require several changes.

You need to change the names under which the plugins register, 
but in general not the names of any source files or functions.


Testing
-------

After building and installing (without substitution) start GIMP and open an image.
You should find the Python plugin in the menus at 'Filters>Misc>Engine template...'.
When you choose that menu item, expect a dialog.
When you choose OK, expect an alert saying the plugin has finished but not actually done anything.

You should find the C-language plugin by browsing the PDB in GIMP at Filter>Python-Fu>Console ... Browse,
and searching for gimp-plugin-engine-template.
