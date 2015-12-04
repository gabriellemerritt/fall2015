function morphed_im = morph_tps(im_source, a1_x, ax_x , ay_x, w_x,a1_y, ax_y, ay_y, w_y, ctrl_pts,sz)
%% makes a n x 2 array of pixel positions (x,y)
[X,~] = meshgrid(1:sz(1), 1:sz(2));
[rowsub , colsub] = ind2sub(size(X),1:sz(1)*sz(2));
len = sz(1)*sz(2); 
im_ind = [colsub; rowsub]'; 

warp_im1 = zeros(1,sz(1)*sz(2)); 
warp_im2 = zeros(1,sz(1)*sz(2)); 

%% p is length of mean pixel vector (mean of corresponding points) 
p = size(w_x,1);
morphed_im = zeros(sz(1),sz(2),3);
%% loop through list of pixel positions 
parfor i = 1:len
    % add affine transformations to a place holder
    % a1 + ax * x + ay * y 
    vx = a1_x +ax_x*im_ind(i,1) + ay_x*im_ind(i,2);
    vy = a1_y +ax_y*im_ind(i,1) + ay_y*im_ind(i,2);
    sumx = 0;
    sumy = 0;
   for k = 1:p      
       %% Sum  L2 norm of ( x_i,y_i  - x_j, y_j) 
         r2x = abs(ctrl_pts(k,1) - im_ind(i,1)).^2.0 + abs(ctrl_pts(k,2) - im_ind(i,2)).^2.0; 
         %% apply U to norm and multiply by weights
         if r2x < eps
             continue;
         end
         Ux = r2x*log10(r2x)*w_x(k);
         Uy = r2x*log10(r2x)*w_y(k);
         sumx = sumx + Ux; 
         sumy = sumy + Uy;
%            sumx =  Ux; 
%            sumy =  Uy;


   end
       %%add the sum of the weighting function to  affine transformations
        warp_im_1(i) = vy+sumy; 
        warp_im_2(i) = vx+sumx; 
%         warp_im(i,1) = vy+sumy; 
        
end
warp_im(:,1) = warp_im_1; 
warp_im(:,2) = warp_im_2; 
%% Trying to bound pixel coordinates into image size %% 
 warp_im = max(round(warp_im), ones(size(warp_im)));
 min_r = min(size(im_source,1), sz(1)); 
 min_c = min(size(im_source,2), sz(2));
 
 warp_im(:,2) = min(round(warp_im(:,2)), min_r*ones(size(warp_im(:,2)))); 
 warp_im(:,1) = min(round(warp_im(:,1)),min_c*ones(size(warp_im(:,1))));
%% Loop through all of the colors
for(c =1:3)
    %% sub2ind  morphed_im(im_ind , c)<-- because matlab can't do this operations
    mindx = sub2ind(size(morphed_im), im_ind(:,2),im_ind(:,1), c*ones(sz(1)*sz(2),1));
    indx = sub2ind(size(im_source),warp_im(:,1),warp_im(:,2),c*ones(size(warp_im,1),1));
    morphed_im(mindx) = im_source(indx);
end
end
