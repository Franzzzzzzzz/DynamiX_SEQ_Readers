% seq=open_seq (filename) open a seq file made with my program in the xray

function seq=open_seq (filename)
seq=struct() ;
if (exist(filename)~=2), disp('Cannot find file') ; return ; end ;

logname=regexprep(filename, '[.]seq$', '.log') ;
if (exist(filename)~=2), disp('Need a log file') ; return ; end ;

try 
    f=jsondecode(fileread(logname)) ; 
    seq.date = f.timestamp;     
    seq.mode = f.detector.mode;
    seq.sz(1) = f.detector.image_size.width;
    seq.sz(2) = f.detector.image_size.height;
    seq.ROI = f.detector.ROI;
    seq.fps = f.detector.fps;
    seq.frames = length(f.detector.frames)
catch 
    disp('The log file provided is not available or not valid json. Continuing assuming the legacy log file format') ; 
    f=fopen(logname) ; 
    tmp=textscan(f, '%[^\n]\n\n') ; 
    seq.date=tmp{1} ;
    tmp=textscan(f, 'MODE %d') ;
    seq.mode=tmp{1} ; 
    tmp=textscan(f, '%dx%d') ;
    seq.sz(1)=tmp{1} ; seq.sz(2)=tmp{2} ; 
    tmp=textscan(f, 'ROI = %d %d, %d %d') ;
    seq.ROI(1)=tmp{1} ; seq.ROI(2)=tmp{2} ; seq.ROI(3)=tmp{3} ; seq.ROI(4)=tmp{4} ; 
    tmp=textscan(f, 'FPS %f') ;
    seq.fps=tmp{1} ; 

    n=0 ; 
    tmp=textscan(f, '%d') ; 
    seq.frames=length(tmp{1}) ; 
    fclose(f) ; 
end ;

seq.curframe=0 ; 
seq.file=fopen(filename) ;

end
