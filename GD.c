/*************************************************************

  GD.c - Ruby extension library to use Boutell's gd library.

  Originally written: Yukihiro Matsumoto (matz@ruby-lang.org)
                                           
  Current maintainer: Ryuichi Tamura (tam@kais.kyoto-u.ac.jp)
  
  $Date: 2001/05/30 14:06:40 $
  $Revision: 1.6.2.3 $
**************************************************************/
#include "ruby.h"
#if defined(HAVE_RUBY_IO_H)
        #include "ruby/io.h"
#else /* seems like Ruby < 1.9 */
        #include "rubyio.h"
#endif

#include "gd.h"
#include "gdfontg.h"		/* giant */
#include "gdfontl.h"		/* large */
#include "gdfontmb.h"		/* medium bold */
#include "gdfonts.h"		/* small */
#include "gdfontt.h"		/* tiny */

/* Is this the best way to do this sort of thing?
 * It feels kind of wrong, but I'm not sure of the (Ruby)C way.
 * Any opinions on the style will be gratefully received! */

#if defined(HAVE_RUBY_IO_H) /* seems like Ruby > 1.8 */
        #define FPTR_TYPE rb_io_t
        #define FILE_POINTER_FOR_CREATE_FROM rb_io_stdio_file(fptr)
        #define FPTR_PATH (char*)fptr->pathv
        #define SECOND_FILE_POINTER rb_io_stdio_file(fptr)
#else
        #define FPTR_TYPE OpenFile
        #define FILE_POINTER_FOR_CREATE_FROM fptr->f
        #define FPTR_PATH fptr->path
        #define SECOND_FILE_POINTER (fptr->f2) ? fptr->f2 : fptr->f
#endif

extern VALUE rb_io_binmode(VALUE io);
extern gdImagePtr gdImageCreateFromXpm(char* );

static VALUE mGD, cImage, cFont, cPolygon;

#ifdef ENABLE_GD_2_0
static VALUE
is_truecolor(im)
    gdImagePtr im;
{
    return im->trueColor ? Qtrue : Qfalse;
}
#endif /* ENABLE_GD_2_0 */

static void
free_img(iptr)
    gdImagePtr iptr;
{
    if (iptr) {
	gdImageDestroy(iptr);
    }
}

static VALUE
img_s_new(klass, w, h)
    VALUE klass, w, h;
{
    gdImagePtr iptr;

    if (NUM2INT(w)<0 || NUM2INT(h)<0)
    	rb_raise(rb_eArgError, "Negative width/height not allowed");

    iptr = gdImageCreate(NUM2INT(w), NUM2INT(h));
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}


static VALUE
img_from_pngfname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromPng(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid PNG File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}


static VALUE
img_from_png(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE); 
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromPng(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid PNG File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

#ifdef ENABLE_GD_2_0
static VALUE
img_from_giffname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGif(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid GIF File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gif(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE); 
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGif(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid GIF File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}
#endif /* ENABLE_GD_2_0 */

static VALUE
img_from_gdfname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gd(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE); 
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd File", FPTR_PATH);
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gd2fname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd2(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd2 File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gd2(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE);
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd2(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd2 File", FPTR_PATH);
    
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gd2_partfname(klass, fname, srcx, srcy, w, h)
    VALUE klass, fname, srcx, srcy, w, h;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd2Part(FILE_POINTER_FOR_CREATE_FROM, NUM2INT(srcx),
                                    NUM2INT(srcy), NUM2INT(w), NUM2INT(h));
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd2 File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_gd2_part(klass, f, srcx, srcy, w, h)
    VALUE klass, f, srcx, srcy, w, h;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE);
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromGd2Part(FILE_POINTER_FOR_CREATE_FROM, NUM2INT(srcx),
                                   NUM2INT(srcy), NUM2INT(w), NUM2INT(h));
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Gd2 File", FPTR_PATH);
    
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}


static VALUE
img_from_xbm(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE); 
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromXbm(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Xbm File", FPTR_PATH);

    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_xbmfname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromXbm(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Xbm File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

#ifdef HAVE_GDIMAGECREATEFROMXPM
static VALUE
img_from_xpm(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE);
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    /* need cast, and the argument is char* type */
    iptr = (gdImagePtr)gdImageCreateFromXpm(fptr->path);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid XPM File", (char*)fptr->path);

    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_xpmfname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    /* need cast, and the argument is char* type */
    iptr = (gdImagePtr)gdImageCreateFromXpm(fptr->path);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid XPM File", (char*)fptr->path);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

#endif

#ifdef HAVE_GDIMAGECREATEFROMJPEG
static VALUE
img_from_jpeg(klass, f)
    VALUE klass, f;
{
    FPTR_TYPE *fptr;
    gdImagePtr iptr;

    Check_Type(f, T_FILE);
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);
    
    iptr = gdImageCreateFromJpeg(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Jpeg File", FPTR_PATH);

    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_from_jpegfname(klass, fname)
    VALUE klass, fname;
{
    VALUE f;
    FPTR_TYPE *fptr;
    gdImagePtr iptr;
    
    Check_Type(fname, T_STRING);

    f = rb_file_open(STR2CSTR(fname), "r");
    rb_io_binmode(f);
    GetOpenFile(f, fptr);
    rb_io_check_readable(fptr);

    iptr = gdImageCreateFromJpeg(FILE_POINTER_FOR_CREATE_FROM);
    if (!iptr)
        rb_raise(rb_eArgError, "%s is not a valid Jpeg File", FPTR_PATH);
                 
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}
#endif


static VALUE
img_destroy(img)
    struct RData *img;
{
    if (img->data) {
	gdImageDestroy((gdImagePtr)img->data);
	img->data = 0;
    }
    return Qnil;
}

static VALUE
hex2triplet(hex)
    VALUE hex;
{
    VALUE rstr, gstr, bstr;
    VALUE ret_ary;

    Check_Type(hex, T_STRING);
    
    if (RSTRING_LEN(hex) != 7)
        rb_raise(rb_eArgError, "Invalid format: %s", RSTRING_PTR(hex));

    rstr = rb_str_new(RSTRING_PTR(hex) + 1, 2);
    gstr = rb_str_new(RSTRING_PTR(hex) + 3, 2);
    bstr = rb_str_new(RSTRING_PTR(hex) + 5, 2);

    ret_ary = rb_ary_new();
	
    rb_ary_push(ret_ary, rb_funcall(rstr, rb_intern("hex"), 0));
    rb_ary_push(ret_ary, rb_funcall(gstr, rb_intern("hex"), 0));
    rb_ary_push(ret_ary, rb_funcall(bstr, rb_intern("hex"), 0));

    return ret_ary;
}

static VALUE
img_color_allocate_tri(img, r, g, b)
    VALUE img, r, g, b;
{
    gdImagePtr im;
    int c;
    
    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorAllocate(im, NUM2INT(r), NUM2INT(g), NUM2INT(b));
    
    return INT2FIX(c);
}

static VALUE
img_color_allocate_str(img, rgbstr)
    VALUE img, rgbstr;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);
    
    ary = hex2triplet(rgbstr);
    c = gdImageColorAllocate(im,
                             NUM2INT(*(RARRAY_PTR(ary))),
                             NUM2INT(*(RARRAY_PTR(ary)+1)),
                             NUM2INT(*(RARRAY_PTR(ary)+2)));
    return INT2FIX(c);
}

static VALUE
img_color_allocate(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_color_allocate_str(img, rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_color_allocate_tri(img, r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

static VALUE
img_color_deallocate(img, color)
    VALUE img, color;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageColorDeallocate(im, NUM2INT(color));

    return img;
}

#ifdef HAVE_GDIMAGECOLORRESOLVE
static VALUE
img_color_resolve_tri(img, r, g, b)
    VALUE img, r, g, b;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorResolve(im, NUM2INT(r), NUM2INT(g), NUM2INT(b));

    return INT2FIX(c);
}

static VALUE
img_color_resolve_str(img, rgbstr)
    VALUE img, rgbstr;
{
    gdImagePtr im;
    int c;
    VALUE ary;
    
    Data_Get_Struct(img, gdImage, im);

    ary = hex2triplet(rgbstr);
    c = gdImageColorResolve(im,
                            NUM2INT(*(RARRAY_PTR(ary))),
                            NUM2INT(*(RARRAY_PTR(ary)+1)),
                            NUM2INT(*(RARRAY_PTR(ary)+2)));

    return INT2FIX(c);
}

static VALUE
img_color_resolve(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);

    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_color_resolve_str(img, rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_color_resolve_tri(img, r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

#endif /* HAVE_GDIMAGECOLORRESOLVE */

static VALUE
img_color_closest_tri(img, r, g, b)
    VALUE img, r, g, b;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorClosest(im, NUM2INT(r), NUM2INT(g), NUM2INT(b));
                
    return INT2FIX(c);
}

static VALUE
img_color_closest_str(img, rgbstr)
    VALUE img, rgbstr;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);

    ary = hex2triplet(rgbstr);
    c = gdImageColorClosest(im,
                            NUM2INT(*(RARRAY_PTR(ary))),
                            NUM2INT(*(RARRAY_PTR(ary)+1)),
                            NUM2INT(*(RARRAY_PTR(ary)+2)));
    return INT2FIX(c);
}

static VALUE
img_color_closest(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);

    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_color_closest_str(img, rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_color_closest_tri(img, r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

extern int gdImageColorClosestHWB(gdImagePtr, int, int, int);

static VALUE
img_color_closestHWB_tri(img, r, g, b)
    VALUE img, r, g, b;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorClosestHWB(im, NUM2INT(r), NUM2INT(g), NUM2INT(b));
                
    return INT2FIX(c);
}

static VALUE
img_color_closestHWB_str(img, rgbstr)
    VALUE img, rgbstr;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);

    ary = hex2triplet(rgbstr);
    c = gdImageColorClosestHWB(im,
                               NUM2INT(*(RARRAY_PTR(ary))),
                               NUM2INT(*(RARRAY_PTR(ary)+1)),
                               NUM2INT(*(RARRAY_PTR(ary)+2)));
    return INT2FIX(c);
}

static VALUE
img_color_closestHWB(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);

    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_color_closestHWB_str(img, rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_color_closestHWB_tri(img, r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

static VALUE
img_color_exact_tri(img, r, g, b)
    VALUE img, r, g, b;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorExact(im, NUM2INT(r), NUM2INT(g), NUM2INT(b));
                
    return INT2FIX(c);
}

static VALUE
img_color_exact_str(img, rgbstr)
    VALUE img, rgbstr;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);

    ary = hex2triplet(rgbstr);
    c = gdImageColorExact(im,
                          NUM2INT(*(RARRAY_PTR(ary))),
                          NUM2INT(*(RARRAY_PTR(ary)+1)),
                          NUM2INT(*(RARRAY_PTR(ary)+2)));
    return INT2FIX(c);
}

static VALUE
img_color_exact(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);

    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_color_exact_str(img, rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_color_exact_tri(img, r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

static VALUE
img_colors_total(img)
    VALUE img;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorsTotal(im);

    return INT2FIX(c);
}

static VALUE
img_get_pixel(img, x, y)
    VALUE img, x, y;
{
    gdImagePtr im;
    int c;

    Data_Get_Struct(img, gdImage, im);
    c = gdImageGetPixel(im, NUM2INT(x), NUM2INT(y));

    return INT2FIX(c);
}

static VALUE
img_set_pixel(img, x, y, color)
    VALUE img, x, y, color;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageSetPixel(im, NUM2INT(x), NUM2INT(y), NUM2INT(color));
                
    return img;
}

static VALUE
img_red(img, idx)
    VALUE img, idx;
{
    gdImagePtr im;
    int i,c;

    Data_Get_Struct(img, gdImage, im);
    i = NUM2INT(idx);
    
    c = gdImageRed(im, i);

    return INT2FIX(c);
}

static VALUE
img_green(img, idx)
    VALUE img, idx;
{
    gdImagePtr im;
    int i,c;

    Data_Get_Struct(img, gdImage, im);
    i = NUM2INT(idx);
    
    c = gdImageGreen(im, i);

    return INT2FIX(c);
}

static VALUE
img_blue(img, idx)
    VALUE img, idx;
{
    gdImagePtr im;
    int i,c;

    Data_Get_Struct(img, gdImage, im);
    i = NUM2INT(idx);
    
    c = gdImageBlue(im, i);

    return INT2FIX(c);
}

static VALUE
img_rgb(img, idx)
    VALUE img, idx;
{
    gdImagePtr im;
    VALUE ary = rb_ary_new2(3);
    int i, c;

    Data_Get_Struct(img, gdImage, im);
    i = NUM2INT(idx);

    c = gdImageRed(im, i);
    rb_ary_push(ary, INT2FIX(c));
    c = gdImageGreen(im, i);
    rb_ary_push(ary, INT2FIX(c));
    c = gdImageBlue(im, i);
    rb_ary_push(ary, INT2FIX(c));

    return ary;
}

static VALUE
img_transparent(img, idx)
    VALUE img, idx;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);

    gdImageColorTransparent(im, NUM2INT(idx));

    return img;
}

