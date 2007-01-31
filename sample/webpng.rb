#! /usr/bin/env ruby
=begin

  webpng.rb - 

=end
require 'getopts'
require 'GD'

def usage
  $stderr.print <<EOS
Usage: webpng.rb [-i y|n ] [-l] [-t index|none ] [-d] pngname.png
  -i [y|n]   Turns on/off interlace
  -l         Prints the table of color indexes
  -t [index] Set the transparent color to the specified index (0-255 or "none")
  -d         Reports the dimensions and other characteristics of the image.
If you specify '-' as the input file, stdin/stdout will be used input/output.
  (e.g. cat demoin.png | ./webpng.rb -i y - > demoout.png)
EOS
  exit 1
end

usage if ARGV.size < 2

if ARGV[-1] == '-'
  useStdinStdout = true
end

# option parse
usage unless getopts("ld", "i:", "t:", "help")
usage if $OPT_help

if $OPT_i
  usage unless $OPT_i == "y" or $OPT_i == "n"
end

if $OPT_t
  usage unless $OPT_t == "none" or 
    ($OPT_t.to_i >= 0 and $OPT_t.to_i <= 255)
end

if useStdinStdout
  in_file = $stdin
else
  in_fname = ARGV.pop
  in_file = File.open(in_fname, "rb")
end
im = GD::Image.newFromPng(in_file)
in_file.close

# options for information displaying and exiting

if $OPT_l
  printf("Index\tRed\tGreen\tBlue\n")
  for j in 0 .. im.colorsTotal
    rgb_ary = im.rgb(j)
    printf("%d\t%d\t%d\t%d\n", j, rgb_ary[0], rgb_ary[1], rgb_ary[2])
  end

  im.destroy
  exit 0
end

if $OPT_d
  printf("Width: %d Height: %d Colors: %d\n",im.width, im.height, im.colorsTotal)
  t = im.getTransparent
  if t != -1
    printf("Transparent index: %d\n", t)
  else
    puts "Transparent index: none"; # -1 means the image is not transparent.
  end
  
  if im.interlace
    puts "Interlaced: yes";
  else
    puts "Interlaced: no";
  end

  im.destroy
  exit 0
end

# options for involving for output part
if $OPT_i
  if $OPT_i == 'y'
    im.interlace = true # Set interlace
  else
    im.interlace = false # Clear interlace
  end
end

if $OPT_t
  if $OPT_t == 'none'
    im.transparent(-1)  # -1 means not transparent
  else
    im.transparent($OPT_t.to_i) # OK, get an integer and set the index
  end
end

if useStdinStdout
  im.png $stdout
else
  o_fname = "webpng.tmp"+$$.to_s
  ofile = open(o_fname, "wb")
  im.png ofile
  ofile.close
  File.delete in_fname
  File.rename o_fname, in_fname
end

im.destroy
