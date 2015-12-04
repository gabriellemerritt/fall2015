function [Ic, T] = carv(I, nr, nc)
% I is the image being resized
% [nr, nc] is the numbers of rows and columns to remove.
% Ic is the resized image
% T is the transport map

[nx, ny, nz] = size(I);
T = zeros(nr+1, nc+1);
TI = cell(nr+1, nc+1); 
TI{1,1} = I; 
alpha; 
% TI is a trace table for images. TI{r+1,c+1} records the image removed r rows and c columns.

%% Add your code here
k = nr+nc; 
for i = 1:k 
    
end 
Ic = TI{nr+1,nc+1};

end