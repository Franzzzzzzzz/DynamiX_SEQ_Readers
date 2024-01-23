% move_seq (nbimage): move into the seq file for a given amount of images. NB: motion relatiove to the current image.


function seq2=move_seq (seq, nbimage)

if (seq.curframe+nbimage <0 || seq.curframe+nbimage > seq.frames)
  disp('Error: motion ourtside the boundary of the file. Doing nothing') ; return ; 
end ; 

fseek(seq.file, nbimage*seq.sz(1)*seq.sz(2)*2, 'bof') ; % there shoud be a 2 here????????
seq.curframe=seq.curframe+nbimage ; 
seq2=seq ; 
end 

