%% create video 
fileName = 'tri_morph.avi'; 
im1 = imread('sec.jpg');
im2 = imread('doge.jpeg'); 
% warp_frac = .5; 
% dissolve_frac = .5; 
% 
[ im1_pts, im2_pts] = click_correspondences(im1,im2);
tri = delaunay(im1_pts);
num_frames = 10;
writerObj = VideoWriter(fileName);
open(writerObj);

h3 = figure; 
set(gca, 'nextplot', 'replacechildren');
set(gcf, 'Renderer', 'zbuffer');
hold on 
warp_idx = [0:(1/60): 1]; 

for (i = 1:60)

morphed_im = morph(im1, im2, im1_pts, im2_pts, tri, warp_idx(i), warp_idx(i));
imshow(morphed_im);
% making a 20 second video 
for j= 1:num_frames
    F = getframe(h3);
    writeVideo(writerObj, F);
end

end

 close(writerObj); 
 