clear;
clear all;
I = imread('../lena.jpg');

I = rgb2gray(I);
I = imresize(I,0.1);

[M,N] = size(I);
I1 = I(:);
P = zeros(1,256);
%��ȡ�����ŵĸ��ʣ�
for i = 0:255
    P(i+1) = length(find(I1 == i))/(M*N);
end

 k = 0:255;
 dict = huffmandict(k,P); %�����ֵ�
 enco = huffmanenco(I1,dict); %����
 deco = huffmandeco(enco,dict); %����
 Ide = col2im(deco,[M,N],[M,N],'distinct'); %����������ת����ͼ��飻

 subplot(1,2,1);imshow(I);title('original image');
 subplot(1,2,2);imshow(uint8(Ide));title('deco image');