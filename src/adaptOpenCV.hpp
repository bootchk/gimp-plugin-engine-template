

// Public

// extern "C" so that this C++ function is callable from C
extern "C" void renderUsingOpenCV (
    gint32        image_ID,
    GimpDrawable *drawable
);


// Private

void
getUpperLeftWidthHeightOfDrawable (
    GimpDrawable *drawable,
    gint* leftX,
    gint* upperY,
    gint* width,
    gint* height
);

cv::Mat
adaptDrawableToOpenCV (
    GimpDrawable *drawable,
    guchar *rect,
    gint left,
    gint upper,
    gint width,
    gint height);

void
adaptOpenCVToDrawable (
    GimpDrawable *drawable,
    guchar *rect,
    gint left,
    gint upper,
    gint width,
    gint height
);
