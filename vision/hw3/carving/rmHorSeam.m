function [Iy, E] = rmHorSeam(I, My, Tby)
% I is the image. Note that I could be color or grayscale image.
% My is the cumulative minimum energy map along horizontal direction.
% Tby is the backtrack table along horizontal direction.
% Iy is the image removed one row.
% E is the cost of seam removal

[nx, ny, nz] = size(I);
% rmIdx = zeros(1, ny);
% rmIdx = sum(Tby,1); 
idxh = zeros(2,ny); 
idxh(1,:) = sum(Tby,1); 
idxh(2,:) = 1:ny;
vec_h = sub2ind(size(I), idxh(1,:),idxh(2,:)); 
energy_h = sub2ind(size(My),idxh(1,:),idxh(2,:));
E = min(My(:,end)); 

if(nz > 1)

    %indicies for removal
    vec_rm = [ vec_h  vec_h+nx*ny  vec_h+(2*nx*ny)];
else
    vec_rm = vec_h;
end

Iy = I; 
Iy(vec_rm) = []; 
Iy = reshape(Iy, nx-1, ny, nz); 
assert(all(size(Iy) == [nx-1 ny nz])); 
end