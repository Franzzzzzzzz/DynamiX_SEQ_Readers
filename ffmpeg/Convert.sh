#!/bin/bash

if [ $# -eq 0 ]
then
  echo "Usage: Convert.sh filename.seq". 
  exit
fi
echo ${1/%seq/log}

w=`grep 'image_size' ${1/%seq/log} | egrep -o 'width": [0-9]+' | egrep -o '[0-9]+' `
h=`grep 'image_size' ${1/%seq/log} | egrep -o 'height": [0-9]+' | egrep -o '[0-9]+' `
fps=`grep "fps" ${1/%seq/log} | egrep -o '[0-9]+' `

ffmpeg -r 1 -f rawvideo -pix_fmt gray16le -video_size ${w}x${h} -r $fps -i $1 \
-filter_complex "lut1d=file='inferno_lut.csp'" \
-q 1 ${1/%seq/avi}









