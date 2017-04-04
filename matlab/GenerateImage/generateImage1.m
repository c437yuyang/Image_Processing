clc;clear all;
I = ones(256,256).*80;
I = uint8(I);
subplot(131);imshow(I);

% I = im2double(I);
% I = uint8(I);
I(41:216,41:216) = 160;
subplot(132);imshow(I);

I(81:176,81:176) = 240;
subplot(133);imshow(I);

figure(2);
imshow(I);