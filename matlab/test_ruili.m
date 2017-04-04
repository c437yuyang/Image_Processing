%瑞利噪声本身并不明显与原图相比
clc,clear,close all  
warning off  
feature jit off  
im = imread('lena.jpg');  
Z0 = imnoise_rayleigh(size(im,1),size(im,2),5,5);  
Z1 = uint8(Z0);   % 类型转换
figure('color',[1,1,1]),  
im(:,:,1) = im(:,:,1) + Z1;  % R  
im(:,:,2) = im(:,:,2) + Z1;  % G  
im(:,:,3) = im(:,:,3) + Z1;  % B  
figure(1); imshow(im);title('加瑞利分布噪声图像')  
figure(2); imhist(Z1); title('加瑞利分布噪声图像直方图')  

%因为不明显，顾使用一张纯色图像来获取噪声图像
I = zeros(128,128);
figure(3);imshow(uint8(I),[]);title('原图');
ZI = imnoise_rayleigh(size(I,1),size(I,2),5,8);
I = I + ZI; 
figure(4);imshow(I,[]);title('添加噪声后图像');%调整对比度方便观察

%因为不明显，顾使用一张纯色图像来获取噪声图像
I1 = zeros(128,128);
figure(5);imshow(uint8(I1),[]);title('原图');
ZIexp = imnoise_exp(size(I,1),size(I,2),5);
I1 = I1 + ZIexp; 
figure(6);imshow(I1,[]);title('添加噪声后图像');%调整对比度方便观察
figure(7); imhist(ZIexp); title('指数分布噪声图像直方图')