=begin

  usage: ruby extconf.rb [options ...]
  configure options:
    --with-xpm      : enables Xpm support
    --with-ttf      : enables TrueType font support
    --with-jpeg     : enables Jpeg support
    --with-freetype : enables FreeType 2.x support

    --with-gd-dir=dir
    --with-gd-include=dir
    --with-gd-lib=dir

    --enable-gd2_0 : enable new features provided by gd-2.0.x(beta).
=end
require 'mkmf'

def delete_link(libname)
  lib_ary = $libs.split
  libname = '-l'+libname
  lib_ary.delete libname
  $libs = lib_ary.join " "
end

dir_config("gd", "/usr/local/include", "/usr/local/lib")

have_header('ruby/io.h')

if have_type("rb_io_t", ["ruby.h", "rubyio.h"])
  have_struct_member("rb_io_t", "fd", ["ruby.h", "rubyio.h"])
end

if with_config("xpm")
  dir_config("X11", "/usr/X11R6/include", "/usr/X11R6/lib")
end

if with_config('jpeg')
  have_library('jpeg')
end

if with_config('ttf')
  have_library('ttf')
end

if with_config('xpm')
  have_library('X11')
  have_library('Xpm')
end

if with_config('freetype')
  have_library('freetype')
end

if have_library('z') and have_library('png') and 
    have_library('gd', 'gdImagePng') and have_library('m')

  if with_config('ttf') and not have_func('gdImageStringTTF')
    delete_link('ttf')
  end
  
  have_func('gdImageColorResolve')

  if with_config('xpm') and not have_func('gdImageCreateFromXpm')
    delete_link('Xpm')
    delete_link('X11')
  end

  if with_config('jpeg') and not have_func('gdImageCreateFromJpeg')
    delete_link('jpeg')
  end
  
  if with_config('freetype') and not have_func('gdImageStringFT')
    delete_link('freetype')
  end
  
  if enable_config("gd2_0", false)
    if have_func("gdImageAlphaBlending")
      $CFLAGS += ' -DENABLE_GD_2_0'
    end
  end

  create_makefile('GD')
end
