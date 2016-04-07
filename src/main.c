/* GIMP Engine Plug-in Template
 * Copyright (C) 2000  Michael Natterer <mitch@gimp.org> (the "Author").
 * Copyright 2016 Lloyd Konneker
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Author of the
 * Software shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the Author.
 */


#include <string.h>
#include <libgimp/gimp.h>

#include "render.h"



//< Customize here
//< This defines values for placeholders in the template.
#define PROCEDURE_NAME         "gimp_plugin_engine_template"	//< underscore, not hyphen
#define PROCEDURE_ARGS_COUNT   4		// Count args including 3 canonical args
#define PROCEDURE_BLURB        "Blurb"	  // Description of effects
#define PROCEDURE_HELP	       "Help"     // More description
#define PROCEDURE_AUTHOR       "Lloyd Konneker <edress>"  // Used twice: author AND copyright holder
#define PROCEDURE_DATES	       "2016-2016"	     // Copyright dates
#define PROCEDURE_IMAGE_TYPES  "RGB*, GRAY*, INDEXED*"	// image modes supported by plugin


/*  Local function prototypes  */

static void   query (void);
static void   run   (const gchar      *name,
		     gint              nparams,
		     const GimpParam  *param,
		     gint             *nreturn_vals,
		     GimpParam       **return_vals);


// No structs for last called vals (actual parameters)


GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

MAIN ()

static void
query (void)
{
  static GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    "run_mode",   "Interactive, non-interactive"    },
    { GIMP_PDB_IMAGE,    "image",      "Input image"                     },
    { GIMP_PDB_DRAWABLE, "drawable",   "Input drawable"                  },
    { GIMP_PDB_INT32,    "dummy",      "dummy1"                          },
  };


  gimp_install_procedure (
		  PROCEDURE_NAME,
		  PROCEDURE_BLURB,
		  PROCEDURE_HELP,
		  PROCEDURE_AUTHOR,
		  PROCEDURE_AUTHOR,
		  PROCEDURE_DATES,
		  "",	// No menu item
		  PROCEDURE_IMAGE_TYPES ,
		  GIMP_PLUGIN,	// enum defined in libgimp
		  G_N_ELEMENTS (args), 0,
		  args, NULL);

  // engine plugin not registered in menus, no call to ...register_menu....
}


static void
run (const gchar      *name,
     gint              n_params,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam   values[1];
  GimpDrawable      *drawable;
  gint32             image_ID;
  GimpRunMode        run_mode;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;

  *nreturn_vals = 1;
  *return_vals  = values;

  run_mode = param[0].data.d_int32;
  image_ID = param[1].data.d_int32;
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  static PlugInVals         vals;  // Not initialized with default values, no GUI

  if (strcmp (name, PROCEDURE_NAME) == 0)
    {
      switch (run_mode)
      {
	case GIMP_RUN_NONINTERACTIVE:
	  if (n_params != PROCEDURE_ARGS_COUNT) { status = GIMP_PDB_CALLING_ERROR; }
	  else
	    {
	      //< flesh out instance of extra args to render()
	      vals.dummy1      = param[3].data.d_int32;
	    }
	  break;

	case GIMP_RUN_INTERACTIVE:
	case GIMP_RUN_WITH_LAST_VALS:
	  // illegal to call in interactive mode because this plugin is an engine w/o gui
	  status = GIMP_PDB_CALLING_ERROR;
	  break;

	default:
	  break;
      }
    }
  else { status = GIMP_PDB_CALLING_ERROR; }

  if (status == GIMP_PDB_SUCCESS)
    {
      g_assert(run_mode == GIMP_RUN_NONINTERACTIVE);
      render (image_ID, drawable, &vals);
      // result is not visible to user, display not flushed since non-interactive
      gimp_drawable_detach (drawable);
    }

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
}
