function k=rgb2y(z)
%i±ØĞëÎªrgbÈıÎ¬¾ØÕó
[m,n,p]=size(z);
k=zeros(m,n);
z=double(z);
for i=1:m
    for j=1:n
       k(i,j)=0.3*z(i,j,1)+0.6*z(i,j,2)+0.1*z(i,j,3);
    end
end