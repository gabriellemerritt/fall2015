function img_mosaic = mymosaic(img_input)


I = img_input{1}; 
len = length(img_input); 
Homos = cell(len,1); 
%% base transform for the first image 
Homos{1} = eye(3,3); 
% find transform for each picture to the base image ( which is the first
% one)
for i = 2: numel(img_input) 
    H = getHomography(img_input{i-1}, img_input{i},1);
    Homos{i} = Homos{i-1}*H;
end

for i = 1:len
    [nr ,nc, nz] = size(img_input{i}); 
    [xlim(i,:), ylim(i,:)] = apply_homography(Homos{i}, [1 1 nc nc]', [1 nr 1 nr]');  
end 
%% to find the middle picture
avgXLim = mean(xlim, 2); 
[~, idx] = sort(avgXLim); 
center_idx = floor((len+1)/2); 
center_img_idx = idx(center_idx); 
%% find inverse transform of middle transform and apply it to all subsequent transforms
homo_inv = inv(Homos{center_img_idx}); 
for i = 1:len  
    Homos{i} = homo_inv*Homos{i}; 
end 
%% Find new limits of panaorama 
for i = 1:len
    [ nr, nc, nz] = size(img_input{i});
    [xlim(i,:), ylim(i,:)] = apply_homography(Homos{i}, [1 1 nc nc]', [1 nr 1 nr]');  
end 
%% finding limits for panorama

xMin = min([1; xlim(:)]);
xMax = max([nc ; xlim(:)]);

yMin = min([1; ylim(:)]);
yMax = max([nr; ylim(:)]);

width  = round(xMax - xMin);
height = round(yMax - yMin);

xpanLimits =[xMin xMax]; 
ypanLimits =[yMin yMax];

%%initialize panorama

panorama = zeros([height width 3]); 
%% image reference
View = imref2d([height width], xpanLimits, ypanLimits); 

blender = vision.AlphaBlender('Operation', 'Binary mask', ...
    'MaskSource', 'Input port');

for i = 1:len 
    I = im2double(img_input{i});
    warpedImage = imwarp(I,projective2d(Homos{i}.'), 'OutputView', View);
    panorama = step(blender, panorama, warpedImage, warpedImage(:,:,1));
end
img_mosaic = panorama; 
% figure;
% imshow(img_mosaic); 
end 