static void
image_req(img)
    VALUE img;
{
    if (!rb_obj_is_kind_of(img, cImage)) {
	rb_raise(rb_eTypeError, "GD::Image required");
    }
}

static VALUE
img_set_blush(img, brush)
    VALUE img, brush;
{
    gdImagePtr im, br;

    Data_Get_Struct(img, gdImage, im);
    image_req(brush);
    Data_Get_Struct(brush, gdImage, br);
    gdImageSetBrush(im, br);

    return img;
}

static VALUE
img_set_style(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    gdImagePtr im;
    int *style;
    int i;

    Data_Get_Struct(img, gdImage, im);
    style = ALLOCA_N(int, argc);
    for (i=0; i<argc; i++) {
	style[i] = NUM2INT(argv[i]);
    }

    gdImageSetStyle(im, style, argc);

    return img;
}

static VALUE
img_set_tile(img, tile)
    VALUE img, tile;
{
    gdImagePtr im, ti;

    Data_Get_Struct(img, gdImage, im);
    image_req(tile);
    Data_Get_Struct(tile, gdImage, ti);
    gdImageSetTile(im, ti);

    return img;
}

static VALUE
img_line(img, x1, y1, x2, y2, c)
    VALUE img, x1, y1, x2, y2, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageLine(im,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(y2),NUM2INT(c));

    return img;
}

static VALUE
img_dashed_line(img, x1, y1, x2, y2, c)
    VALUE img, x1, y1, x2, y2, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageDashedLine(im,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(y2),NUM2INT(c));

    return img;
}


static VALUE
img_rectangle(img, x1, y1, x2, y2, c)
    VALUE img, x1, y1, x2, y2, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageRectangle(im,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(y2),NUM2INT(c));

    return img;
}


static VALUE
img_filled_rectangle(img, x1, y1, x2, y2, c)
    VALUE img, x1, y1, x2, y2, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageFilledRectangle(im,NUM2INT(x1),NUM2INT(y1),NUM2INT(x2),NUM2INT(y2),NUM2INT(c));

    return img;
}

static void
poly_req(ply)
    VALUE ply;
{
    if (!rb_obj_is_kind_of(ply, cPolygon) || TYPE(ply) != T_ARRAY) {
	rb_raise(rb_eTypeError, "GD::Polygon required");
    }
}

static VALUE
img_polygon(img, ply, c)
    VALUE img;
    struct RArray *ply;
    VALUE c;
{
    gdImagePtr im;
    gdPointPtr pnt;
    int i, len;

    Data_Get_Struct(img, gdImage, im);

    poly_req(ply);
    len = RARRAY_LEN(ply)/2;
    pnt = ALLOCA_N(gdPoint, len);
    for (i=0; i<len; i++) {
	pnt[i].x = NUM2INT(RARRAY_PTR(ply)[i*2]);
	pnt[i].y = NUM2INT(RARRAY_PTR(ply)[i*2+1]);
    }

    gdImagePolygon(im, pnt, len, NUM2INT(c));

    return img;
}

