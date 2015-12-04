function [Mx, Tbx] = cumMinEngVer(e)
% e is the energy map.
% Mx is the cumulative minimum energy map along vertical direction.
% Tbx is the backtrack table along vertical direction.
[nx,ny] = size(e);
Mx = inf(nx, ny);
Mx(1,:) = e(1,:); 
% for i=2:nx*ny
%     if( i <= nx+1)  
%         
%         Mx(i) = e(i)+ min(Mx(i-1), Mx((i-1)+nx));
%         
%     elseif (i +nx) > nx*ny
%             Mx(i) = e(i)+ min(Mx(i-1), Mx(i-1-nx)); 
%     else
%          Mx(i) = e(i) + min(min(Mx(i-1), Mx(i-1-nx)), Mx((i-1)+nx)); 
%     end
%             
% end

Tbx = zeros(nx, ny);

Mcheck = inf(nx,ny); 
Mcheck(1,:) = e(1,:); 
for n =2:nx 
    for m = 1:ny 
        if (  m < ny && m > 1)
              m_value= min(Mcheck(n-1,(m -1):(m+1)));
            Mcheck(n,m) = e(n,m) + m_value;
        elseif( m == ny)
               m_value= min(Mcheck(n-1, (m-1):m)); 
               Mcheck(n,m) = e(n,m) +m_value;
        else
              m_value = min(Mcheck(n-1,m:m+1)); 
             Mcheck(n,m) = e(n,m)+ m_value; 
        end
            
    end
end

Mx = Mcheck;
[~,j] = min(Mx(end,:)); 
Tbx(end,j) = j; 

for i =nx-1:-1:1
    if(j > 1 && j < ny ) 
        test = j-1:j+1; 
        [~, idx] = min(Mx(i,j-1:j+1)); 
        id = test(idx); 
    elseif j == ny 
        test = j-1:j;
        [~, idx] = min(Mx(i,test));
        id = test(idx); 
    else
        test = j:j+1;
        [~,idx] = min(Mx(i,test)); 
        id = test(idx); 
    end 
    
    Tbx(i,id) = id;
    j = id; 
end




end



%% Add your code here

