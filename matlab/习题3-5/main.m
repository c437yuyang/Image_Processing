clc,clear all,close all
lena = imread('../lena.jpg');
lena = rgb2gray(lena);
imhist(lena);
lena_high = bitand(lena,240);
figure;
imhist(lena_high);
lena_low = bitand(lena,15);
figure;
imhist(lena_low);
