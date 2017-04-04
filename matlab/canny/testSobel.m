clear all;
close all;
clc;

img=imread('..\lena.jpg');
img = rgb2gray(img);
figure(1);title('原图');imshow(img);

img = double(img)
w=fspecial('sobel');

img_w=imfilter(img,w,'replicate');      %求横边缘
w=w';
img_h=imfilter(img,w,'replicate');      %求竖边缘
img=sqrt(img_w.^2+img_h.^2);        %注意这里不是简单的求平均，而是平方和在开方。我曾经好长一段时间都搞错了
figure(2);imshow(uint8(img));title('横+竖边缘');
figure(3);imshow(uint8(img_w));title('横边缘');
figure(4);imshow(uint8(img_h));title('竖边缘');

w = w';
w = -w;
img_w=imfilter(img,w,'replicate');      %求横边缘
img_w = saturateCast(img_w);
w=w';
img_h=imfilter(img,w,'replicate');      %求竖边缘
img=sqrt(img_w.^2+img_h.^2);        %注意这里不是简单的求平均，而是平方和在开方。我曾经好长一段时间都搞错了
% figure(5);imshow(uint8(img));title('横+竖边缘');
% figure(6);imshow(uint8(img_w));title('横边缘');
% figure(7);imshow(uint8(img_h));title('竖边缘');