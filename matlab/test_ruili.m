%��������������������ԭͼ���
clc,clear,close all  
warning off  
feature jit off  
im = imread('lena.jpg');  
Z0 = imnoise_rayleigh(size(im,1),size(im,2),5,5);  
Z1 = uint8(Z0);   % ����ת��
figure('color',[1,1,1]),  
im(:,:,1) = im(:,:,1) + Z1;  % R  
im(:,:,2) = im(:,:,2) + Z1;  % G  
im(:,:,3) = im(:,:,3) + Z1;  % B  
figure(1); imshow(im);title('�������ֲ�����ͼ��')  
figure(2); imhist(Z1); title('�������ֲ�����ͼ��ֱ��ͼ')  

%��Ϊ�����ԣ���ʹ��һ�Ŵ�ɫͼ������ȡ����ͼ��
I = zeros(128,128);
figure(3);imshow(uint8(I),[]);title('ԭͼ');
ZI = imnoise_rayleigh(size(I,1),size(I,2),5,8);
I = I + ZI; 
figure(4);imshow(I,[]);title('���������ͼ��');%�����Աȶȷ���۲�

%��Ϊ�����ԣ���ʹ��һ�Ŵ�ɫͼ������ȡ����ͼ��
I1 = zeros(128,128);
figure(5);imshow(uint8(I1),[]);title('ԭͼ');
ZIexp = imnoise_exp(size(I,1),size(I,2),5);
I1 = I1 + ZIexp; 
figure(6);imshow(I1,[]);title('���������ͼ��');%�����Աȶȷ���۲�
figure(7); imhist(ZIexp); title('ָ���ֲ�����ͼ��ֱ��ͼ')