static VALUE
img_filled_polygon(img, ply, c)
    VALUE img;
    struct RArray *ply;
    VALUE c;
{
    gdImagePtr im;
    gdPointPtr pnt;
    int i, len;

    Data_Get_Struct(img, gdImage, im);

    poly_req(ply);
    len = RARRAY_LEN(ply)/2;
    pnt = ALLOCA_N(gdPoint, len);
    for (i=0; i<len; i++) {
	pnt[i].x = NUM2INT(RARRAY_PTR(ply)[i*2]);
	pnt[i].y = NUM2INT(RARRAY_PTR(ply)[i*2+1]);
    }

    gdImageFilledPolygon(im, pnt, len, NUM2INT(c));

    return img;
}

static VALUE
img_arc(img, cx, cy, w, h, s, e, c)
    VALUE img, cx, cy, w, h, s, e, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageArc(im,NUM2INT(cx),NUM2INT(cy),NUM2INT(w),NUM2INT(h),NUM2INT(s),NUM2INT(e),NUM2INT(c));

    return img;
}

static VALUE
img_fill(img, x, y, c)
    VALUE img, x, y, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageFill(im,NUM2INT(x),NUM2INT(y),NUM2INT(c));

    return img;
}

static VALUE
img_fill_to_border(img, x, y, b, c)
    VALUE img, x, y, b, c;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageFillToBorder(im,NUM2INT(x),NUM2INT(y),NUM2INT(b),NUM2INT(c));

    return img;
}

static VALUE
img_copy(img, img2, dx, dy, sx, sy, w, h)
    VALUE img, img2, dx, dy, sx, sy, w, h;
{
    gdImagePtr im, im2;

    Data_Get_Struct(img, gdImage, im);
    image_req(img2);
    Data_Get_Struct(img2, gdImage, im2);

#ifdef ENABLE_GD_2_0    
    if (is_truecolor(im) && (!is_truecolor(im2))){
        rb_raise(rb_eRuntimeError,
                 "Copying truecolor image to palette image is not permitted");
    }
#endif
        
    gdImageCopy(im2, im,
                NUM2INT(dx), NUM2INT(dy),
                NUM2INT(sx), NUM2INT(sy),
                NUM2INT(w),NUM2INT(h));

    return img;
}

static VALUE
img_copy_resized(img, img2, dx, dy, sx, sy, dw, dh, sw, sh)
    VALUE img, img2, dx, dy, sx, sy, dw, dh, sw, sh;
{
    gdImagePtr im, im2;

    Data_Get_Struct(img, gdImage, im);
    image_req(img2);
    Data_Get_Struct(img2, gdImage, im2);

#ifdef ENABLE_GD_2_0    
    if (is_truecolor(im) && (!is_truecolor(im2))){
        rb_raise(rb_eRuntimeError,
                 "Copying truecolor image to palette image is not permitted");
    }
#endif

    gdImageCopyResized(im2,im,NUM2INT(dx),NUM2INT(dy),NUM2INT(sx),NUM2INT(sy),NUM2INT(dw),NUM2INT(dh),NUM2INT(sw),NUM2INT(sh));

    return img;
}

static VALUE
img_copy_merge(img, img_dest, dx, dy, sx, sy, w, h, pct)
    VALUE img, img_dest, dx, dy, sx, sy, w, h, pct;
{
    gdImagePtr im, im_dest;

    Data_Get_Struct(img, gdImage, im);
    image_req(img_dest);
    Data_Get_Struct(img_dest, gdImage, im_dest);

#ifdef ENABLE_GD_2_0    
    if (is_truecolor(im) && (!is_truecolor(im_dest))){
        rb_raise(rb_eRuntimeError,
                 "Copying truecolor image to palette image is not permitted");
    }
#endif
    
    gdImageCopyMerge(im_dest,im,NUM2INT(dx),NUM2INT(dy),NUM2INT(sx),NUM2INT(sy),NUM2INT(w),NUM2INT(h), NUM2INT(pct));

    return img;
}

static VALUE
img_copy_merge_gray(img, img_dest, dx, dy, sx, sy, w, h, pct)
    VALUE img, img_dest, dx, dy, sx, sy, w, h, pct;
{
    gdImagePtr im, im_dest;

    Data_Get_Struct(img, gdImage, im);
    image_req(img_dest);
    Data_Get_Struct(img_dest, gdImage, im_dest);

#ifdef ENABLE_GD_2_0    
    if (is_truecolor(im) && (!is_truecolor(im_dest))){
        rb_raise(rb_eRuntimeError,
                 "Copying truecolor image to palette image is not permitted");
    }
#endif
    
    gdImageCopyMergeGray(im_dest,im,NUM2INT(dx),NUM2INT(dy),NUM2INT(sx),NUM2INT(sy),NUM2INT(w),NUM2INT(h), NUM2INT(pct));

    return img;
}
    
static VALUE
img_palette_copy(img, img2)
    VALUE img, img2;
{
    gdImagePtr im, im2;

    image_req(img2);
    Data_Get_Struct(img, gdImage, im);
    Data_Get_Struct(img2, gdImage, im2);
    
    gdImagePaletteCopy(im, im2);

    return img;
}

static void
font_req(fnt)
    VALUE fnt;
{
    if (!rb_obj_is_kind_of(fnt, cFont)) {
	rb_raise(rb_eTypeError, "GD::Font required");
    }
}

static VALUE
img_string(img, fnt, x, y, str, c)
    VALUE img, fnt, x, y, str, c;
{
    gdImagePtr im;
    gdFontPtr  f;

    Check_Type(str, T_STRING);
    Data_Get_Struct(img, gdImage, im);
    font_req(fnt);
    Data_Get_Struct(fnt, gdFont, f);

    gdImageString(im,f,NUM2INT(x),NUM2INT(y),(unsigned char*)RSTRING_PTR(str),NUM2INT(c));

    return img;
}

static VALUE
img_string_up(img, fnt, x, y, str, c)
    VALUE img, fnt, x, y, str, c;
{
    gdImagePtr im;
    gdFontPtr  f;

    Check_Type(str, T_STRING);
    Data_Get_Struct(img, gdImage, im);
    font_req(fnt);
    Data_Get_Struct(fnt, gdFont, f);

    gdImageStringUp(im,f,NUM2INT(x),NUM2INT(y),(unsigned char*)RSTRING_PTR(str),NUM2INT(c));

    return img;
}


#ifdef HAVE_GDIMAGESTRINGTTF
static VALUE
img_s_string_ttf(klass, fgcolor, fontname, ptsize, angle, x, y, string)
    VALUE klass, fgcolor, fontname, ptsize, angle, x, y, string;
{
    int brect[8], i;
    char *msg;
    VALUE ary = rb_ary_new2(8);

    Check_Type(fontname, T_STRING);
    Check_Type(string, T_STRING);

    msg = gdImageStringTTF(NULL,
                           &brect[0],
                           NUM2INT(fgcolor),
                           RSTRING_PTR(fontname),
                           NUM2DBL(ptsize),
                           NUM2DBL(angle),
                           NUM2INT(x),
                           NUM2INT(y),
                           RSTRING_PTR(string));
    for (i=0; i<8; i++) {
        rb_ary_push(ary, INT2FIX(brect[i]));
    }
    if (msg) {
        return rb_ary_new3(2, rb_str_new2(msg), ary);
    } else {
        return rb_ary_new3(2, Qnil, ary);
    }
}

static VALUE
img_string_ttf(img, fgcolor, fontname, ptsize, angle, x, y, string)
    VALUE img, fgcolor, fontname, ptsize, angle, x, y, string;
{
    gdImagePtr im;
    int brect[8], i;
    char *msg;
    VALUE ary = rb_ary_new2(8);

    Check_Type(fontname, T_STRING);
    Check_Type(string, T_STRING);

    Data_Get_Struct(img, gdImage, im);
    msg = gdImageStringTTF(im,
                           &brect[0],
                           NUM2INT(fgcolor),
                           RSTRING_PTR(fontname),
                           NUM2DBL(ptsize),
                           NUM2DBL(angle),
                           NUM2INT(x),
                           NUM2INT(y),
                           RSTRING_PTR(string));
    for (i=0; i<8; i++) {
        rb_ary_push(ary, INT2FIX(brect[i]));
    }
    if (msg) {
        return rb_ary_new3(2, rb_str_new2(msg), ary);
    } else {
        return rb_ary_new3(2, Qnil, ary);
    }
}
#endif /* HAVE_GDIMAGESTRINGTTF */

