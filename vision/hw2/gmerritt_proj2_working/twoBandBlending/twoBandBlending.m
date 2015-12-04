function [im_blend, im1_low, im1_high, im2_low, im2_high] = twoBandBlending(im1, im2) 

%% don't greyscale just convolute each color seperately 
% if (size(im1,3) > 1)
%      I1 = rgb2gray(im1);
% end 
% 
% if (size(im2,3) > 1)
%      I2 = rgb2gray(im2);
% end 

I1 =im2double(im1) ;
I2 = im2double(im2); 



Gx = normpdf([-5:1:5], 0, 1);
Gy = Gx';
pad_I1 = padarray(I1,[floor(size(Gx,2)/2) floor(size(Gx,2)/2)],'symmetric'); 
pad_I2 = padarray(I2,[floor(size(Gx,2)/2) floor(size(Gx,2)/2)],'symmetric'); 
im1_low = zeros(size(im2,1),size(im2,2),size(im2,3)); 
im2_low = zeros(size(im2,1),size(im2,2),size(im2,3)); 
for i = 1:3
    I1_conv1 = conv2(pad_I1(:,:,i),Gy,'valid'); 
    im1_low(:,:,i) = conv2(I1_conv1,Gx,'valid');
    I2_conv1 =conv2(pad_I2(:,:,i),Gy,'valid'); 
    im2_low(:,:,i) = conv2(I2_conv1,Gx,'valid');
end


% I1 = im2double(I1); 
im1_high =  I1 - im1_low;
im2_high  = I2 - im2_low;
%% debugging output
% figure
% imagesc(I2); colormap(gray);
% 
% figure
% imagesc(I1);colormap(gray);
% % 
% figure
% imagesc(im1_low); colormap(gray);
% figure
% imagesc(im2_low); colormap(gray);
% 
% figure
% imagesc(im1_high); colormap(gray);
% figure
% imagesc(im2_high); colormap(gray);
smooth = [0:.0575:1];
smooth = smooth /sum(smooth); 
smoothy = smooth;
lin_filt = conv2(smoothy, smooth); 

% disp(' after clicking polygon right click a control point and create mask'); 
%w_binary = roipoly(im2); 
disp(' Currently auto loading binary mask from .mat file'); 
w_binary = binaryMask(); 
d_binary = im2double(w_binary); 
w_bin_pad = padarray(d_binary,[floor(size(lin_filt,1)/2) floor(size(lin_filt,2)/2)],'symmetric'); 
% figure;
% imshow(w_binary);
% lin_filt = conv2(smoothy, smooth); 
% w_lin = conv2(w_bin_pad, lin_filt, 'valid'); 
w_lin = conv2(w_bin_pad, lin_filt,'valid'); 
% figure;
% imshow(w_lin);
% w_lin(450,:) = []; 
% w_lin(:,641) = [];

%%
for (i = 1:3)
  im_blend_high(:,:,i) = im2_high(:,:,i).*d_binary + im1_high(:,:,i).*(1 - d_binary) ;
end
figure;
imshow(im_blend_high);
for (i = 1:3) 
    im_blend_low(:,:,i) = im2_low(:,:,i).*w_lin + im1_low(:,:,i).* (1 - w_lin);
end 
figure;imshow(im_blend_low);

im_blend = im_blend_high + im_blend_low; 
figure; 
imshow(im_blend); 

end