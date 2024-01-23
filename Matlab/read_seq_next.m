% data=read_seq_next(seq)

function data=read_seq_next(seq)

if (seq.curframe>=seq.frames-1)
disp('Cannot read after the end of the file') ; data=[] ; return ; 
end ;

data=zeros(seq.sz(1), seq.sz(2), 1, 'uint16') ; 
data=fread(seq.file, seq.sz, 'uint16') ; 
seq.curframe=seq.curframe+1 ;
end