#ifdef HAVE_GDIMAGESTRINGFT
static VALUE
img_s_string_ft(klass, fgcolor, fontname, ptsize, angle, x, y, string)
    VALUE klass, fgcolor, fontname, ptsize, angle, x, y, string;
{
    int brect[8], i;
    char *msg;
    VALUE ary = rb_ary_new2(8);

    Check_Type(fontname, T_STRING);
    Check_Type(string, T_STRING);

    msg = gdImageStringFT(NULL,
                           &brect[0],
                           NUM2INT(fgcolor),
                           RSTRING_PTR(fontname),
                           NUM2DBL(ptsize),
                           NUM2DBL(angle),
                           NUM2INT(x),
                           NUM2INT(y),
                           RSTRING_PTR(string));
    for (i=0; i<8; i++) {
        rb_ary_push(ary, INT2FIX(brect[i]));
    }
    if (msg) {
        return rb_ary_new3(2, rb_str_new2(msg), ary);
    } else {
        return rb_ary_new3(2, Qnil, ary);
    }
}

static VALUE
img_string_ft(img, fgcolor, fontname, ptsize, angle, x, y, string)
    VALUE img, fgcolor, fontname, ptsize, angle, x, y, string;
{
    gdImagePtr im;
    int brect[8], i;
    char *msg;
    VALUE ary = rb_ary_new2(8);

    Check_Type(fontname, T_STRING);
    Check_Type(string, T_STRING);

    Data_Get_Struct(img, gdImage, im);
    msg = gdImageStringFT(im,
                           &brect[0],
                           NUM2INT(fgcolor),
                           RSTRING_PTR(fontname),
                           NUM2DBL(ptsize),
                           NUM2DBL(angle),
                           NUM2INT(x),
                           NUM2INT(y),
                           RSTRING_PTR(string));
    for (i=0; i<8; i++) {
        rb_ary_push(ary, INT2FIX(brect[i]));
    }
    if (msg) {
        return rb_ary_new3(2, rb_str_new2(msg), ary);
    } else {
        return rb_ary_new3(2, Qnil, ary);
    }
}
#endif /* HAVE_GDIMAGESTRINGFT */

static VALUE
img_char(img, fnt, x, y, ch, c)
    VALUE img, fnt, x, y, ch, c;
{
    gdImagePtr im;
    gdFontPtr  f;
    int ci;

    Data_Get_Struct(img, gdImage, im);
    font_req(fnt);
    Data_Get_Struct(fnt, gdFont, f);
    
    if (TYPE(ch) == T_STRING) {
	if (RSTRING_LEN(ch) != 1) {
	    rb_raise(rb_eArgError, "string must be 1 byte(%ld bytes)", RSTRING_LEN(ch));
	}
	ci = RSTRING_PTR(ch)[0];
    }
    else {
	ci = NUM2INT(ch);
    }
    gdImageChar(im,f,NUM2INT(x),NUM2INT(y),ci,NUM2INT(c));

    return img;
}

static VALUE
img_char_up(img, fnt, x, y, ch, c)
    VALUE img, fnt, x, y, ch, c;
{
    gdImagePtr im;
    gdFontPtr  f;
    int ci;

    Data_Get_Struct(img, gdImage, im);
    font_req(fnt);
    Data_Get_Struct(fnt, gdFont, f);

    if (TYPE(ch) == T_STRING) {
	if (RSTRING_LEN(ch) != 1) {
	    rb_raise(rb_eArgError, "string must be 1 byte(%ld bytes)", RSTRING_LEN(ch));
	}
	ci = RSTRING_PTR(ch)[0];
    }
    else {
	ci = NUM2INT(ch);
    }
    gdImageCharUp(im,f,NUM2INT(x),NUM2INT(y),ci,NUM2INT(c));

    return img;
}

static VALUE
img_get_interlace(img)
    VALUE img;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    if (gdImageGetInterlaced(im)) {
	return Qtrue;
    }
    return Qfalse;
}

static VALUE
img_set_interlace(img, val)
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageInterlace(im, RTEST(val));

    return img;
}

static VALUE
img_bounds(img)
    VALUE img;
{
    gdImagePtr im;
    VALUE ary = rb_ary_new2(2);
    int i;

    Data_Get_Struct(img, gdImage, im);
    i = gdImageSX(im);
    rb_ary_push(ary, INT2FIX(i));
    i = gdImageSY(im);
    rb_ary_push(ary, INT2FIX(i));

    return ary;
}

 
static VALUE
img_bounds_safe(img, x, y)
    VALUE img, x, y;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    if ( gdImageBoundsSafe(im, NUM2INT(x), NUM2INT(y)) ) {
       return Qtrue;
    } else {
       return Qfalse;
    }
}

static VALUE
img_get_transparent(img)
    VALUE img;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    return INT2NUM(gdImageGetTransparent(im));
}


static VALUE
img_width(img)
    VALUE img;
{
    gdImagePtr im;
    int i;

    Data_Get_Struct(img, gdImage, im);
    i = gdImageSX(im);
    return INT2FIX(i);
}

static VALUE
img_height(img)
    VALUE img;
{
    gdImagePtr im;
    int i;

    Data_Get_Struct(img, gdImage, im);
    i = gdImageSY(im);
    return INT2FIX(i);
}

static VALUE
img_png(img, out)
    VALUE img, out;
{
    gdImagePtr im;
    FPTR_TYPE *fptr;
    FILE *f;

    Data_Get_Struct(img, gdImage, im);
    Check_Type(out, T_FILE); 
    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);

    f = SECOND_FILE_POINTER;
    gdImagePng(im, f);

    return img;
}

static VALUE
img_png_str(img)
    VALUE img;
{
    int size;
    void *ptr;
    gdImagePtr im;
    VALUE imageString;
    
    Data_Get_Struct(img, gdImage, im);
    ptr = gdImagePngPtr(im, &size);
    imageString = rb_str_new(ptr, size);
    
#ifdef ENABLE_GD_2_0
    gdFree(ptr);
#else
    free(ptr);
#endif

    return imageString;
}


#ifdef ENABLE_GD_2_0
static VALUE
img_gif(img, out)
    VALUE img, out;
{
    gdImagePtr im;
    FPTR_TYPE *fptr;
    FILE *f;

    Data_Get_Struct(img, gdImage, im);
    Check_Type(out, T_FILE); 
    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);
    f = rb_io_stdio_file(fptr);

    gdImageGif(im, f);

    return img;
}

static VALUE
img_gif_str(img)
    VALUE img;
{
    int size;
    void *ptr;
    gdImagePtr im;
    VALUE imageString;
    
    Data_Get_Struct(img, gdImage, im);
    ptr = gdImageGifPtr(im, &size);
    imageString = rb_str_new(ptr, size);
    gdFree(ptr);

    return imageString;
}
#endif /* ENABLE_GD_2_0 */


static VALUE
img_gd(img, out)
    VALUE img, out;
{
    gdImagePtr im;
    FPTR_TYPE *fptr;
    FILE *f;

    Data_Get_Struct(img, gdImage, im);
    Check_Type(out, T_FILE); 
    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);
    f = SECOND_FILE_POINTER;

    gdImageGd(im, f);

    return img;
}

static VALUE
img_gd2(img, out, cs, fmt)
    VALUE img, out, cs, fmt;
{
    FPTR_TYPE *fptr;
    gdImagePtr im;
    FILE *f;

    Check_Type(out, T_FILE);
    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);
    f = SECOND_FILE_POINTER;

    Data_Get_Struct(img, gdImage, im);
    gdImageGd2(im, f, NUM2INT(cs), NUM2INT(fmt));

    return img;
}


#ifdef HAVE_GDIMAGECREATEFROMJPEG
static VALUE
img_jpeg(img, out, quality)
    VALUE img, out, quality;
{
    gdImagePtr im;
    FPTR_TYPE *fptr;
    FILE *f;

    Data_Get_Struct(img, gdImage, im);
    
    Check_Type(out, T_FILE);

    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);
    f = SECOND_FILE_POINTER;

    gdImageJpeg(im, f, FIX2INT(quality));

    return img;
}

