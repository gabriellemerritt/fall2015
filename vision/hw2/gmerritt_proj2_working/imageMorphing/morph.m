function morphed_im = morph(im1, im2, im1_pts, im2_pts, tri, warp_frac, dissolve_frac)
%% convert images to doubles
m = size(im1,1);
n = size(im1,2); 
im1 = im2double(im1); 
im2 = im2double(im2); 
%% Create list of pixel positions for image 1
[X,Y] = meshgrid(1:n, 1:m);
[rowsub , colsub] = ind2sub(size(im1(:,:,1)),1:m*n);
im1_ind = [colsub; rowsub]';
% im1_ind = [ rowsub; colsub]';
%% warp ctrl points 
impoints = (1-warp_frac).*im1_pts + warp_frac.*im2_pts;
%% creating intermediate mean images
imwarp1 = ones(size(im1,1), size(im1,2),size(im1,3)); 
imwarp2 = ones(size(im1,1), size(im1,2),size(im1,3)); 

%% compute barycentric coordinates  
T = tsearchn(impoints, tri, im1_ind); %% Triangle indicies for intermediate image

%%Loop on color
for c = 1:3 
    %% loop on triangle list
    for i = 1: size(tri,1)
      tri_corners = impoints(tri(i, :),:)'; %% triangle corners in warp image im points in x y 
      tri_corners(3,:) = ones(); 
      warp_coords = im1_ind((T == i), :); % x y 
      homo_warp = [warp_coords ones(size(warp_coords,1),1)]'; 
      %% Barycentric Coordinates 3 x n 
      abc = tri_corners\homo_warp; 

      tri_source1 = im1_pts(tri(i,:),:)'; %% triangle corners in source images 
      tri_source1(3,:) = ones(); 
      map1_coords = tri_source1* abc; %% pixel coordinates in source 1 
      map1_coords = [map1_coords(1,:)./ map1_coords(3,:); map1_coords(2,:)./map1_coords(3,:)]'; 

      tri_source2 = im2_pts(tri(i,:),:)'; %% triangle corners in source images 
      tri_source2(3,:) = ones(); 
      map2_coords = tri_source2* abc; %% pixel coordinates in source 2 
      map2_coords = [map2_coords(1,:)./ map2_coords(3,:); map2_coords(2,:)./map2_coords(3,:)]'; 

      %% rounding to get integer indicies  
      warp_coords = round(warp_coords); 
      map1_coords = round(map1_coords); 
      map2_coords = round(map2_coords); 
      
      %% bounding the image
      warp_coords(:,1) = min(warp_coords(:,1), size(imwarp1,2)); 
      warp_coords(:,2) = min(warp_coords(:,2), size(imwarp1,1)); 
      warp_coords = max(warp_coords, ones(size(warp_coords,1),size(warp_coords,2))); 
%       
      
      map1_coords(:,1) = min(map1_coords(:,1), size(im1,2)); 
      map1_coords(:,2) = min(map1_coords(:,2), size(im1,1)); 
      map1_coords = max(map1_coords, ones(size(map1_coords,1),size(map1_coords,2))); 
% 
      map2_coords(:,1) = min(map2_coords(:,1), size(im2,2)); 
      map2_coords(:,2) = min(map2_coords(:,2), size(im2,1)); 
      map2_coords = max(map2_coords, ones(size(map2_coords,1),size(map2_coords,2))); 

      
      %% matlab indexing hell again
      
       idx_warp = sub2ind(size(imwarp1),warp_coords(:,2), warp_coords(:,1), c*ones(size(warp_coords,1),1));
       idx_im1 = sub2ind (size(im1),map1_coords(:,2), map1_coords(:,1), c*ones(size(map1_coords,1),1)); 
       idx_im2 = sub2ind (size(im2),map2_coords(:,2), map2_coords(:,1), c*ones(size(map2_coords,1),1)); 


      imwarp1(idx_warp) = im1(idx_im1);
      imwarp2(idx_warp) = im2(idx_im2); 
    end
end
%% cross dissolve blending 
    morphed_im = (1-dissolve_frac).*imwarp1 + dissolve_frac.*imwarp2;

end