function morphed_im = morph_tps_wrapper(im1, im2, im1_pts, im2_pts, warp_frac, dissolve_frac)
 im1 = im2double(im1); 
 im2 =  im2double(im2); 
 
 mean_ctrl_pts = (1-warp_frac).*im1_pts + warp_frac.*im2_pts;
 %% params for image 1
[a1_x,ax_x,ay_x,w_x] = est_tps(mean_ctrl_pts, im1_pts(:,1));
[a1_y,ax_y, ay_y,w_y] = est_tps(mean_ctrl_pts,im1_pts(:,2));
%%params for image 2 
[a1_x2,ax_x2,ay_x2,w_x2] = est_tps(mean_ctrl_pts, im2_pts(:,1));
[a1_y2,ax_y2, ay_y2,w_y2] = est_tps(mean_ctrl_pts,im2_pts(:,2));
%% always set size to first image 
sz = size(im1); 
ctrl_pts = mean_ctrl_pts; 
%% morph the images using params
morph1 = morph_tps(im1, a1_x, ax_x , ay_x, w_x,a1_y, ax_y, ay_y, w_y, ctrl_pts,sz);
morph2 =  morph_tps(im2, a1_x2, ax_x2 , ay_x2, w_x2,a1_y2, ax_y2, ay_y2, w_y2, ctrl_pts,sz);
%% cross disolve
 morphed_im = (1-dissolve_frac).*morph1 + dissolve_frac.*morph2;


end