static VALUE
img_jpeg_str(img, quality)
    VALUE img, quality;
{
    int size;
    void *ptr;
    gdImagePtr im;
    VALUE imageString;
    
    Data_Get_Struct(img, gdImage, im);
    ptr = gdImageJpegPtr(im, &size, FIX2INT(quality));
    imageString = rb_str_new(ptr, size);
    
#ifdef ENABLE_GD_2_0
    gdFree(ptr);
#else
    free(ptr);
#endif

    return imageString;
}
#endif

static VALUE
img_wbmp(img, fg, out)
    VALUE img, out, fg;
{
    gdImagePtr im;
    FPTR_TYPE *fptr;
    FILE *f;

    Data_Get_Struct(img, gdImage, im);

    Check_Type(out, T_FILE);

    rb_io_binmode(out);
    GetOpenFile(out, fptr);
    rb_io_check_writable(fptr);
    f = SECOND_FILE_POINTER;

    gdImageWBMP(im, FIX2INT(fg), f);

    return img;
}

/*
 *
 *  Poligon
 *
 */

static VALUE
ply_new(klass)
    VALUE klass;
{
    VALUE self = rb_ary_new();

    RBASIC(self)->klass = klass;
    return self;
}

static VALUE
ply_add_pt(ply, x, y)
    VALUE ply, x, y;
{
    /* type check */
    NUM2INT(x);
    NUM2INT(y);

    rb_ary_push(ply, x);
    rb_ary_push(ply, y);
    return ply;
}

static VALUE
ply_to_pt(ply, dx, dy)
    VALUE ply, dx, dy;
{
    VALUE x, y;

    /* type check */
    NUM2INT(dx);
    NUM2INT(dy);

    if (RARRAY_LEN(ply) > 0) {
        x = rb_ary_entry(ply, RARRAY_LEN(ply) - 2);
        y = rb_ary_entry(ply, RARRAY_LEN(ply) - 1);
        rb_ary_push(ply, INT2NUM(NUM2INT(x) + NUM2INT(dx)));
        rb_ary_push(ply, INT2NUM(NUM2INT(y) + NUM2INT(dy)));
    } else {
        ply_add_pt(ply, dx, dy);
    }
    return ply;
}

static VALUE
ply_get_pt(ply, idx)
    VALUE ply, idx;
{
    int i = NUM2INT(idx);

    if (RARRAY_LEN(ply) < idx) return Qnil;
    i *= 2;

    return rb_assoc_new(rb_ary_entry(ply, i), rb_ary_entry(ply, i+1));
}

static VALUE
ply_set_pt(ply, idx, x, y)
    VALUE ply, idx, x, y;
{
    int i = NUM2INT(idx)*2;

    /* type check */
    NUM2INT(x);
    NUM2INT(y);

    rb_ary_store(ply, i,   x);
    rb_ary_store(ply, i+1, y);

    return ply;
}

static VALUE
ply_delete_pt(ply, idx)
    VALUE ply, idx;
{
    int i = NUM2INT(idx)*2;

    rb_ary_delete_at(ply, INT2FIX(i));
    rb_ary_delete_at(ply, INT2FIX(i+1));

    return ply;
}

static VALUE
ply_length(ply)
    VALUE ply;
{
    return INT2FIX(RARRAY_LEN(ply) / 2);
}

static VALUE
ply_vertices(ply)
    struct RArray *ply;
{
    int i;
    VALUE ary = rb_ary_new2(RARRAY_LEN(ply)/2);

    for (i = 0; i<RARRAY_LEN(ply); i+=2) {
	rb_ary_push(ary, rb_assoc_new(RARRAY_PTR(ply)[i], RARRAY_PTR(ply)[i+1]));
    }
    return ary;
}

static VALUE
ply_bounds(ply)
    struct RArray *ply;
{
    int i, l, t, r, b;
    int nx, ny;

    if (RARRAY_LEN(ply) == 0) {
	l = t = r = b = 0;
    }
    else {
	l = r = NUM2INT(RARRAY_PTR(ply)[0]);
	t = b = NUM2INT(RARRAY_PTR(ply)[1]);
    }
    for (i = 2; i<RARRAY_LEN(ply); i+=2) {
	nx = NUM2INT(RARRAY_PTR(ply)[i]);
	if (nx < l) l = nx;
	if (nx > r) r = nx;
	ny = NUM2INT(RARRAY_PTR(ply)[i+1]);
	if (ny < t) t = ny;
	if (ny > b) b = ny;
    }
    return rb_ary_new3(4, INT2FIX(l), INT2FIX(t), INT2FIX(r), INT2FIX(b));
}

static VALUE
ply_offset(ply, vx, vy)
    struct RArray *ply;
    VALUE vx, vy;
{
    int i, x, y, c;

    x = NUM2INT(vx);
    y = NUM2INT(vy);

    for (i = 0; i<RARRAY_LEN(ply); i+=2) {
	c = NUM2INT(RARRAY_PTR(ply)[i]) + x;
	RARRAY_PTR(ply)[i] = INT2FIX(c);
	c = NUM2INT(RARRAY_PTR(ply)[i+1]) + y;
	RARRAY_PTR(ply)[i+1] = INT2FIX(c);
    }

    return (VALUE)ply;
}

static VALUE
ply_map(argc, argv, ply)
    int argc;
    VALUE *argv;
    struct RArray *ply;
{
    VALUE sl, st, sr, sb, dl, dt, dr, db;
    int sx, sy, dx, dy;
    double xmag, ymag;
    int i, c;

    i = rb_scan_args(argc,argv,"44",&sl,&st,&sr,&sb, &dl,&dt,&dr,&db);

    if (i == 4) {
	int i, l, t, r, b;
	int nx, ny;

	if (RARRAY_LEN(ply) == 0) {
	    l = t = r = b = 0;
	}
	else {
	    l = r = NUM2INT(RARRAY_PTR(ply)[0]);
	    t = b = NUM2INT(RARRAY_PTR(ply)[1]);
	}
	for (i = 2; i<RARRAY_LEN(ply); i+=2) {
	    nx = NUM2INT(RARRAY_PTR(ply)[i]);
	    if (nx < l) l = nx;
	    if (nx > r) r = nx;
	    ny = NUM2INT(RARRAY_PTR(ply)[i+1]);
	    if (ny < t) t = ny;
	    if (ny > b) b = ny;
	}
	sx = l;
	sy = t;
	dx = NUM2INT(sl);
	dy = NUM2INT(st);
	xmag = (double)(NUM2INT(sr) - NUM2INT(sl))/(double)(r - l);
	ymag = (double)(NUM2INT(sb) - NUM2INT(st))/(double)(b - t);
    }
    else if (i == 8) {
	sx = NUM2INT(sl);
	sy = NUM2INT(st);
	dx = NUM2INT(dl);
	dy = NUM2INT(dt);
	xmag = (double)(NUM2INT(dr) - NUM2INT(dl))/
	    (double)(NUM2INT(sr) - NUM2INT(sl));
	ymag = (double)(NUM2INT(db) - NUM2INT(dt))/
	    (double)(NUM2INT(sb) - NUM2INT(st));
    }
    else {
	rb_raise(rb_eArgError, "wrong # of arguments (%d for 4 or 8)", argc);
    }

    for (i = 0; i<RARRAY_LEN(ply); i+=2) {
	c = NUM2INT(RARRAY_PTR(ply)[i]);
	c = (c-sx)*xmag+dx;
	RARRAY_PTR(ply)[i] = INT2FIX(c);

	c = NUM2INT(RARRAY_PTR(ply)[i+1]);
	c = (c-sy)*ymag+dy;
	RARRAY_PTR(ply)[i+1] = INT2FIX(c);
    }

    return (VALUE)ply;
}

static VALUE
ply_transform(ply, a, b, c, d, tx, ty)
    VALUE ply, a, b, c, d, tx, ty;
{
    int i;
    VALUE x, y;

    for (i = 0; i < RARRAY_LEN(ply) / 2; i++) {
/*      x = rb_ary_entry(ply, i * 2);
        y = rb_ary_entry(ply, i * 2 + 1);*/
        x = RARRAY_PTR(ply)[i * 2];
        y = RARRAY_PTR(ply)[i * 2 + 1];
        ply_set_pt(ply, INT2NUM(i),
                INT2NUM(NUM2DBL(a) * NUM2INT(x) + NUM2DBL(c) * NUM2INT(y) + NUM2INT(tx)),
                INT2NUM(NUM2DBL(b) * NUM2INT(x) + NUM2DBL(d) * NUM2INT(y) + NUM2INT(ty)));
    }
    return ply;
}

