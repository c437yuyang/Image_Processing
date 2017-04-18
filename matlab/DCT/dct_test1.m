clear;
clc;
 
X=imread('../lena.jpg');
X = rgb2gray(X);

X=double(X);
[a,b]=size(X);
Y=blkproc(X,[8 8],'dct2');
X1=blkproc(Y,[8 8],'idct2');
 
figure
subplot(1,3,1);
imshow(uint8(X));
title('ԭʼͼ');
 
subplot(1,3,2);
imshow(uint8(Y));
title('�ֿ�DCT�任ͼ');
 
subplot(1,3,3);
imshow(uint8(X1));
title('�ֿ�DCT�ָ�ͼ');
 
Y1=dct2(X);
X10=idct2(Y1);
 
figure
subplot(1,3,1);
imshow(uint8(X));
title('ԭʼͼ');
 
subplot(1,3,2);
imshow(uint8(Y1));
title('DCT�任ͼ');
 
subplot(1,3,3);
imshow(uint8(X10));
title('DCT���任�ָ�ͼ');