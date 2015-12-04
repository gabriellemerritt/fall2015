function[ im1_pts, im2_pts] = click_correspondences(im1,im2)
% max_r = max(size(im1,1), size(im2,1)); 
% max_c = max(size(im1,2), size(im2,2)); 
% im1(end:max_r,end:max_c,:) = zeros(); 
% im2(end:max_r,end:max_c,:) = zeros();

[im2_pts, im1_pts] = cpselect(im2,im1,'Wait', true); %% export to workspace 


end 