static VALUE
ply_scale(ply, sx, sy)
    VALUE ply, sx, sy;
{
    return ply_transform(ply, sx, INT2NUM(0), INT2NUM(0), sy, INT2NUM(0), INT2NUM(0));
}

static VALUE
fnt_create(fnt)
    gdFontPtr fnt;
{
    return Data_Wrap_Struct(cFont, 0, 0, fnt);
}

static VALUE
fnt_new(name)
    char *name;
{
    if (strcmp(name, "Giant") == 0) {
	return fnt_create(gdFontGiant);
    }
    if (strcmp(name, "Large") == 0) {
	return fnt_create(gdFontLarge);
    }
    if (strcmp(name, "Medium") == 0) {
	return fnt_create(gdFontMediumBold);
    }
    if (strcmp(name, "Small") == 0) {
	return fnt_create(gdFontSmall);
    }
    if (strcmp(name, "Tiny") == 0) {
	return fnt_create(gdFontTiny);
    }
    rb_raise(rb_eArgError, "undefined font name `%s'", name);
}

static VALUE
fnt_s_new(obj, name)
    VALUE obj;
    struct RString *name;
{
    Check_Type(name, T_STRING);
    return fnt_new(RSTRING_PTR(name));
}

static VALUE
fnt_nchars(fnt)
    VALUE fnt;
{
    gdFontPtr fp;

    Data_Get_Struct(fnt,gdFont,fp);
    return INT2FIX(fp->nchars);
}

static VALUE
fnt_offset(fnt)
    VALUE fnt;
{
    gdFontPtr fp;

    Data_Get_Struct(fnt,gdFont,fp);
    return INT2FIX(fp->offset);
}

static VALUE
fnt_width(fnt)
    VALUE fnt;
{
    gdFontPtr fp;

    Data_Get_Struct(fnt,gdFont,fp);
    return INT2FIX(fp->w);
}

static VALUE
fnt_height(fnt)
    VALUE fnt;
{
    gdFontPtr fp;

    Data_Get_Struct(fnt,gdFont,fp);
    return INT2FIX(fp->h);
}

/*
 *
 *  gd-2.0.x features : experimental
 *
 */

#ifdef ENABLE_GD_2_0

static VALUE
img_s_new_tc(klass, sx, sy)
    VALUE klass, sx, sy;
{
    gdImagePtr iptr;

    if (NUM2INT(sx)<0 || NUM2INT(sy)<0)
    	rb_raise(rb_eArgError, "Negative width/height not allowed");
    
    iptr = gdImageCreateTrueColor(NUM2INT(sx), NUM2INT(sy));
    if (!iptr)
        rb_raise(rb_eRuntimeError, "Unable to allocate the new image");
    
    return Data_Wrap_Struct(klass,0,free_img,iptr);
}

static VALUE
img_color_allocate_alpha_tri(img, r, g, b, a)
    VALUE img, r, g, b, a;
{
    gdImagePtr im;
    int c;
    
    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorAllocateAlpha(im, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    
    return INT2NUM(c);
}

static VALUE
img_color_allocate_alpha_str(img, rgbstr, a)
    VALUE img, rgbstr, a;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);
    
    ary = hex2triplet(rgbstr);
    c = gdImageColorAllocateAlpha(im,
                                  NUM2INT(*(RARRAY_PTR(ary))),
                                  NUM2INT(*(RARRAY_PTR(ary)+1)),
                                  NUM2INT(*(RARRAY_PTR(ary)+2)),
                                  NUM2INT(a));
    return INT2NUM(c);
}

static VALUE
img_color_allocate_alpha(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, a, retval;

    if (!(argc == 2 || argc == 4))
        rb_raise(rb_eArgError, "Wrong # of arguments (2 or 4 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "20", &rgbstr, &a);
            retval = img_color_allocate_alpha_str(img, rgbstr, a);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "40", &r, &g, &b, &a);
            retval = img_color_allocate_alpha_tri(img, r, g, b, a);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}


static VALUE
img_color_resolve_alpha_tri(img, r, g, b, a)
    VALUE img, r, g, b, a;
{
    gdImagePtr im;
    int c;
    
    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorResolveAlpha(im, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    
    return INT2NUM(c);
}

static VALUE
img_color_resolve_alpha_str(img, rgbstr, a)
    VALUE img, rgbstr, a;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);
    
    ary = hex2triplet(rgbstr);
    c = gdImageColorResolveAlpha(im,
                                 NUM2INT(*(RARRAY_PTR(ary))),
                                 NUM2INT(*(RARRAY_PTR(ary)+1)),
                                 NUM2INT(*(RARRAY_PTR(ary)+2)),
                                 NUM2INT(a));
    return INT2NUM(c);
}

static VALUE
img_color_resolve_alpha(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, a, retval;

    if (!(argc == 2 || argc == 4))
        rb_raise(rb_eArgError, "Wrong # of arguments (2 or 4 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "20", &rgbstr, &a);
            retval = img_color_resolve_alpha_str(img, rgbstr, a);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "40", &r, &g, &b, &a);
            retval = img_color_resolve_alpha_tri(img, r, g, b, a);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

static VALUE
img_color_closest_alpha_tri(img, r, g, b, a)
    VALUE img, r, g, b, a;
{
    gdImagePtr im;
    int c;
    
    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorClosestAlpha(im, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    
    return INT2NUM(c);
}

static VALUE
img_color_closest_alpha_str(img, rgbstr, a)
    VALUE img, rgbstr, a;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);
    
    ary = hex2triplet(rgbstr);
    c = gdImageColorClosestAlpha(im,
                                 NUM2INT(*(RARRAY_PTR(ary))),
                                 NUM2INT(*(RARRAY_PTR(ary)+1)),
                                 NUM2INT(*(RARRAY_PTR(ary)+2)),
                                 NUM2INT(a));
    return INT2NUM(c);
}

static VALUE
img_color_closest_alpha(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, a, retval;

    if (!(argc == 2 || argc == 4))
        rb_raise(rb_eArgError, "Wrong # of arguments (2 or 4 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "20", &rgbstr, &a);
            retval = img_color_closest_alpha_str(img, rgbstr, a);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "40", &r, &g, &b, &a);
            retval = img_color_closest_alpha_tri(img, r, g, b, a);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}


static VALUE
img_color_exact_alpha_tri(img, r, g, b, a)
    VALUE img, r, g, b, a;
{
    gdImagePtr im;
    int c;
    
    Data_Get_Struct(img, gdImage, im);
    c = gdImageColorExactAlpha(im, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    
    return INT2NUM(c);
}

static VALUE
img_color_exact_alpha_str(img, rgbstr, a)
    VALUE img, rgbstr, a;
{
    gdImagePtr im;
    int c;
    VALUE ary;

    Data_Get_Struct(img, gdImage, im);
    
    ary = hex2triplet(rgbstr);
    c = gdImageColorExactAlpha(im,
                                 NUM2INT(*(RARRAY_PTR(ary))),
                                 NUM2INT(*(RARRAY_PTR(ary)+1)),
                                 NUM2INT(*(RARRAY_PTR(ary)+2)),
                                 NUM2INT(a));
    return INT2NUM(c);
}

static VALUE
img_color_exact_alpha(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, a, retval;

    if (!(argc == 2 || argc == 4))
        rb_raise(rb_eArgError, "Wrong # of arguments (2 or 4 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "20", &rgbstr, &a);
            retval = img_color_exact_alpha_str(img, rgbstr, a);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "40", &r, &g, &b, &a);
            retval = img_color_exact_alpha_tri(img, r, g, b, a);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}

static VALUE
img_alpha_blending(img, blending_mode)
    VALUE img, blending_mode;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    gdImageAlphaBlending(im, RTEST(blending_mode));
    
    return img;
}

static VALUE
img_alpha(img, color)
    VALUE img, color;
{
    gdImagePtr im;

    Data_Get_Struct(img, gdImage, im);
    return INT2NUM(gdImageAlpha(im, NUM2INT(color)));
}


