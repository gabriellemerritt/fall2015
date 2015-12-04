function E = edgeLink(M, J, theta)

nc = size(J,2);  
nr = size(J,1); 
% low_threshold = max(mean(J)); 
E = false(nr,nc);
%% throw out outliers so our mean isn't shifted super far out
no_outliers = J;
[~,idx] = max(mean(J,1));  
no_outliers(:,idx) = [];
[~,idx2] = max(mean(J,2));  
no_outliers(idx2,:) = []; 
%% Set Low threshold in X and Y directions (just taking the mean of the mag)
low_threshold = mean(mean(no_outliers,1)); 
low2 = mean(mean(no_outliers,2)); 

%% just initializing some logic vectors for later
logic = zeros(nr,nc); 
logic2 = zeros(nr,nc); 
logic4 = zeros(1,nc); 
logic3 = zeros(nr,1); 
logic_4 = zeros(1,nc);
logic5 = zeros(nr,nc); 
%% High threshold is 1 std above the mean 
high_threshold = low_threshold + mean(std(no_outliers,0,1));
high2 = low2 + mean(std(no_outliers,0,2));
%% Filtering out points with non max 
new_J = J.*M; 

%%  check if filtered mag is above high threshold
for j= 1:nc 
  logic_vec = new_J(:,j) > high_threshold;
    logic_vec2 = (J(:,j) > low_threshold); % if in hysterisis band check to see if left right neighbors are above high threshold
  if(j >1 && j < nc) 
     logic3 = logic_vec2 & ((new_J(:,j-1) > high_threshold) | ( new_J(:,j+1) > high_threshold)); 
  end
  
  %storing the defs valid points in new_J , creating a logic matrix to deal
  %with up down condition later
   new_J(:,j) = logic_vec; 
   logic(:,j) = logic3; 
end

%% checking for valid points for up and down 
for i = 1:nr
    l = new_J(i,:) > high2; 
    logic_4 = (J(i,:) > low2);

    if ( i >1 && i < nr) % check top and bottom neighbor for definite edges
        logic4 = logic_4 & (( new_J(i-1,:) > high2) | (new_J(i+1,:) > high2)); 
    end 
        
    logic2(i,:) = logic4; 
    E(i,:) = new_J(i,:) | l ; 
end 

%% this did check the corners of the box for any high thresholds 
%%   to link the edges but it was slow and didnt help


% for m = 1:nr
%    if(m > 1 && m < nr) 
%      for n = 1:nc 
%         if(n >1 && n <nc)
%              if(J(m,n) > mean([low_threshold low2])) && J(m,n)< mean([high_threshold high2]) 
%                 logic5(m,n) = (new_J(m -1,n-1) > mean([high_threshold high2]))| ...
%                     (new_J(m +1, n-1) >mean ([high_threshold high2]))| ...
%                     (new_J(m +1, n+1) > mean([high_threshold high2]))| ... 
%                     (new_J(m-1, n+1) > mean([high_threshold high2])); 
%              end 
%         end
%      end
%    end
% end

%% combining the row and column checks by or'ing them together
%%but some times anding looks better
logic = logic | logic2; 
new_J = new_J | logic; 
E = new_J; 

end 
