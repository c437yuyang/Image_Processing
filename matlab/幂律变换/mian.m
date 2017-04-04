clc,clear all,close all
lena = imread('../lena.jpg');
lena = rgb2gray(lena);
lena = double(lena);
L = 255;
r = 0.3;
c = 1;

lena_gamma = c*((lena/L).^0.5)*255;%必须先转换成double类型才能算小数幂次

figure(1)
subplot(121);imshow(uint8(lena));title('原图');

str = sprintf('幂律变换后图像r=%.2f,c=%d',r,c);
subplot(122);imshow(uint8(lena_gamma));title(str);