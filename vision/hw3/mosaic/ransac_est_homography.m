function [H, inlier_ind] = ransac_est_homography(y1,x1,y2,x2,thresh)
% take 4 at random and est homography 
% go through all the other points see if reprojects within threshold 
 %save set of inliers within distance threshold and the max number 
% if  new set has more inliers save the new inliers and new max 
% run ~1000 times  
% at the end recompute H with max num inliers set 
inlier_ind = []; 
%% pts1 IS SOURCE

%% pts2 is destination 

%%  pts2 = H (pts1)

for (i = 1:5000)
    % take 4 random points from 
    rand_idx = randsample(size(x2,1),4); 
  
    fit_H = est_homography(x1(rand_idx), y1(rand_idx), x2(rand_idx), y2(rand_idx)); 
    [estX estY] =  apply_homography(fit_H, x2, y2); 
    
    SSD = ((x1 - estX).^2 + (y1 - estY).^2);  
    errorx = x1 - estX; 
    errory = y1 - estY; 
    inliers = (SSD <= thresh); 
    
    if (sum(inliers) > sum(inlier_ind)) 
        inlier_ind = inliers; 
    end 
    
end

H = est_homography(x1(inlier_ind), y1(inlier_ind), x2(inlier_ind), y2(inlier_ind)); 
end 