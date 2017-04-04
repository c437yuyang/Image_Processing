I = imread('lena.jpg');
figure(1);
imshow(I);
PSF = fspecial('motion',20,15);
J = imfilter(I,PSF,'conv','circular');
figure(2),imshow(J);