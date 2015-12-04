 im1 = imread('nc.jpg');
 im2 = imread('gc.jpg'); 
%  [im1_pts, im2_pts] = click_correspondences(im1,im2);
load('nic.mat'); %% im so sleepy
fileName = 'parallel.avi'; 
% im1_target_value =  im1_pts(:,1); 
% im2_target_value = im2_pts(:,2);
warp_idx = [0:(1/60): 1]; 
num_frames = 5;
writerObj = VideoWriter(fileName);
open(writerObj);
% parpool(4);
h3 = figure; 
set(gca, 'nextplot', 'replacechildren');
set(gcf, 'Renderer', 'zbuffer');
hold on 
%     video = avifile(fileName,'FPS',60);
% h3 = figure; 
% set(gca, 'nextplot', 'replacechildren');
% set(gcf, 'Renderer', 'zbuffer');
hold on; 
for (i = 1:60)
%           morphed_im = morph_tps_wrapper(im1, im2, fp, mp, warp_idx(i), warp_idx(i));
        morphed_im = morph_tps_wrapper(im1, im2, im1_pts, im2_pts, warp_idx(i), warp_idx(i));
        imshow(morphed_im);
for j= 1:num_frames
%       video = addframe(video,morphed_im); 
     F = im2frame(morphed_im);
    writeVideo(writerObj, F);
    
   
end

end
 close(writerObj); 
