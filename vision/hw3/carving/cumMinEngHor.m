function [My, Tby] = cumMinEngHor(e)
% e is the energy map.
% My is the cumulative minimum energy map along horizontal direction.
% Tby is the backtrack table along horizontal direction.
[nx,ny] = size(e);
rows = nx; 
cols = ny; 
My = inf(nx, ny);
Tby = zeros(nx, ny);
My(:,1) = e(:,1);

Tby = zeros(nx, ny);
 
for j =2:cols 
    for i = 1:rows 
        if (  i < rows && i > 1)
              m_value= min(My((i -1):(i+1), j-1));
              My(i,j) = e(i,j) + m_value;
        elseif( i == rows)
               m_value= min(My((i-1):i, j-1)); 
               My(i,j) = e(i,j) +m_value;
        else
              m_value = min(My(i:i+1,j-1)); 
             My(i,j) = e(i,j)+ m_value; 
        end
            
    end
end


[~,l] = min(My(:,end)); 
Tby(l,end ) = l; 

for k =cols-1:-1:1
    if(l > 1 && l < rows ) 
        idx_vec = l-1:l+1; 
        [~, idx] = min(My(l-1:l+1,k)); 
        id = idx_vec(idx); 
    elseif l == cols 
        idx_vec = l-1:l;
        [~, idx] = min(My(idx_vec,k));
        id = idx_vec(idx); 
    else
        idx_vec = l:l+1;
        [~,idx] = min(My(idx_vec,k)); 
        id = idx_vec(idx); 
    end 
    
    Tby(id,k) = id;
    l = id; 
end

end