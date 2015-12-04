function [Ix, E] = rmVerSeam(I, Mx, Tbx)
% I is the image. Note that I could be color or grayscale image.
% Mx is the cumulative minimum energy map along vertical direction.
% Tbx is the backtrack table along vertical direction.
% Ix is the image removed one column.
% E is the cost of seam removal

[nx, ny, nz] = size(I);


rm_idx(2,:) = sum(Tbx,2);  % 0 everywhere except the index 
rm_idx(1,:) = [1:nx];
%Have to transpose for this method to work 
Ix = permute(I,[2,1,3]);
vec_v = sub2ind(size(Ix), rm_idx(2,:),rm_idx(1,:));
energy_v = sub2ind(size(Mx),rm_idx(1,:),rm_idx(2,:)); 
E = min(Mx(end,:));
if ( nz > 1)
    rm_vec = [ vec_v, vec_v+(nx*ny), vec_v+(2*nx*ny)]; 
else 
    rm_vec = vec_v; 
end

%% Delete Seam 
Ix(rm_vec)= [];  

Ix = reshape(Ix, ny-1, nx,nz); 
Ix = ipermute(Ix,[2,1,3]); 
end