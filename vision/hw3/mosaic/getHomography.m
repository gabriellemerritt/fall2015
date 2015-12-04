function [H] = getHomography(I,I2,show_matching)

I = im2double(I); 
I2 = im2double(I2); 

i1 = rgb2gray(I); 
i2 = rgb2gray(I2); 

C1 = abs(cornermetric(i1));
C2 = abs(cornermetric(i2)); 
figure;
subplot(1,2,1);
imshow(C1);
subplot(1,2,2);
imshow(C2); 


[y1, x1, rmax1] = anms(C1,450); 
[y2, x2, rmax2] = anms(C2,450); 



p1 = feat_desc(i1,y1,x1); 
p2 = feat_desc(i2,y2,x2);  

m = feat_match(p1,p2);
corr = [ (1:length(m))', m]; 
idx_corr = corr(corr(:,2) > 0,:);

x_1 = x1(idx_corr(:,1));
y_1 = y1(idx_corr(:,1)); 

x_2 = x2(idx_corr(:,2)); 
y_2 = y2(idx_corr(:,2)); 


[H, inlier_ind] = ransac_est_homography(y_1,x_1, y_2, x_2,10);
 
if(show_matching) 
    figure; 
    subplot(1,2,1); imshow(i1); 
    hold on; 
    scatter(x_1,y_1,'bo');
    labels1 = cellstr(num2str((1:length(x_1)).'));
    %% ransac matches
    scatter(x_1(inlier_ind),y_1(inlier_ind),'ro'); 
    text(x_1,y_1, labels1, 'Color',[1 1 0]); 
    subplot(1,2,2); 
    imshow(i2);  
    hold on; 
    scatter(x_2,y_2,'bo');
    %ransac matches 
    scatter(x_2(inlier_ind), y_2(inlier_ind),'ro'); 
    labels2 = cellstr(num2str((1:length(x_2)).'));
    text(x_2,y_2,labels2,'Color', [1 1 0]); 
    hold off; 
    
figure;
subplot(1,2,1);
imshow(i1); 
hold on; 
scatter(x1,y1,'yo'); 
title('Non Max Suppression Points');
subplot(1,2,2); 
imshow(i2); 
hold on; 
scatter(x2,y2,'yo'); 
end

end 