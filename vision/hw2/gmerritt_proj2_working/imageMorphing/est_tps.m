function [a1,ax,ay,w] = est_tps(mean_ctrl_pts, target_value)
%%set lambeda 
lambda = eps;

%% these are the target pixels
t_v = [target_value; zeros(3,1)]; 
n = size(mean_ctrl_pts,1);
K= zeros(n,n); %% in this case n is = p
for a = 1:n
   for b= 1:n
       %% r^2 log10(r^2)
       r_2 = abs(mean_ctrl_pts(a,1) - mean_ctrl_pts(b,1)).^2  + abs(mean_ctrl_pts(a,2) - mean_ctrl_pts(b,2)).^2;
       if r_2 < eps
           K(a,b) = 0;
       else
           K(a,b) =  r_2*log10(r_2);
       end
       
   end
end
%% some log values will return nan, set nans to 0 
% K(isnan(K)) = 0; 
P = [ mean_ctrl_pts ones(n,1)];

A = [K  P ; P' zeros(3,3)]+(lambda* eye(n+3, n+3)); 
[coef] = A\t_v;
%% the order might be wrong the slides say different things 
w = coef(1:n); 
ax = coef(end -2); 
ay = coef(end -1); 
a1 = coef(end); 

end
