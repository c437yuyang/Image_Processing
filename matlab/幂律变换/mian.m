clc,clear all,close all
lena = imread('../lena.jpg');
lena = rgb2gray(lena);
lena = double(lena);
L = 255;
r = 0.3;
c = 1;

lena_gamma = c*((lena/L).^0.5)*255;%������ת����double���Ͳ�����С���ݴ�

figure(1)
subplot(121);imshow(uint8(lena));title('ԭͼ');

str = sprintf('���ɱ任��ͼ��r=%.2f,c=%d',r,c);
subplot(122);imshow(uint8(lena_gamma));title(str);