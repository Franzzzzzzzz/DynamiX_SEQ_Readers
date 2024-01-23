# @File(label='Choose a file') fic
# @Integer(label='First image', value=0,persist=false) begimg
# @Integer(label='Last image', value=-1,persist=false) endimg

# We do only include the module os,
# as we can use os.path.walk()
# to access functions of the submodule.
import os

import ij.IJ as IJ
import ij.plugin.FileInfoVirtualStack as FileInfoVirtualStack
import ij.io.FileInfo as FileInfo
import re
import json


def openseq (fic, mode, isvarian) :
	if mode=="Mode 1" : w=1536 ; h=1920 ;
	else : w=768 ; h=960 ;

	(a,b)=os.path.split(str(fic)) ;
	sz=os.path.getsize(str(fic)) ;

	if not isvarian :
		(root, rien)=os.path.splitext(str(fic)) ;
		if False :
			try:
				with open(root+".log") as myfile:
					head = [next(myfile) for x in xrange(4)]
			except :
				print('A log file is expected for non-varian seq files.') ;
				return ;
			res=re.split('x', head[3]) ;
			w=int(res[0])
			h=int(res[1])
		else : #new log file version (end of 2019)
			try:
				with open(root+".log", 'r') as f:
   	 				infos = json.load(f)
			except:
				print('A log file is expected for non-varian seq files.') ;
				return ;
			w=int(infos["detector"]["image_size"]["width"]) ;
			h=int(infos["detector"]["image_size"]["height"]) ;

	fi=FileInfo()
	fi.fileFormat = fi.RAW;
	fi.fileName = b;
	fi.directory = a;
	fi.width = w;
	fi.height = h;
	if (isvarian) :
		off=2048+begimg*2*w*h ;
		nmax=((sz-2048)/(2*w*h)) ;
		if (endimg>-1):
			n=min(endimg-begimg,nmax-begimg)
		else:
			n=nmax-begimg
	else :
		off=0+begimg*2*w*h ;
		nmax=((sz-0)/(2*w*h)) ;
		if (endimg>-1):
			n=min(endimg-begimg,nmax-begimg)
		else:
			n=nmax-begimg
	fi.offset=off ;
	fi.nImages = n;
	fi.gapBetweenImages = 0;
	fi.intelByteOrder = True;
	fi.fileType = FileInfo.GRAY16_UNSIGNED;

	vs=FileInfoVirtualStack(fi);

	print("If you want to open the file as a raw, here are the parameters:")
	print("Image type: 16-bit Unsigned")
	print("Width: "+str(w))
	print("Height: "+str(h))
	print("Offset to 1st image: "+str(off))
	print("Number of images: "+str(n))
	print("Gap between images: 0") ;
	print("Little-endian byte order") ;

	IJ.run("mpl-inferno", "");
	IJ.run("Brightness/Contrast...");

images = openseq(fic, "0", False) ;