static VALUE
img_s_truecolor_str(rgbstr)
    VALUE rgbstr;
{
    int c;
    VALUE ary;
    ary = hex2triplet(rgbstr);
    c = gdTrueColor(NUM2INT(*(RARRAY_PTR(ary))),
                    NUM2INT(*(RARRAY_PTR(ary)+1)),
                    NUM2INT(*(RARRAY_PTR(ary)+2)));
    
    return INT2NUM(c);
}

static VALUE
img_s_truecolor_tri(r, g, b)
    VALUE r, g, b;
{
    int c;
    c = gdTrueColor(NUM2INT(r), NUM2INT(g), NUM2INT(b));
    
    return INT2NUM(c);
}

static VALUE
img_s_truecolor(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, retval;

    if (!(argc == 1 || argc == 3))
        rb_raise(rb_eArgError, "Wrong # of arguments (1 or 3 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "10", &rgbstr);
            retval = img_s_truecolor_str(rgbstr);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "30", &r, &g, &b);
            retval = img_s_truecolor_tri(r, g, b);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    
    return retval;
}


static VALUE
img_s_truecolor_alpha_str(rgbstr, a)
    VALUE rgbstr, a;
{
    int c;
    VALUE ary;
    ary = hex2triplet(rgbstr);
    c = gdTrueColorAlpha(NUM2INT(*(RARRAY_PTR(ary))),
                         NUM2INT(*(RARRAY_PTR(ary)+1)),
                         NUM2INT(*(RARRAY_PTR(ary)+2)),
                         NUM2INT(a));
    return INT2NUM(c);
}

static VALUE
img_s_truecolor_alpha_tri(r, g, b, a)
   VALUE r, g, b, a;
{
    int c;
    c = gdTrueColorAlpha(NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    
    return INT2NUM(c);
}

static VALUE
img_s_truecolor_alpha(argc, argv, img)
    int argc;
    VALUE *argv;
    VALUE img;
{
    int i;
    VALUE rgbstr, r, g, b, a, retval;

    if (!(argc == 2 || argc == 4))
        rb_raise(rb_eArgError, "Wrong # of arguments (2 or 4 for %d)", argc);
    
    switch(TYPE(argv[0])) {
        case T_STRING:
            i = rb_scan_args(argc, argv, "20", &rgbstr, &a);
            retval = img_s_truecolor_alpha_str(rgbstr, a);
            break;
        case T_FIXNUM:
            i = rb_scan_args(argc, argv, "40", &r, &g, &b, &a);
            retval = img_s_truecolor_alpha_tri(r, g, b, a);
            break;
        default:
            rb_raise(rb_eTypeError, "String or Fixnum expected");
            break;
    }
    return retval;
}

static VALUE
img_copy_resampled(img, img2, dx, dy, sx, sy, dw, dh, sw, sh)
    VALUE img, img2, dx, dy, sx, sy, dw, dh, sw, sh;
{
    gdImagePtr im, im2;

    Data_Get_Struct(img, gdImage, im);
    image_req(img2);
    Data_Get_Struct(img2, gdImage, im2);

    if (is_truecolor(im) && (!is_truecolor(im2))){
        rb_raise(rb_eRuntimeError,
                 "Copying truecolor image to palette image is not permitted");
    }
    
    gdImageCopyResampled(im2, im,
                         NUM2INT(dx), NUM2INT(dy),
                         NUM2INT(sx), NUM2INT(sy),
                         NUM2INT(dw), NUM2INT(dh),
                         NUM2INT(sw), NUM2INT(sh));
    return img;
}

static VALUE
img_filled_ellipse(img, cx, cy, w, h, start, end, color)
    VALUE img, cx, cy, w, h, start, end, color;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);
    gdImageFilledEllipse(im, NUM2INT(cx), NUM2INT(cy), NUM2INT(w), NUM2INT(h), NUM2INT(color));
    return img;
}

static VALUE
img_filled_arc(img, cx, cy, w, h, start, end, color, style)
    VALUE img, cx, cy, w, h, start, end, color, style;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);
    gdImageFilledArc(im, NUM2INT(cx), NUM2INT(cy), NUM2INT(w), NUM2INT(h),
                         NUM2INT(start), NUM2INT(end), NUM2INT(color), NUM2INT(style));
    return img;
}

static VALUE
img_is_truecolor_image(img)
    VALUE img;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);

    return is_truecolor(im);
}

static VALUE
img_is_palette_image(img)
    VALUE img;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);

    return is_truecolor(im) ? Qfalse : Qtrue;
}

static VALUE
img_to_palette_image(img, dither_flag, max_colors)
    VALUE img, dither_flag, max_colors;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);

    gdImageTrueColorToPalette(im, dither_flag, FIX2INT(max_colors));

    return img;
}

static VALUE
img_set_thickness(img, thickness)
    VALUE img, thickness;
{
    gdImagePtr im;
    Data_Get_Struct(img, gdImage, im);

    gdImageSetThickness(im, FIX2INT(thickness));
    
    return img;
}



#endif /* ENABLE_GD_2_0 */

