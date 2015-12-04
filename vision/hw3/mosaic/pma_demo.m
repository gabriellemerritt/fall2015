I = imread('11.jpg'); 
I2 = imread('12.jpg'); 
I3 = imread('13.jpg'); 
img_input = cell(3,1); 

img_input{1} = I; 
img_input{2} = I2; 
img_input{3} = I3;  

[mosaic] = mymosaic(img_input); 
figure;
imshow(mosaic); 