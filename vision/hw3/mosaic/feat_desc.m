function p = feat_desc(im,y,x)
%take 40 by 40 window 
% imgaussfilt to blur it 
% take every 5th pixel to make 8x8 
%normalize by  taking x- mean(x)/ std(x-mean(x) 
%%
% lin = sub2ind(size(I), y', x');
% des_list = im(lin);  
im = im2double(im); 
%% 40 x 40 window for each point 
for i = 1:size(y,1) 
     filt_win = imgaussfilt(im((y(i) - 20):(y(i) + 19),(x(i)-20):(x(i)+19)),1); 
     sampled_win = filt_win(1:5:end, 1:5:end); 
     sampled_win = sampled_win - mean(sampled_win(:)); 
     sampled_win =  sampled_win/std(sampled_win(:)); 
     p(:,i) = reshape(sampled_win,[64,1]); 
end 

end 