void
Init_GD()
{
    mGD = rb_define_module("GD");
    cImage = rb_define_class_under(mGD, "Image", rb_cObject);
    rb_define_singleton_method(cImage, "new", img_s_new, 2);
    rb_define_singleton_method(cImage, "newPalette", img_s_new, 2);
    rb_define_singleton_method(cImage, "newFromPng", img_from_png, 1);
    rb_define_singleton_method(cImage, "new_from_png", img_from_pngfname, 1);
#ifdef ENABLE_GD_2_0
    rb_define_singleton_method(cImage, "newFromGif", img_from_gif, 1);
    rb_define_singleton_method(cImage, "new_from_gif", img_from_giffname, 1);
#endif /* ENABLE_GD_2_0 */
    rb_define_singleton_method(cImage, "newFromXbm", img_from_xbm, 1);
    rb_define_singleton_method(cImage, "new_from_xbm", img_from_xbmfname, 1);
    
    rb_define_singleton_method(cImage, "newFromGd",  img_from_gd, 1);
    rb_define_singleton_method(cImage, "new_from_gd",  img_from_gdfname, 1);

#ifdef HAVE_GDIMAGECREATEFROMXPM
    rb_define_singleton_method(cImage, "newFromXpm", img_from_xpm, 1);
    rb_define_singleton_method(cImage, "new_from_xpm", img_from_xpmfname, 1);    
#endif /* HAVE_GDIMAGECREATEFROMXPM */

    rb_define_singleton_method(cImage, "newFromGd2", img_from_gd2, 1);
    rb_define_singleton_method(cImage, "new_from_gd2", img_from_gd2fname, 1);
    
    rb_define_singleton_method(cImage, "newFromGd2Part", img_from_gd2_part, 5);
    rb_define_singleton_method(cImage, "new_from_gd2_Part", img_from_gd2_partfname, 5);    
    
#ifdef HAVE_GDIMAGECREATEFROMJPEG
    rb_define_singleton_method(cImage, "newFromJpeg", img_from_jpeg, 1);
    rb_define_singleton_method(cImage, "new_from_jpeg", img_from_jpegfname, 1);    
#endif /* HAVE_GDIMAGECREATEFROMJPEG */

    rb_define_method(cImage, "destroy", img_destroy, 0);

    rb_define_method(cImage, "colorAllocate", img_color_allocate, -1);
    rb_define_method(cImage, "colorDeallocate", img_color_deallocate, 1);
    rb_define_method(cImage, "colorClosest", img_color_closest, -1);
    rb_define_method(cImage, "colorClosestHWB", img_color_closestHWB, -1);    
    rb_define_method(cImage, "colorExact", img_color_exact, -1);

#ifdef HAVE_GDIMAGECOLORRESOLVE    
    rb_define_method(cImage, "colorResolve", img_color_resolve, -1);
#endif /* HAVE_GDIMAGECOLORRESOLVE */

    rb_define_method(cImage, "colorsTotal", img_colors_total, 0);
    rb_define_method(cImage, "getPixel", img_get_pixel, 2);
    rb_define_method(cImage, "red", img_red, 1);
    rb_define_method(cImage, "green", img_green, 1);
    rb_define_method(cImage, "blue", img_blue, 1);
    rb_define_method(cImage, "rgb", img_rgb, 1);
    rb_define_method(cImage, "transparent", img_transparent, 1);

    rb_define_method(cImage, "setBrush", img_set_blush, 1);
    rb_define_const(mGD, "Brushed", INT2FIX(gdBrushed));
    rb_define_method(cImage, "setStyle", img_set_style, -1);
    rb_define_const(mGD, "Styled", INT2FIX(gdStyled));
    rb_define_const(mGD, "StyledBrushed", INT2FIX(gdStyledBrushed));
    rb_define_method(cImage, "setTile", img_set_tile, 1);
    rb_define_const(mGD, "Tiled", INT2FIX(gdTiled));
    rb_define_const(mGD, "Transparent", INT2FIX(gdTransparent));

    rb_define_const(mGD, "GD2_FMT_COMPRESSED", INT2FIX(GD2_FMT_COMPRESSED));
    rb_define_const(mGD, "GD2_FMT_RAW", INT2FIX(GD2_FMT_RAW));
    
    rb_define_method(cImage, "setPixel", img_set_pixel, 3);
    rb_define_method(cImage, "line", img_line, 5);
    rb_define_method(cImage, "dashedLine", img_dashed_line, 5);
    rb_define_method(cImage, "rectangle", img_rectangle, 5);
    rb_define_method(cImage, "filledRectangle", img_filled_rectangle, 5);
    rb_define_method(cImage, "polygon", img_polygon, 2);
    rb_define_method(cImage, "filledPolygon", img_filled_polygon, 2);
    rb_define_method(cImage, "arc", img_arc, 7);
    rb_define_method(cImage, "fill", img_fill, 3);
    rb_define_method(cImage, "fillToBorder", img_fill_to_border, 4);
    rb_define_method(cImage, "line", img_line, 5);

    rb_define_method(cImage, "copy", img_copy, 7);
    rb_define_method(cImage, "copyResized", img_copy_resized, 9);
    rb_define_method(cImage, "copyMerge", img_copy_merge, 8);
    rb_define_method(cImage, "copyMergeGray", img_copy_merge_gray, 8);
    rb_define_method(cImage, "paletteCopy", img_palette_copy, 1);
     
    rb_define_method(cImage, "string", img_string, 5);
    rb_define_method(cImage, "stringUp", img_string_up, 5);

#ifdef HAVE_GDIMAGESTRINGTTF
    rb_define_singleton_method(cImage, "stringTTF", img_s_string_ttf, 7);
    rb_define_method(cImage, "stringTTF", img_string_ttf, 7);
#endif    

#ifdef HAVE_GDIMAGESTRINGFT
    rb_define_singleton_method(cImage, "stringFT", img_s_string_ft, 7);
    rb_define_method(cImage, "stringFT", img_string_ft, 7);
#endif    

    rb_define_method(cImage, "char", img_char, 5);
    rb_define_method(cImage, "charUp", img_char_up, 5);

    rb_define_method(cImage, "interlace",  img_get_interlace, 0);
    rb_define_method(cImage, "interlace=", img_set_interlace, 1);
    rb_define_method(cImage, "getTransparent", img_get_transparent, 0);
    
    rb_define_method(cImage, "bounds", img_bounds, 0);
    rb_define_method(cImage, "boundsSafe", img_bounds_safe, 2);
    rb_define_method(cImage, "width", img_width, 0);
    rb_define_method(cImage, "height", img_height, 0);

    rb_define_method(cImage, "png", img_png, 1);
    rb_define_method(cImage, "pngStr", img_png_str, 0);
#ifdef ENABLE_GD_2_0
    rb_define_method(cImage, "gif", img_gif, 1);
    rb_define_method(cImage, "gifStr", img_gif_str, 0);
#endif /* ENABLE_GD_2_0 */
    rb_define_method(cImage, "gd2", img_gd2, 3);
    rb_define_method(cImage, "gd", img_gd, 1);

#ifdef HAVE_GDIMAGECREATEFROMJPEG
    rb_define_method(cImage, "jpeg", img_jpeg, 2);
    rb_define_method(cImage, "jpegStr", img_jpeg_str, 1);
#endif /* HAVE_GDIMAGECREATEFROMJPEG */
    
    rb_define_method(cImage, "wbmp", img_wbmp, 2);
 
    cPolygon = rb_define_class_under(mGD, "Polygon", rb_cObject);
    rb_define_singleton_method(cPolygon, "new", ply_new, 0);

    rb_define_method(cPolygon, "addPt", ply_add_pt, 2);
    rb_define_method(cPolygon, "toPt", ply_to_pt, 2);
    rb_define_method(cPolygon, "getPt", ply_get_pt, 1);
    rb_define_method(cPolygon, "setPt", ply_set_pt, 3);
    rb_define_method(cPolygon, "deletePt", ply_delete_pt, 1);
    rb_define_method(cPolygon, "length", ply_length, 0);
    rb_define_method(cPolygon, "vertices", ply_vertices, 0);
    rb_define_method(cPolygon, "bounds", ply_bounds, 0);
    rb_define_method(cPolygon, "offset", ply_offset, 2);
    rb_define_method(cPolygon, "map", ply_map, -1);
    rb_define_method(cPolygon, "transform", ply_transform, 6);
    rb_define_method(cPolygon, "scale", ply_scale, 2);

    cFont = rb_define_class_under(mGD, "Font", rb_cObject);
    rb_define_singleton_method(cFont, "new", fnt_s_new, 1);

    rb_define_const(cFont, "GiantFont", fnt_new("Giant"));
    rb_define_const(cFont, "SmallFont", fnt_new("Small"));
    rb_define_const(cFont, "LargeFont", fnt_new("Large"));
    rb_define_const(cFont, "MediumFont", fnt_new("Medium"));
    rb_define_const(cFont, "MediumBoldFont", fnt_new("Medium"));
    rb_define_const(cFont, "TinyFont", fnt_new("Tiny"));

    rb_define_method(cFont, "nchars", fnt_nchars, 0);
    rb_define_method(cFont, "offset", fnt_offset, 0);
    rb_define_method(cFont, "width", fnt_width, 0);
    rb_define_method(cFont, "height", fnt_height, 0);

#ifdef ENABLE_GD_2_0    

    rb_define_singleton_method(cImage, "newTrueColor", img_s_new_tc, 2);
    rb_define_method(cImage, "colorAllocateAlpha", img_color_allocate_alpha, -1);
    rb_define_method(cImage, "colorResolveAlpha", img_color_resolve_alpha, -1);
    rb_define_method(cImage, "colorClosestAlpha", img_color_closest_alpha, -1);
    rb_define_method(cImage, "colorExactAlpha", img_color_exact_alpha, -1);
    rb_define_method(cImage, "alphaBlending=", img_alpha_blending, 1);
    rb_define_method(cImage, "alpha", img_alpha, 1);

    rb_define_singleton_method(cImage, "trueColor", img_s_truecolor, -1);
    rb_define_singleton_method(cImage, "trueColorAlpha", img_s_truecolor_alpha, -1);

    rb_define_method(cImage, "copyResampled", img_copy_resampled, 9);
    rb_define_method(cImage, "filledEllipse", img_filled_ellipse, 7);
    rb_define_method(cImage, "filledArc", img_filled_arc, 8);
    rb_define_method(cImage, "is_trueColor?", img_is_truecolor_image, 0);
    rb_define_method(cImage, "is_palette?", img_is_palette_image, 0);
    rb_define_method(cImage, "to_paletteImage", img_to_palette_image, 2);
    rb_define_method(cImage, "thickness=", img_set_thickness, 1);
    
    rb_define_const(mGD, "AlphaTransparent", INT2FIX(gdAlphaTransparent));
    rb_define_const(mGD, "AlphaOpaque", INT2FIX(gdAlphaOpaque));

    rb_define_const(mGD, "Arc", INT2FIX(gdArc));
    rb_define_const(mGD, "Chord", INT2FIX(gdChord));
    rb_define_const(mGD, "Pie", INT2FIX(gdPie));
    rb_define_const(mGD, "NoFill", INT2FIX(gdNoFill));
    rb_define_const(mGD, "Edged", INT2FIX(gdEdged));

#endif /* ENABLE_GD_2_0 */

}
