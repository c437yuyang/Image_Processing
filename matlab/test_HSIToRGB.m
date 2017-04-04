clc
clear all
I = imread('Airplane.jpg');
% I = zeros(20,20);
% I(6:15,6:15) = 255;
% I = cat(3,I,I,I);

subplot(221);
imshow(I);title('ԭͼ');
IHsi = rgb2hsi(I);
subplot(222);
imshow(IHsi,[]);title('HSI');
IRgb = hsi2rgb(IHsi);
subplot(223);
imshow(IRgb,[]);title('HSI��ԭ��RGB');
sub = im2double(I)-im2double(IRgb);
subplot(224);
imshow(sub,[]);title('��ԭ�ص�RGB��ԭͼ���');