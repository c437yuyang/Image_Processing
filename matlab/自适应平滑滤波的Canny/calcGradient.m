function [Gx,Gy] = calcGradient(img)
[m,n] = size(img);   
Gx = zeros(m,n);
Gy = zeros(m,n);
for i=2:m-1
    for j=2:n-1
        Gx(i,j) = 0.5*(img(i+1,j) - img(i-1,j));
        Gy(i,j) = 0.5*(img(i,j+1) - img(i,j-1));
    end
end
end