clear all;
close all;
clc;

img=imread('1.jpg');
img = imresize(img,0.3);
img = rgb2gray(img);
[m n]=size(img);
img=double(img);

[mag_w,mag_h] = calcGradient(img);
figure(1);
subplot(121);imshow(mag_w,[]);title('ˮƽ�����ݶ�');
subplot(122);imshow(mag_h,[]);title('��ֱ�����ݶ�');

angle = zeros(m,n);
magnitude = zeros(m,n);

k = 3;%�ݶ�Խ��ϵ��ԽС
w = exp(-(mag_w.^2 + mag_h.^2)./(2*k.^2));
figure(2);
imshow(w,[]);title('��ʼģ��');

count = 10;
for c=1:count
    for i=2:m-1
        for j=2:n-1   
            num1 = 0;
            num2 = 0;
            for i1 = -1:1
                for j1=-1:1
                    num1 = num1 + img(i+i1,j+j1)*w(i+i1,j+j1);
                    num2 = num2 + w(i+i1,j+j1);
                end
            end
            img(i,j) = num1/num2;
        end
    end 
    [mag_w,mag_h] = calcGradient(img);
    w = exp(-(mag_w.^2 + mag_h.^2)./(2*k.^2));
end
imwrite(img,'after.jpg')
figure(3);
imshow(img,[]);title('����Ӧƽ���˲���ͼ��')
img = medfilt2(img,[13,13]);

figure(7);
imshow(img,[]);title('��ֵ�˲���')
figure(4);
imshow(w,[]);title('����ģ��');

BW1 = edge(img,'sobel');
BW2 = edge(img,'canny');
figure(5);imshow(BW1);title('����Ӧƽ���˲���ͼ���sobel')
figure(6);imshow(BW2);title('����Ӧƽ���˲���ͼ���canny')