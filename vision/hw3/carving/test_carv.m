% I = imread('simple.png'); 
I = imread('woodblock.jpg');
e = genEngMap(I); 
% imagesc(e); 
[ny,nx, z] = size(I); 
idxv = zeros(2,ny); 
idxh = zeros(2,nx); 
[mv, tv] = cumMinEngVer(e);
[mh, th] = cumMinEngHor(e); 
%% Create Vertical Seam 
idxv(2,:) = sum(tv,2);  % 0 everywhere except the index 
idxv(1,:) = [1:ny];
vec_v = sub2ind(size(I), idxv(1,:),idxv(2,:));
%Create a red line where the seam is 
newIv = I; 
newIv(vec_v) = 255; 
vec_v = vec_v+ (nx*ny); 
newIv(vec_v) = 255; 
vec_v = vec_v + (nx*ny); 
newIv(vec_v) = 0; 
%% Create Horizontal Seam 
newIh = I; 
idxh(1,:) = sum(th,1); 
idxh(2,:) = 1:nx;
vec_h = sub2ind(size(I), idxh(1,:),idxh(2,:)); 
newIh(vec_h) = 255; 
vec_h = vec_h + nx*ny; 
newIh(vec_h) = 255; 
vec_h = vec_h + nx*ny; 
newIh(vec_h) = 0; 
%% show image
figure; 
subplot(2,1,1)
imshow(newIh); hold on;
scatter(idxh(2,:)', idxh(1,:)','bo');
subplot(2,1,2); 
imshow(newIv); hold on; 
scatter(idxv(2,:)', idxv(1,:)','bo');

[Irm_h Eh] = rmHorSeam(I,mh,th); 
[Irm_v Ev] = rmVerSeam(I,mv,tv);
figure; 

subplot(3,1,1); 
title('Original Image');
imshow(I); 
subplot(3,1,2); 
title('Horizontal Seam Removed');
imshow(Irm_h);
subplot(3,1,3); 
title('Verticle Seam Removed');
imshow(Irm_v); 