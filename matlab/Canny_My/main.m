clear all;
close all;
clc;

img=imread('..\lena.jpg');
img = rgb2gray(img);
% figure(1);title('原图');
% imshow(img);
[m n]=size(img);
img=double(img);

%%高斯滤波
w=fspecial('gaussian',[3 3]);
img=imfilter(img,w,'replicate');
% figure(2);
% imshow(uint8(img));title('gaussian filtered');

%%sobel边缘检测
w=fspecial('sobel');
img_w=imfilter(img,w,'replicate');      %求横边缘
w=w';
img_h=imfilter(img,w,'replicate');      %求竖边缘
img=sqrt(img_w.^2+img_h.^2);        %注意这里不是简单的求平均，而是平方和在开方。我曾经好长一段时间都搞错了
figure(3);
subplot(131);imshow(uint8(img));title('横+竖边缘');
subplot(132);imshow(uint8(img_w));title('横边缘');
subplot(133);imshow(uint8(img_h));title('竖边缘');

angle = zeros(m,n);
magnitude = zeros(m,n);

%求出幅值和相位
for i=1:m
    for j=1:n
        Mx=img_w(i,j);
        My=img_h(i,j);
        if(Mx == 0)
            if(My > 0)
              angle(i,j) = 90;
            end
            if(My < 0)
               angle(i,j) = -90; 
            end
        elseif(My == 0)
            angle(i,j) = 0;
        else angle(i,j) = atan(My/Mx) * 180 / pi;
        end
        angle(i,j) = angle(i,j) + 90;
        magnitude(i,j) = sqrt(Mx.^2 + My.^2);
    end
end

figure(4);
imshow(magnitude,[]);

new_edge=zeros(m,n);
for i=2:m-1
    for j=2:n-1
        m0 = magnitude(i,j);
        if (angle(i,j)>=0 && angle(i,j)<22.5)
            m1 = magnitude(i,j-1);
            m2 = magnitude(i,j+1);
            if (m0<m1 || m0< m2)
                magnitude(i,j) = 0;
            end
        end
        if (angle(i,j)>=22.5 && angle(i,j)<67.5)
            m1 = magnitude(i-1,j+1);
            m2 = magnitude(i+1,j-1);
            if (m0<m1 || m0< m2)
                magnitude(i,j) = 0;
            end
        end
        if (angle(i,j)>=67.5 && angle(i,j)<112.5)
            m1 = magnitude(i+1,j);
            m2 = magnitude(i-1,j);
            if (m0<m1 || m0< m2)
                magnitude(i,j) = 0;
            end
        end 
        if (angle(i,j)>=112.5 && angle(i,j)<157.5)
            m1 = magnitude(i-1,j-1);
            m2 = magnitude(i+1,j+1);
            if (m0<m1 || m0< m2)
                magnitude(i,j) = 0;
            end
        end
        if (angle(i,j)>=157.5)
            m1 = magnitude(i,j+1);
            m2 = magnitude(i,j-1);
            if (m0<m1 || m0< m2)
                magnitude(i,j) = 0;
            end
        end 
    end    
end
figure(5);
imshow(magnitude,[]);title('非极大抑制后边缘');

thresh_low = 40;
thresh_high = 80;

mag_low = (magnitude>=thresh_low);
mag_high = (magnitude>=thresh_high);

figure(6);
imshow(mag_low,[]);
figure(7);;imshow(mag_high,[]);
flag_high = zeros(m,n);
flag_low = zeros(m,n); %标记数组

set(0,'RecursionLimit',2000);  %设置最大递归深度

loopCount = 0;

for i=1:m
    for j=1:n
        if mag_high(i,j)~=0
            linkEdge(mag_low,mag_high,flag_low,flag_high,i,j,m,n,loopCount);
        end
    end
end

figure(8);
imshow(mag_high,[]);
