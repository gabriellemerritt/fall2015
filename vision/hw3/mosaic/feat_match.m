function m = feat_match(p1,p2)
%abitrary theshold 
%%
threshold = .95; 
[ ~ , col] = size(p1); 
p1 = p1'; 
p2 = p2'; 
m = -1*ones(col,1); 
%%
for i = 1:col 
    %search for closest distance between descriptors 
    [idx1, SSD1] = dsearchn(p2, p1(i,:));
%% remove closest neighbor 
    p_t = p2;
    p_t(idx1,:) = []; 
%% search for second closest neighbor
    [idx2, SSD2] = dsearchn(p_t, p1(i,:));     
%% test to see if closest neighbor is good unique feature descriptor
    if( SSD1/SSD2 <= threshold) 
        m(i) = idx1; 
    else
        %if it isnt set index to -1 
        m(i) = -1; 
    end
end
end