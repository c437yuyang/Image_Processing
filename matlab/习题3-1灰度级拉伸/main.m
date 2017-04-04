clc,clear all,close all
lena = imread('../lena_50.bmp');
lena = rgb2gray(lena);
imshow(lena);
imhist(lena);
lena = double(lena);
L = 200;
C = 50;

max = max(lena(:));
min = min(lena(:));

lena_extend = (lena - min) ./ (max - min) * (L-C) + C;


lena_extend = uint8(lena_extend);
figure
imhist(lena_extend)