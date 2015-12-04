function [J, theta, Jx, Jy] = findDerivatives(I, Gx, Gy)

%%pad array with size of conv filter 
pad_I = padarray(I,[floor(size(Gx,2)/2) floor(size(Gx,2)/2)],'symmetric'); 
% 1d convolution of padded image and Gy and Gx 
first_conv =conv2(pad_I,Gy,'valid'); 
second_conv = conv2(first_conv,Gx,'valid');
%% Difference filter 
dx = [1 -1]; 
dy = dx';
%% Gradient in X Y and Theta Direction 
Jx = conv2(second_conv,dx,'same'); 
Jy = conv2(second_conv,dy,'same');
theta = atan2(Jy, Jx); 
% Gradient Magnitude 
J = sqrt(Jx.^2 + Jy.^2);  
end