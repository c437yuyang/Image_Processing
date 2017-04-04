function j=gaosi(i);
%i必须为二维double矩阵
j=i;
[h,w]=size(i);
for m=2;h-1
    for n=2:w-1
        j(m,n)=(i(m,n-1)+2*i(m,n)+i(m,n+1))/4;%横向高斯滤波
    end
end