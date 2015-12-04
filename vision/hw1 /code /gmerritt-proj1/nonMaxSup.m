function M = nonMaxSup2(J, theta)
%% This function is kind of slow/ im too tired to linearize rn
nr = size(theta,1); 
nc = size(theta,2);
%% Detect local maximum
% making a false / true matrix for M because its faster
M = false(nr,nc); 
%discretizing theta into these 5 angles
pi_vec = [0 pi/4 pi/2 3*pi/4 pi]; 

%% Sorry I changed index conventions in edgeLink  :(
for j = 1:size(theta,1) % y
    for i = 1:size(theta,2) % x 
       [~,idx] =  min(abs(pi_vec - abs(theta(j,i)))); %matching theta to one of the angles in pi_vec
       %% Changing which indicies i check for a max based on the angle at J(j,i)
      if(pi_vec(idx) == 0 || pi_vec(idx) == pi)
          if(j > 1 && j < nr)
              if(J(j-1,i) < J(j,i) && J(j,i) > J(j+1,i))
                  M(j,i) =true; 
              end
          end
      end
             
      if(pi_vec(idx) == pi/4)
          if(j > 1 && j < nr && i>1 && i< nc)
              if(J(j+1,i-1)< J(j,i) && J(j-1,i+1) < J(j,i))
                  M(j,i) =true; 
              end
          end
      end
      
      if(pi_vec(idx) == pi/2)
         if(i>1 && i< nc)
            if(J(j,i-1) < J(j,i) && J(j,i+1) < J(j,i))
                  M(j,i) = true; 
             end
          end
      end
      
      if(pi_vec(idx) == 3*pi/4)
         if(j > 1 && j < nr && i>1 && i< nc)
            if(J(j -1,i-1) < J(j,i) && J(j+1,i+1) < J(j,i))
                  M(j,i) =true; 
             end
          end
      end
              
    end 
        
end
end
