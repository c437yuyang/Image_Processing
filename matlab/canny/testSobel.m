clear all;
close all;
clc;

img=imread('..\lena.jpg');
img = rgb2gray(img);
figure(1);title('ԭͼ');imshow(img);

img = double(img)
w=fspecial('sobel');

img_w=imfilter(img,w,'replicate');      %����Ե
w=w';
img_h=imfilter(img,w,'replicate');      %������Ե
img=sqrt(img_w.^2+img_h.^2);        %ע�����ﲻ�Ǽ򵥵���ƽ��������ƽ�����ڿ������������ó�һ��ʱ�䶼�����
figure(2);imshow(uint8(img));title('��+����Ե');
figure(3);imshow(uint8(img_w));title('���Ե');
figure(4);imshow(uint8(img_h));title('����Ե');

w = w';
w = -w;
img_w=imfilter(img,w,'replicate');      %����Ե
img_w = saturateCast(img_w);
w=w';
img_h=imfilter(img,w,'replicate');      %������Ե
img=sqrt(img_w.^2+img_h.^2);        %ע�����ﲻ�Ǽ򵥵���ƽ��������ƽ�����ڿ������������ó�һ��ʱ�䶼�����
% figure(5);imshow(uint8(img));title('��+����Ե');
% figure(6);imshow(uint8(img_w));title('���Ե');
% figure(7);imshow(uint8(img_h));title('����Ե');