=begin

= Ruby/GD Version 0.7.4

Ruby extension library for using Thomas Boutell's gd
library(http://www.boutell.com/gd/).

Originally written by Yukihiro Matsumoto (matz@ruby-lang.org)

Current maintainer: Ryuichi Tamura (tam@kais.kyoto-u.ac.jp)

((* If gd-2.0.x is installed in your system, and Ruby/GD is compiled
with (({--enable-gd2_0})) option, you can use those constants or
methods described in the subsection "gd-2.0.0 or higher"*))

== Module/Class Hierarchy

  GD -+
      |
      +- GD::Image
      |
      +- GD::Polygon
      |
      +- GD::Font

= Module GD

Module that defines some constants refered by GD::Image or GD::Polygon
instances.

== Constants

--- GD::Brushed

If this constant is used as the color when invoking a line-drawing
method such as ((<GD::Image#line>)) or ((<GD::Image#rectangle>)), the
line to be drawn is the brush image that has been set with
((<GD::Image#setBrush>)).

--- GD::Styled

If this constant is used as the color when invoking a line-drawing
method such as ((<GD::Image#line>)) or ((<GD::Image#rectangle>)), the
colors of the pixels are drawn successively from the style that has
been set with ((<GD::Image#setStyle>)). If the color of a pixel is
equal to ((<GD::Transparent>)), that pixel is not altered.

--- GD::StyledBrushed

If this constant is used as the color when invoking a line-drawing
method such as ((<GD::Image#line>)) or ((<GD::Image#rectangle>)), the
brush set with ((<GD::Image#setBrush>)) is drawn wherever the color
specified in ((<GD::Image#setStyle>)) is neither zero nor
((<GD::Transparent>)).

--- GD::Tiled

If this constant is used as the color when invoking a filling method
such as ((<GD::Image#filledRectangle>)),
((<GD::Image#filledPolygon>)), ((<GD::Image#fill>)), and
((<GD::Image#fillToBorder>)). The area is filled with a tile image set
with ((<GD::Image#setTile>)).

--- GD::Transparent

Used in place of a normal color in a style to be set with
((<GD::Image#setStyle>)). This constant is not the transparent color
index of the image; for that functionality, see
((<GD::Image#transparent>)).

--- GD::GD2_FMT_COMPRESSED

specifies Gd2 image to be compressed. This constant is used in
((<GD::Image#gd2>)).

--- GD::GD2_FMT_RAW

specifies Gd2 image to be uncompressed. This constant is used in
((<GD::Image#gd2>)).


=== gd-2.0.0 or higher

--- GD::AlphaTransparent

--- GD::AlphaOpaque

These constants are used in ((<GD::Image.trueColorAlpha>)), and each
defines the upper(127) and lower(0) bounds of alpha channel value,
respectively. Full transparency corresponds to GD::AlphaTransparent.

--- GD::Arc

--- GD::Chord

--- GD::Pie

--- GD::NoFill

--- GD::Edged

These constants become options that specify how an arc is drawn with
using ((<GD::Image#filledArc>)). See also ((<GD::Image#filledArc>)).


= Class GD::Image

== Class Methods

+ Instance Creation

--- GD::Image.new(width, height)

creates a new palette image instance with specified ((|width|)) and
((|height|)).

--- GD::Image.newFromGd(file)

creates a new image instance from Gd file. ((|file|)) is a File
object.

--- GD::Image.new_from_gd(filename)

creates a new Gd image instance from ((|filename|)). ((|filename|))
is a String object which specifies the localtion of the image file.

--- GD::Image.newFromGd2(file)

creates a new image instance from Gd2 file. ((|file|)) is a File
object.

--- GD::Image.new_from_gd2(filename)

creates a new Gd2 image instance from ((|filename|)). ((|filename|))
is a String object which specifies the location of the image file.

--- GD::Image.newFromGd2Part(file, srcX, srcY, width, height)

creates a new image instance from part of Gd2 file. ((|file|)) is a
File object.

--- GD::Image.new_from_gd2_part(filename)

creates a new Gd2 image instance from ((|filename|)). ((|filename|))
is a String object which specifies the location of the image file.

--- GD::Image.newFromJpeg(file)
(gd-1.8 or later)

Creates a new image instance from JPEG file. ((|file|)) is a File
object.

--- GD::Image.new_from_jpeg(filename)

creates a new Jpeg image instance from ((|filename|)). ((|filename|))
is a String object which specifies the location of the image file.

--- GD::Image.newFromPng(file)

creates a new image instance from PNG file. ((|file|)) is a File
object.

--- GD::Image.new_from_png(filename)

creates a new PNG image instance from ((|filename|)). ((|filename|))
is a String object which specifies the location of the image file.

--- GD::Image.newFromGif(file)

creates a new image instance from GIF file. ((|file|)) is a File
object.

--- GD::Image.new_from_gif(filename)

creates a new GIF image instance from ((|filename|)). ((|filename|))
is a String object which specifies the location of the image file.

--- GD::Image.newFromXbm(file)

creates a new image instance from Xbm file. ((|file|)) is a File
object.

--- GD::Image.new_from_xbm(filename)

creates a new XBitmap image instance from
((|filename|)). ((|filename|)) is a String object which specifies the
location of the image file.


--- GD::Image.newFromXpm(file)           
(gd-1.7 or later)

creates a new image instance from Xpm file. ((|file|)) is a File
object.

--- GD::Image.new_from_xpm(filename)

creates a new XPixmaps image instance from
((|filename|)). ((|filename|)) is a String object which specifies the
location of the image file.

+ Query

--- GD::Image.stringTTF(fg_color, fnt_name, pt, angle, x, y, str)
(gd-1.6.1 or later) 

Tries to find the bounding rectangle of ((|str|)) with size (((|pt|)))
and the name of the TrueType font(((|fnt_name|))), and returns the
array consists of the error message string and the array of each
vertex of the rectangle(brect[8], see the figure in the section of
((<GD::Image#stringTTF>))). If successful, the error message is
((:nil:)). See also ((<GD::Image#stringTTF>)).

--- GD::Image.stringFT(fg_color, fnt_name, pt, angle, x, y, str)
(gd-1.8.4) 

This method provides the same functionarity as
((<GD::Image.stringTTF>)) does, but uses FreeType2 library when
redering the string. See also ((<GD::Image#stringFT>))

=== gd-2.0.0 or higher

--- GD::Image.newTrueColor(width, height)

Creates a new truecolor image instance with specified ((|width|)) and
((|height|)).

--- GD::Image.trueColor(r, g, b)
--- GD::Image.trueColor(rgbstr)

returns the RGBA color value for drawing on a truecolor image, or an
image created by ((<GD::Image.newTrueColor>)). ((|r|)), ((|g|)),
((|b|)) values are in the range between 0 and 255.  Or, you can
specify it by ((|rgbstr|)) which is in the range between "#000000" and
"#FFFFFF".

--- GD::Image.trueColor(r, g, b, alpha)
--- GD::Image.trueColorAlpha(rgbstr, alpha)

returns the RGBA color value for drawing on a truecolor image, or an
image created by ((<GD::Image.newTrueColor>)) with alpha channel
transparency. Because gd has 7-bit alpha channel, the ((|alpha|))
value takes between 0(opaque) and 127(transparent).

These class methods are used for ((*truecolor*)) images. If you want
to specify alpha value for ((*palette*)) images, use
((<GD::Image#colorAllocateAlpha>)), ((<GD::Image#colorExactAlpha>)),
((<GD::Image#colorClosestAlpha>)), or
((<GD::Image#colorResolveAlpha>)) instead.


== Methods

--- GD::Image#destroy

Free the memory associated with the image instance.

--- GD::Image#interlace=(val)

If ((|val|)) is ((:true:)), the image is interlaced. Otherwise and by
default, the image is not interlaced.

=== Color Handling

--- GD::Image#colorAllocate(r, g, b)
--- GD::Image#colorAllocate(str)

Finds the first available color index in the image instance, and sets
its RGB values to ((|r|)), ((|g|)), ((|b|)), respectively and returns
the index of the new color table entry. ((|r|)), ((|g|)), ((|b|)) take
a value between 0 and 255. Also, if you use Ruby-1.6.0 or later, you
can specify a color as strings like "#FF00FF".

Note that the first time you invoke this method after creating a new
image by ((<GD::Image.new>)), ((*the background color*)) of the image
is set.

You can allocate upto 256 colors to one image. If there is no space to
allocate the requested color, this returns -1.

If you use gd-1.6.2 or later, you can use
((<GD::Image#colorResolve>)), more robust way of color allocation.

--- GD::Image#colorClosest(r, g, b)
--- GD::Image#colorClosest(str)

Searches the colors which have been defined thus far in the image
instance and returns the index of the color with RGB values closest to
((|r|)), ((|g|)), ((|b|)), respectively. Also, if you use Ruby-1.6.0
or later, you can specify a color as strings like "#FF00FF".If no
color have been yet allocated to that image, this returns -1

This method is useful when a image instance which you want to allocate
a new color is created from the image scanned from a photograph in
which many colors will be used.

--- GD::Image#colorDeallocate(color)

This marks the ((|color|)) at the specified index as being ripe for
reallocation.  The next time ((<GD::Image#colorAllocate>)) is used,
this entry will be replaced.  You can call this method several times
to deallocate multiple colors.

--- GD::Image#colorExact(r, g, b)
--- GD::Image#colorExact(str)

Searches the colors which have been defined thus far in the image
instance and returns the index of the first color with RGB values
which exactly match (((|r|)), ((|g|)), ((|b|))). If no allocated color
matches the request precisely, this returns -1. Also, if you use
Ruby-1.6.0 or later, you can specify a color as strings like
"#FF00FF".
  
--- GD::Image#colorResolve(r, g, b)
--- GD::Image#colorResolve(str)
(gd-1.6.2 or later)

Searches the colors which have been defined thus far in the image
specified and returns the index of the first color with RGB values
which exactly match those of the request. Also, if you use Ruby-1.6.0
or later, you can specify a color as strings like "#FF00FF".

If no allocated color matches the request precisely, then this method
tries to allocate the exact color.If there is no space left in the
color table then this method returns the closest color (as in
((<GD::Image#colorClosest>))).

Unlike ((<GD::Image#colorAllocate>)), this method always returns an
index of a color.

--- GD::Image#transparent(idx)

Sets the transparent color index for the specified image to the
specified color index ((|idx|)). To indicate that there should be no
transparent color, set ((|idx|)) to -1. 

Note that JPEG images ((*do not support*)) transparency, so this
setting has no effect when writing JPEG images.

+ Drawing and Filling

--- GD::Image#arc(cx, cy, width, height, start, end, color)

Draw a partial ellipse centered at the given point (((|cx|)),((|cy|)))
with specified width ((|width|)) and height ((|height|)) in pixel with
specified ((|color|)).  The arc begins at the position in degrees
specified by ((|start|)) and ends at the position specified by
((|end|)).

--- GD::Image#dashedLine(x1, y1, x2, y2, color)

This method is provided solely for backwards compatibility of the
Boutell's gd library. When drawing a dashed line, new programs should
use ((<GD::Image#line>)) with ((<GD::Image#setStyle>)).

--- GD::Image#fill(x, y, color)

Flood a portion of the image with the specified ((|color|)), beginning
at the specified point (((|x|)),((|y|))) and flooding the surrounding
region of the same color as the starting point. See also
((<GD::Image#fillToBorder>)).

--- GD::Image#filledPolygon(points, color)

Draw a filled polygon with the verticies specified, using the
((|color|)) index specified. see also ((<GD::Image#polygon>)).

--- GD::Image#filledRectangle(x1, y1, x2, y2, color)

Draw a filled rectangle. See also ((<GD::Image#rectangle>)).

--- GD::Image#fillToBorder(x, y, border_color, color)

floods a portion of the image with the specified ((|color|)),
beginning at the specified point (((|x|)),((|y|))) and stopping at the
specified ((|border_color|)).

The ((|border_color|)) cannot be a special color such as
((<GD::Tiled>)). It must be a proper solid color. However the fill
((|color|)) can be a special color.

--- GD::Image#line(x1, y1, x2, y2, color)

Draw a line between two endpoints (((|x1|)),((|y1|))) and
(((|x2|)),((|y2|))).  ((|color|)) is the color allocated by
((<GD::Image#colorAllocate>)), or one of ((<GD::Styled>)),
((<GD::Brushed>)), ((<GD::StyledBrushed>)).

--- GD::Image#polygon(points, color)

Draw a polygon with the verticies (at least 3) specified, using the
((|color|)) index. ((|points|)) is ((:GD::Polygon:)) instance. After
you created the ((:GD::Polygon:)) instances and did some operation on
the vertices, the poligon is drawn with this method.

--- GD::Image#rectangle(x1, y1, x2, y2, color)

Draw a rectangle with two corners(upper left is (((|x1|)),((|y1|))),
lower right (((|x2|)), ((|y2|)))) with the specified ((|color|))
index.

--- GD::Image#setBrush(image)

Set a "brush"(an image used to draw wide, shaped strokes) to
((|image|)).  ((|image|)) can be any of ((:GD::Image:)) instance.By
setting the transparent color index of the brush image with
((<GD::Image#transparent>)), a brush of any shape can be created. All
line-drawing methods, such as ((<GD::Image#line>)) and
((<GD::Image#polygon>)), will use the current brush if the special
"color" ((<GD::Brushed>)) or ((<GD::StyledBrushed>)) is used when
invoking them.

As for any image instance, brush image must be destroyed by
((<GD::Image#destroy>)).

--- GD::Image#setPixel(x, y, color)

Set the color of a pixel at (((|x|)),((|y|))) to ((|color|)) index.

--- GD::Image#setStyle(color1, color2, ...)

Set the series of colors to be drawn repeatedly during the drawing by
a method such as ((<GD::Image#line>)). Each element of ((|color|)) is
either the color index allocated by ((<GD::Image#colorAllocate>)), or
((<GD::Transparent>)) if you want the color of the particular pixel
left unchanged.

--- GD::Image#setTile(image)

Set the image to be tiled. ((|image|)) can be any ((:GD::Image:)) instance.

+ Copy and Merge

--- GD::Image#copy(dest_img, dest_X, dest_Y, self_X, self_Y, width, height)

Copy a portion of the image at (((|self_X, self_Y|))) with specified
((|width|)) and ((|height|)) to (((|dest_X|)), ((|dest_Y|))) of
((|dest_img|)).

--- GD::Image#copyMerge(dest_img, dest_X, dest_Y, self_X, self_Y, width, height, percent)

Copy the two images by an amount specified in the last parameter
((|percent|)). Arguments except for this are identical to those of
((<GD::Image#copy>)). If ((|percent|)) is equal to 100, then this
method will function identically to ((<GD::Image#copy>)) i.e. the
source image replaces the pixels in the destination. If the
((|percent|)) = 0, no action is taken.

This feature is most useful to 'highlight' sections of an image by
merging a solid color with ((|percent|)) = 50.

--- GD::Image#copyMergeGray(dest_img, dest_X, dest_Y, self_X, self_Y, width, height, percent)

Identical to ((<GD::Image#copyMerge>)), except that when merging
images it preserves the hue of the source by converting the
destination pixels to grey scale before the copy operation.

--- GD::Image#copyResized(dest_img, dest_X, dest_Y, self_X, self_Y, self_width, self_height, dest_X, dest_Y)

Copy a portion of the image at (((|self_X, self_Y|))) with specified
((|self_width|)) and ((|self_height|)) to ((|dest_img|)) at
(((|dest_X|)), ((|dest_Y|))) with specified ((|dest_width|)) and
((|dest_height|)).

--- GD::Image#paletteCopy(dest)

Copies a palette to ((|dest|)) image, attempting to match the colors
in the target image to the colors in the palette of the self.

+ Font and text handling

--- GD::Image#char(font, x, y, char, color)

Draws a single character ((|char|)) at (((|x|)), ((|y|))) with
specified ((|color|)). ((|font|)) is specified by one of
((<GD::Font::TinyFont>)), ((<GD::Font::SmallFont>)),
((<GD::Font::MediumFont>)), ((<GD::Font::LargeFont>)),
((<GD::Font::GiantFont>)).

--- GD::Image#charUp(font, x, y, char, color)

Draws a single chracter ((|char|)) at (((|x|)), ((|y|))) with
specified ((|color|)). ((|char|)) is drawn in a vertical direction,
i.e. drawn with rotated in 90 degree. See also ((<GD::Image#char>)).

--- GD::Image#string(font, x, y, str, color)

Draws multiple characters ((|str|)) on the image with the specified
((|color|)). ((|font|)) is specified by one of
((<GD::Font::TinyFont>)), ((<GD::Font::SmallFont>)),
((<GD::Font::MediumFont>)), ((<GD::Font::LargeFont>)),
((<GD::Font::GiantFont>)), or ((:GD::Font:)) instance created by
((<GD::Font.new>)).

--- GD::Image#stringUp(font, x, y, str, color)

Draw multiple characters ((|str|)) on the image with the specified
((|color|)). ((|str|)) is drawn in a vertical direction, i.e. drawn
with rotated in 90 degree. See also ((<GD::Image#string>)).

--- GD::Image#stringTTF(fg_color, fnt_name, pt, angle, x, y, str)

(gd-1.6.1 or later) 

Draws a string ((|str|)) at (((|x|)), ((|y|))) on the image using
user-supplied TrueType fonts with specified ((|fg_color|)).  The
location of TrueType font ((|fnt_name|)) is specified by full path.
The string may be arbitrarily scaled at ((|pt|)) and rotated
(((|angle|)) in radians).

This method returns an array of 2 elements. the first element is the
error message string, the 2nd is ((*brect*)) of the bounding rectangle
with 8 elements. Each element of ((*brect*)) is illustrated as below:

 (brect[6],brect[7])   (brect[4],brect[5]) 
          +--------------------+
          |( S  t  r  i  n  g )|
          +--------------------+
 (brect[0],brect[1])   (brect[2],brect[3])

When the string is successfully drawn, error message is ((:nil:)). 

If you only want to know the brect of the bounding rectangle, you can
use ((<GD::Image.stringTTF>)).

--- GD::Image#stringFT(fg_color, fnt_name, pt, angle, x, y, str)
(gd-1.8.4) 

This method provides the same functionarity as
((<GD::Image#stringTTF>)) does, but uses FreeType2 library when
redering the string.

+ Query

--- GD::Image#bounds

Returns the width and height of the image as Array instance.

--- GD::Image#boundsSafe(x, y)

Returns ((:true:)) if specified point (((|x|)),((|y|))) is within the
bounds of the image. Otherwise return ((:false:)).

--- GD::Image#blue(idx)

Returns the blue component of the specified color index ((|idx|)).

--- GD::Image#colorsTotal

Returns the number of colors currently allocated in the image. 

--- GD::Image#getTransparent

Returns the current transparent color index of the image. Returns -1
if there is no transparent color.

--- GD::Image#getPixel(x, y)

Returns the color index of a pixel at (((|x|)),((|y|)))).

--- GD::Image#green(idx)

Returns the green component of the specified color index ((|idx|)).

--- GD::Image#height

Returns the height of the image.

--- GD::Image#interlace

Returns ((:true:)) if the image is interlaced, and returns ((:false:)) otherwise.

--- GD::Image#red(idx)

Returns the red component of the specified color index ((|idx|)).

--- GD::Image#rgb(idx)

Returns array of the RGB values for the specified index ((|idx|)) of
the image.

--- GD::Image#width

Returns the width of the image.

+ Output

--- GD::Image#gd(file)

Outputs the image to the specified ((|file|)) in Gd format. 

--- GD::Image#gd2(file, chunk_size, fmt)

Outputs the image to the specified ((|file|)) in Gd2 format with
specified ((|chunk_size|)) and ((|fmt|)). A gd2 Image are stored as a
series of compressed/uncompressed subimages. You can specify
((|chunk_size|)) which determines the size of the subimages. If 
((|chunk_size|)) is set zero, the default size is used. Whether the image
is compressed or uncompressed is determined by ((|fmt|)) being
((<GD::GD2_FMT_COMPRESSED>)) or
((<GD::GD2_FMT_RAW>)), respectively.

--- GD::Image#jpeg(file, quality)
(gd-1.8 or later)

Outputs the image to the specified ((|file|)) with ((|quality|)) in
Jpeg format. If ((|quality|)) is set to nagative, the default IJG JPEG
quality value (which should yield a good general quality size
trade-off for most situations) is used.  For practical purposes,
quality should be a value in the range 0-95.

--- GD::Image#jpegStr(quality)
(gd-1.8 or later)

Outputs the Jpeg image as String object with specified ((|quality|)).
This method will be especially useful when you want to transmit an
image ((*directly*)) to an user(i.e, without first writing it to a
file).

This method is provided by Colin Steele(colin@webg2.com). 

--- GD::Image#png(file)

Outputs the image to the specified ((|file|)) in PNG format. 

--- GD::Image#pngStr(file)

Outputs the image in PNG format as String object.  This method will be
especially useful when you want to transmit an image ((*directly*)) to
an user(i.e, without first writing it to a file).

--- GD::Image#gif(file)

Outputs the image to the specified ((|file|)) in GIF format. 

--- GD::Image#gifStr(file)

Outputs the image in GIF format as String object.  This method will be
especially useful when you want to transmit an image ((*directly*)) to
an user(i.e, without first writing it to a file).

--- GD::Image#wbmp(fg_color, file)
(gd-1.8 or later)

Outputs the specified image to the specified file in WBMP format.

WBMP file support is ((*black*)) and ((*white*)) only. The color index
specified by the ((|fg_color|)) argument is the "foreground," and only
pixels of this color will be set in the WBMP file. All other pixels
will be considered "background."


=== gd-2.0.0 or higher

+ Color handling

--- GD::Image#colorAllocateAlpha(r, g, b, alpha)
--- GD::Image#colorAllocateAlpha(rgbstr, alpha)

Finds the first available color index in the image specified, sets its
RGBA values to those requested and returns the index of the new color
table entry, or an RGBA value in the case of a truecolor image; in
either case you can then use the returned value as a parameter to
drawing functions. The ((|alpha|)) value takes between 0(opaque) and
127(transparent).

See also ((<GD::Image#colorAllocate>)).

--- GD::Image#colorExactAlpha(r, g, b, alpha)
--- GD::Image#colorExactAlpha(rgbstr, alpha)

Searches the colors which have been defined thus far in the image
specified and returns the index of the first color entry which exactly
match those of the request, or an RGBA value in the case of a
truecolor image; in either case you can then use the returned value as
a parameter to drawing functions.The ((|alpha|)) value takes between
0(opaque) and 127(transparent).

See also ((<GD::Image#colorExact>)).

--- GD::Image#colorClosestAlpha(r, g, b, alpha)
--- GD::Image#colorClosestAlpha(rgbstr, alpha)

Searches the colors which have been defined thus far in the palette
image specified and returns the index of the color with RGBA values
closest to those of the request. The ((|alpha|)) value takes between
0(opaque) and 127(transparent).

Closeness is determined by Euclidian distance, which is used to
determine the distance in four-dimensional color/alpha space between
colors.

When applied to a truecolor image, this method always succeeds in
returning the desired color.

See also ((<GD::Image#colorClosest>)).

--- GD::Image#colorResolveAlpha(r, g, b, alpha)
--- GD::Image#colorResolveAlpha(rgbstr, alpha)

Searches the colors which have been defined thus far in the palette
image specified and returns the index of the first color with RGBA
values which exactly match those of the request. The ((|alpha|)) value
takes between 0(opaque) and 127(transparent).

If no allocated color matches the request precisely, then it tries to
allocate the exact color. If there is no space left in the color table
then it returns the closest color (as in
((<GD::Image#closestAlpha>))). 

This method always returns an index of a color.  When applied to a
truecolor image, this function always succeeds in returning the
desired color.

See also ((<GD::Image#colorResolve>)).

--- GD::Image#alphaBlending=(bool)

Specifies the different modes for drawing on ((*truecolor*)) images.
When ((|bool|)) sets ((:true:))("blending mode"), how much of the
underlying color should be allowed to shine through is determined by
the alpha channel value of the color.

This blending mode is effective for the following drawing operations,
and in this mode, the existing color at the drawing point is blended
with drawing color with its alpha value.

When ((|bool|)) sets ((:false:))("non-blending mode"), the existing
color at the drawing point is replaced by the drawing color with its
own alpha value.

Note that this blending mode is not available when drawing on
((*palette*)) images.

--- GD::Image#alpha(color)

Returns the alpha channel component (between 0 and 127) of the
specified color index.


+ Image manipulation

--- GD::Image#copyResampled(dest_img, dest_X, dest_Y, self_X, self_Y, dest_width, dest_height, self_width, self_height)

Copies a rectangular portion of one image to another image, smoothly
interpolating pixel values so that, in particular, reducing the size
of an image still retains a great deal of clarity.

Pixel values are interpolated only if the destination image is a
truecolor image. Otherwise, for a palette image, the same
functionality as ((<GD::Image#copyResized>)) will be invoked.

See also ((<GD::Image#copyResized>)), for a version which does not
interpolate pixel values.

--- GD::Image#filledEllipse(cx, cy, width, height, start, end, color)

draws an ellipse centered at the given point (((|cx|)), ((|cy|))),
with the specified ((|width|)) and ((|height|)) in pixels.

The ellipse is filled in the ((|color|)) and is drawn by beginning
from ((|start|)) degrees and ending at ((|end|)) degrees. ((|end|))
must be greater than ((|start|)). Values greater than 360 are
interpreted modulo 360.

--- GD::Image#filledArc(cx, cy, width, height, start, end, color, style)

draws an partial ellipse(arc) centered at the given point (((|cx|)),
((|cy|))), with the specified ((|width|)) and ((|height|)) in pixels,
and filled in the ((|color|)).  The arc begins at ((|start|)) degrees
and ends at ((|end|)) degrees.

The last argument is bitwise (({OR})) of the following possibilities.

 * (({GD::Arc}))

draws the rounded edge between ((|start|)) and ((|end|)).

 * (({GD::Chord}))

draws the line connecting ((|start|)) and ((|end|)).

 * (({GD::Pie})) 

synonym for (({GD::Arc}))


 * (({GD::NoFill}))

indicates Pie and Chord should be outlined, not filled.

 * (({GD::Edged}))

used together with (({GD::NoFill})), indicates that the beginning and
ending angles should be connected to the center; this is a good way to
outline (rather than fill) a 'pie slice'.

--- GD::Image#to_paletteImage(dither_flag, colors)

transforms the truecolor image to palette image. If ((|dither_flag|))
is ((:true:)), the image will be dithered to approximate colors
better, at the expense of some obvious "speckling." ((|colors|)) may
be anything up to 256, but if the image includes the photographic
information, or is a JPEG image, 256 is strongly recommended.

+ Query methods

--- GD::Image#is_trueColor?

returns ((:true:)) if the image is a truecolor image.

--- GD::Image#is_palette?

returns ((:true:)) if the image is a palette image.

--- GD::Image#thickness=(val)

specifies the line thickness (defaults to 1) that affects methods
drawing lines or curves. 

= Class GD::Polygon

== Class Methods

--- GD::Polygon.new

Creates a new polygon instance. After some operations provided below,
the polygon is drawn on the image by ((<GD::Image#polygon>)) method.

== Methods

+ Operation

--- GD::Polygon#addPt(x, y)

Adds a new point(vertex) from which the polygon is formed.

--- GD::Polygon#deletePt(idx)

Deletes the the specified ((|idx|))-th elements of the vertices.

--- GD::Polygon#getPt(idx)

Returns the value of the specified ((|idx|))-th elements of the
vertices.

--- GD::Polygon#map(dest_L, dest_T, dest_R, dest_B)
--- GD::Polygon#map([src_L, src_T, src_R, src_B,] dest_L, dest_T, dest_R, dest_B)

Maps the polygon from a source rectangle to an equivalent position in
a destination rectangle, moving it and resizing it as necessary. Both
the source and destination rectangles are given in
(left,top,right,bottom) coordinates. See figure below.

This method takes 4 or 8 arguments. Note that if 4 arguments are
given, source rectangle are automatically computed as the bounding box
of the poligon, and maps it to the destination rectangle specified in
the arguments.


         <source rectangle>            <destination rectangle>
  (src_L, src_T)                  (dest_L, dest_T)                
          +--------------+                  +----------+        
          |              |                  |          |
          |              |        =>        |          |        
          |              |                  |          |        
          +--------------+                  |          |
                 (src_R, src_B)             +----------+ 
                                                (dest_R, dest_B)

--- GD::Polygon#offset(dx, dy)

Offsets all the vertices of the polygon by ((|dx|)) pixels
horizontally and ((|dy|)) pixels vertically.

--- GD::Polygon#setPt(idx, new_x, new_y)

Changes the value of the specified ((|idx|))-th elements of the
vertices to (((|new_x|)), ((|new_y|))).

--- GD::Polygon#toPt(dx, dy)

Draw from current vertex to a new vertex, using relative (((|dx|)),
((|dy|))) coordinates.  If this is the first point, act like
((<GD::Image#addPt>)).

--- GD::Polygon#scale(sx, sy)

Scales each vertex of the polygon by the X and Y factors indicated by
((|sx|)) and ((|sy|)).  For best results, move the center of the
polygon to position (0,0) before you scale, then move it back to its
previous position.

--- GD::Polygon#transform(sx,rx,sy,ry,tx,ty)

Runs each vertex of the polygon through a transformation matrix, where
((|sx|)) and ((|sy|)) are the X and Y scaling factors, ((|rx|)) and
((|ry|)) are the X and Y rotation factors, and ((|tx|)) and ((|ty|))
are X and Y offsets.

+ Query

--- GD::Polygon#bounds

Returns the smallest rectangle that completely encloses the polygon.
The return value is an array containing the [left,top,right,bottom] of
the rectangle.

--- GD::Polygon#length

Returns the number of the vertices.

--- GD::Polygon#vertices

Returns all of the coodinates of the vertices as Array instance.

= Class GD::Font

== Class Methods

--- GD::Font.new(name)

Creates a new font instance. The font is specified by its
((|name|)), which is one of "((*Tiny*))", "((*Small*))",
"((*Medium*))", "((*Large*))", "((*Giant*))". These strings correspond
to ((<GD::Font::TinyFont>)), ((<GD::Font::SmallFont>)),
((<GD::Font::MediumFont>)), ((<GD::Font::LargeFont>)),
((<GD::Font::GiantFont>)), respectively.

The instance becomes the arguments in the charcter/string drawing
method such as ((<GD::Image#char>)), ((<GD::Image#string>)).

== Methods

--- GD::Font#height

Returns the height of each character of the font.

--- GD::Font#nchars

Returns the number of characeters in the font.

--- GD::Font#offset

Returns the offset of the first character in the font.

--- GD::Font#width

Returns the width of each character of the font.

== Constants

These constants also becomes the arguments in the charcter/string
drawing method such as ((<GD::Image#char>)), ((<GD::Image#string>)).

--- GD::Font::GiantFont

This stands for 9x15 bold font:
  -Misc-Fixed-Bold-R-Normal-Sans-15-140-75-75-C-90-ISO8859-2

--- GD::Font::LargeFont

This stands for the public domain 8x16 font:
  -misc-fixed-medium-r-normal--16-140-75-75-c-80-iso8859-2

--- GD::Font::MediumFont

This stands for the a public domain 7x13 font:
  -misc-fixed-bold-r-normal-sans-13-94-100-100-c-70-iso8859-2

--- GD::Font::SmallFont

This stands for a well known public domain 6x12 font:
  -misc-fixed-medium-r-semicondensed-sans-12-116-75-75-c-60-iso8859-2

--- GD::Font::TinyFont

This stands for almost unreadable font, 5x8 pixels wide:
  -Misc-Fixed-Medium-R-Normal--8-80-75-75-C-50-ISO8859-2


= Reference

* index.html distributed with gd-1.8.4
* index.html distributed with gd-2.0.1
* gd.h distributed with gd-2.0.1
* documents embedded in GD.pm version 1.30

=end
