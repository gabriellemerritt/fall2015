function [y, x, rmax] = anms(cimg, max_pts) 
%% imregionalmax get rid of corners
[nx, ny] = size(cimg); 
map(1,:) = reshape(cimg, 1,nx*ny); 
map(2,:) = zeros(1,nx*ny); % radii 

% threshold = mean(min((cimg))) + 2* mean(mean((cimg))); 
threshold = mean(max((cimg)));  


for (i = 1:nx*ny)
         if(map(1,i) > threshold)
             [I,J] = find(cimg > map(1,i));  
             %faster than ind2sub
             if (isempty([I,J]))
                 map(2,i) = Inf;
                 continue; 
             end
             r  = mod(i -1,nx) +1; 
             c =  (i-r)/nx +1; 
             map(2,i) = min(sqrt((I -r).^2 + (J -c).^2)); 
         else
             map(2,i) = -Inf;
         end
    
end 

map(3,:) = [1:nx*ny]; 
map = map'; 
%% sort second column (radii) in descending order 
map = sortrows(map,-2); 
repeated_radius = find(map(:,2) == map(max_pts,2));  

if(size(repeated_radius,1) > 1)
    max_pts = max_pts - (max_pts - min(repeated_radius)) -1;
end
lin = map(1:max_pts,3); 
valid_points = map(1:max_pts,:); 
[y, x] = ind2sub([nx ny], lin);
x(((x -20) < 1) | ((x +20) > ny) | ((y -20) < 1) | ((y+20)> nx)) = -1; 
y(((x -20) < 1) | ((x +20) > ny)| ((y -20) < 1) | ((y+20)> nx)) = -1;

y(y== -1) = []; 
x(x == -1) = []; 
%% we had to flip the indicies 
idx = y(end) + (x(end)-1)*nx;
%% find which radius matches the new index max radius 
rmax = valid_points(valid_points(:,3) == idx,2); 
end