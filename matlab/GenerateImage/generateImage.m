clear all;
clc;
x = zeros(250,250);
%x(21:230,26:32) = ones(210,7);
[M,N] = size(x);
figure(1);
imshow(x);
for i = 1:9
    x(21:230,26+24*(i-1):32+24*(i-1)) = ones(210,7);
end
figure(2);
imshow(x);
%print -dpng 'myplot.png' %用matlab命令保存的图像外面有白边，不好
imwrite(x,'x.jpg');