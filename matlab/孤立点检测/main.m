clc,clear all,close all;
img = imread('Fig1004(b)(turbine_blade_black_dot).jpg');
imshow(img);
w = fspecial('laplacian',0.5);
img_w = imfilter(img,w);
figure(2)
imshow(uint8(img_w)>240)