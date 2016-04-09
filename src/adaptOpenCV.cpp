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

#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>
#include "adaptOpenCV.hpp"




void renderUsingOpenCV (
    gint32        image_ID,
    GimpDrawable *drawable)
{
    gint         channels;
    gint         left, upper;
    gint         width, height;

    guchar      *data;
    cv::Mat	mat;

    g_message ("Rendering using OpenCV.");

    getUpperLeftWidthHeightOfDrawable(drawable, &left, &upper, &width, &height);
    channels = gimp_drawable_bpp (drawable->drawable_id);
    data = g_new (guchar, channels * width * height);	// Allocate
    mat = adaptDrawableToOpenCV(drawable, data, left, upper, width, height);
    // findDust(&mat);      //< insert image processing function(OpenCV Mat)
    adaptOpenCVToDrawable(drawable, data, left, upper, width, height);
    g_free (data);
    return;
}


void
getUpperLeftWidthHeightOfDrawable (
    GimpDrawable *drawable,
    gint* leftX,
    gint* upperY,
    gint* width,
    gint* height
)
{
  gint x1, y1, x2, y2;

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

  // results
  *leftX = x1;
  *upperY = y1;

  *width = x2 - x1;
  *height = y2 - y1;
}

// Read rect from GIMP and wrap rect in Mat type
cv::Mat
adaptDrawableToOpenCV (
    GimpDrawable *drawable,
    guchar *rect,
    gint left,
    gint upper,
    gint width,
    gint height)
{
  GimpPixelRgn rgn_in;

  // Region for reading
  gimp_pixel_rgn_init (&rgn_in, drawable, left, upper, width, height, FALSE, FALSE);

  // Read from rgn into rect
  gimp_pixel_rgn_get_rect (&rgn_in, rect, left, upper, width, height);

  // Create OpenCV matrix object from raw data.
  // GIMP and OpenCV uses same data type (sequence of bytes in pixel order).
  // Note rect is not copied.
  return cv::Mat(height, width, CV_8UC3, rect);

}

// write modified raw data to GIMP region
void
adaptOpenCVToDrawable (
    GimpDrawable *drawable,
    guchar *rect,
    gint left,
    gint upper,
    gint width,
    gint height
)
{
  GimpPixelRgn rgn_out;

  // create region for writing back
  gimp_pixel_rgn_init (&rgn_out, drawable, left, upper, width, height, TRUE, TRUE);

  // write raw data into region
  gimp_pixel_rgn_set_rect (&rgn_out, rect, left, upper, width, height);

  // Update drawable from region.  Standard code.
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, left, upper, width, height);
}
