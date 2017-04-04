clc,clear all,close all;

img = imread('../三色测试图.bmp');



figure(1);
subplot(221);imshow(img);
subplot(222);imshow(img(:,:,1));title('R分量')
subplot(223);imshow(img(:,:,2));title('G分量')
subplot(224);imshow(img(:,:,3));title('B分量')

r = img(:,:,1);
g = img(:,:,2);
b = img(:,:,3);

[r_x,r_y] = find(r==255);
[g_x,g_y] = find(g==255);
[b_x,b_y] = find(b==255);

figure(4);
imshow(img(r_y,r_x,1))

img_hsv = rgb2hsv(img);

figure(2);
subplot(131);
imshow(img_hsv(:,:,1),[]);
subplot(132);
imshow(img_hsv(:,:,2),[]);
subplot(133);
imshow(img_hsv(:,:,3),[]);

h = img_hsv(:,:,1); %全部是归一化到(0,1)的值
s = img_hsv(:,:,2);
i = img_hsv(:,:,3);

img_rgb = hsv2rgb(img_hsv);
figure(3);
imshow(img_rgb)
