function crop_seq(filename, varargin)
%crop_seq(filename, [ROI]) Crop a seq file to make it more size manageable
%   Detailed explanation goes here

seq = open_seq(filename) ;
I=read_seq_next(seq) ;
close_seq(seq) ; 

if (nargin>1)
    ROI=varargin(1) ;
    disp(ROI)
    x(1)=ROI{1}.left ; 
    x(2)=ROI{1}.right ; 
    y(1)=ROI{1}.top ; 
    y(2)=ROI{1}.bottom ; 
else
    imagesc(I') ;
    axis image ;
    [x,y] = ginput(2) ; 
    hold all ;
    line([x(1), x(2)],[y(1) y(1)], 'Color', 'r') ; 
    line([x(1), x(2)],[y(2) y(2)], 'Color', 'r') ; 
    line([x(1), x(1)],[y(1) y(2)], 'Color', 'r') ; 
    line([x(2), x(2)],[y(1) y(2)], 'Color', 'r') ; 
end
x=round(x) ; 
y=round(y) ; 
disp('x') ; 
disp(x) ; 
disp('y') ; 
disp(y) ; 
disp('Press any key to proceed.')
pause() ; 

% JSON
filetext = fileread([filename(1:end-4), '.log']);
JS=jsondecode(filetext) ;
JS.detector.ROI.left = min(x(1),x(2)) ; 
JS.detector.ROI.right = max(x(1),x(2)) ; 
JS.detector.ROI.top=min(y(1),y(2)) ; 
JS.detector.ROI.bottom=max(y(1),y(2)) ; 
JS.detector.image_size.width=abs(x(2)-x(1))+1 ; 
JS.detector.image_size.height=abs(y(2)-y(1))+1 ;
txt=prettyjson(jsonencode(JS)) ; 

fid = fopen([filename(1:end-4), '-crop.log'],'wt');
fprintf(fid, '%s', txt);
fclose(fid);

%SEQ
seq = open_seq(filename) ;
fid = fopen([filename(1:end-4), '-crop.seq'],'wb');
for i=1:seq.frames
    I=read_seq_next(seq) ;
    I=I(min(x(1),x(2)):max(x(1),x(2)), min(y(1),y(2)):max(y(1),y(2))) ; 
    fwrite(fid, I, 'uint16') ; 
end
fclose(fid) ; 

end

