clc,clear all,close all
lena = imread('../lena.jpg');
lena = rgb2gray(lena);

w = fspecial('average',[3,3]);
lena_blured = imfilter(lena,w,'replicate');
imshow(lena);title('ԭͼ')
figure
imshow(lena_blured);title('ģ������')
g = lena - lena_blured;
figure
imshow(g),title('gmask')
k = 5;
lena_g = lena + k*g;
figure;
str = sprintf('�������˲����,k=%d',k);
imshow(lena_g);title(str);