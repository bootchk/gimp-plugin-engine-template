#!/usr/bin/env python

'''
Gimp plugin "Engine template"

An example, template plugin: 
-written in Python, 
-having an internationalized GUI, 
-wrapping an engine plugin written in C, without a GUI.

Copyright 2016 lloyd konneker (bootch at nc.rr.com)

License:

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  The GNU Public License is available at
  http://www.gnu.org/copyleft/gpl.html

'''

from gimpfu import *

gettext.install("engine-template", gimp.locale_directory, unicode=True)



def doFoo(timg, tdrawable, fooParam=50):
  '''
  Adapt to an inner engine.
  '''
  # typical GUI preamble
  pdb.gimp_image_undo_group_start(timg)
  
  # insert any image processing preamble here
  # typically, convert image mode, create new output image, etc.
  
  # This displays an alert dialog, but doesn't wait until dialog is acknowledged.
  pdb.gimp_message(_("Python engine template ran."))
  
  # Call wrapped, C-language, engine plugin
  # !!! Method name matches PROCEDURE_NAME that C-language engine registered
  # PyGimp creates methods of pdb object dynamically at GIMP start time
  pdb.gimp_plugin_engine_template(timg, tdrawable, fooParam)
  
  # Clean up (comment out to debug)
  # e.g. gimp.delete(tempImage)
  
  pdb.gimp_image_undo_group_end(timg)



# This mostly defines GUI for the plugin
# Note it is internationalized il8n
register(
  "python_fu_engine_template",
  N_("Template for wrapper for engine plugin"),
  "Requires separate C-language engine-template plugin.",
  "Lloyd Konneker",
  "2016 Lloyd Konneker",  # Copyright 
  "2016",
  N_("_Engine template..."),    # menu item
  "RGB*, GRAY*",
  [
    (PF_IMAGE, "image",       "Input image", None),
    (PF_DRAWABLE, "drawable", "Input drawable", None),
    (PF_INT, "fooParam", _("Foo:"), 50),
  ],
  [],
  doFoo,    # Python method that implements plugin
  menu="<Image>/Filters/Misc",  # parent menu
  domain=("engine-template", gimp.locale_directory)
  )

main()

