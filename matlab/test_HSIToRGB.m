clc
clear all
I = imread('Airplane.jpg');
% I = zeros(20,20);
% I(6:15,6:15) = 255;
% I = cat(3,I,I,I);

subplot(221);
imshow(I);title('原图');
IHsi = rgb2hsi(I);
subplot(222);
imshow(IHsi,[]);title('HSI');
IRgb = hsi2rgb(IHsi);
subplot(223);
imshow(IRgb,[]);title('HSI还原回RGB');
sub = im2double(I)-im2double(IRgb);
subplot(224);
imshow(sub,[]);title('还原回的RGB和原图差距');