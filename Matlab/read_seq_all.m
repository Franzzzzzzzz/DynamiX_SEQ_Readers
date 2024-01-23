%data=read_seq_all(seq)

function data=read_seq_all(seq)
if (seq.frames*seq.sz(1)*seq.sz(2)*2/1024/1024/1024>2) 
  res=input('Warning: you are loading a lot of data (>2Go). Proceed (Y,N)?') ; 
  if (strcmp(res, 'N')) data=[] ; return ; end ;
end ; 
data=zeros(seq.sz(1), seq.sz(2), seq.frames-seq.curframe ,'uint16') ;
for i=seq.curframe:seq.frames-1
  data(:,:,i-seq.curframe+1)=read_seq_next(seq) ; 
end ; 

assignin('caller', 'seq', seq) ; 
end


