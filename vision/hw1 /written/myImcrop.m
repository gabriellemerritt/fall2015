function J = myImcrop(I)

 raw_I = imread(I); 
 H = figure; 
 imshow(raw_I);
 [x,y] = ginput; 
 hold on  
 plot(x, y, 'g-'); 
 
 
 figure 
 jmin = floor(min(y)); 
 jmax = floor(max(y));  
 imin = floor(min(x)); 
 imax = floor(max(x));  
 imshow(raw_I(jmin:jmax,imin:imax,:))
 
 
 


end