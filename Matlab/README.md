## Installation
Add all the files to a location that Matlab can find, or add the folder to your path. The reading of the file is image by image, sequantially. 

## Documentation
- `seq=open_seq("filename.seq")`: open the file `filename.seq`. This function must always be run first. In the following, we assume that the return value of this function is put in a variable called `seq`.

- `I=read_seq_next(seq)`: read the next image in the file, and put it in the vraiable I. 

- `I=read_seq_all(seq)`: read all the remaining images in the file, and put them in the three-dimensional variable I. For long videos this may exceed the memory capacity of Matlab or the computer. 

- `close_seq(seq)`: close the file. If `open_seq` is subsequently used, the file is reopened at the beginning. 

- `move_seq(seq, i)`: move `i` images forward or backward in the file. 

## Example of a typical usage:
```matlab
seq=open_seq("Sample.seq") ; 
for i=1:seq.frames ; 
    I=read_seq_next(seq) ;
    imagesc(I) ; axis image ; 
    pause(0.1) ; 
end ;
close_seq(seq) ;
``` 



