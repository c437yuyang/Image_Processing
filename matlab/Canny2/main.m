clear;
clc;
i=imread('lena.jpg');
k=rgb2y(i);%获取h分量，即亮度分量

k1=gaosi(k);%横向滤波
k1=k1';%对图像进行转置，为下一步纵向滤波作准备（纵向滤波==转置后横向滤波）
k1=gaosi(k1);
k1=k1';%还原


%计算梯度的大小和方向
[h,w]=size(k);
for m=2:h-1
    for n=2:w-1
        zz1=k1(m,n-1)+k1(m+1,n-1);
        zz2=k1(m,n)+k1(m+1,n);
        zz3=k1(m,n-1)+k1(m,n);
        zz4=k1(m+1,n-1)+k1(m+1,n);
        kp(m,n)=0.5*(zz2-zz1);
        kq(m,n)=0.5*(zz3-zz4);
        kfu(m,n)=sqrt((kp(m,n)^2)+(kq(m,n)^2));%梯度大小
        kjiao(m,n)=atan(kq(m,n)/(kp(m,n)+0.001));%梯度方向,0.001防止分母为0
    end
end


%非极大值抑制
%首先将梯度方向划分为4个方向0,45,90,135（以及他们的反向延长线）
for m=2:h-1
    for n=2:w-1
        if kjiao(m,n)>=3/8*pi
            kjiao(m,n)=2;
        else if kjiao(m,n)>=1/8*pi
            kjiao(m,n)=1;
        else if  kjiao(m,n)>=-1/8*pi
             kjiao(m,n)=0;
        else if  kjiao(m,n)>=-3/8*pi
             kjiao(m,n)=3;
            else
                kjiao(m,n)=2;
            end
            end
            end
        end
    end
end


%按照各个方向分别判断
k2=k1;
for m=2:h-1
    for n=2:w-1
       if kjiao(m,n)==0
           if k1(m,n)>k1(m,n-1)&&k1(m,n)>k1(m,n+1);
           else k2(m,n)=0;
           end
       end
       if kjiao(m,n)==1
           if k1(m,n)>k1(m+1,n-1)&&k1(m,n)>k1(m-1,n+1);
           else k2(m,n)=0;
           end
       end
       if kjiao(m,n)==2
           if k1(m,n)>k1(m-1,n)&&k1(m,n)>k1(m+1,n);
           else k2(m,n)=0;
           end
       end
       if kjiao(m,n)==3
           if k1(m,n)>k1(m-1,n-1)&&k1(m,n)>k1(m+1,n+1);
           else k2(m,n)=0;
           end
       end
    end
end

%两次阈值分割
k3=k2;%以t1为阈值分割后的矩阵
k4=k2;%以t2为阈值分割后的矩阵
t1=50;
t2=2*t1;
for m=2:h-1
    for n=2:w-1
       if kfu(m,n)<t1
           k3(m,n)=0;
       end
       if kfu(m,n)<t2
           k4(m,n)=0;
       end
    end
end


figure;
subplot(221);imshow(i);title('原图像');
subplot(222);imshow(k3,[]);title('阈值为50的分割图像');
subplot(223);imshow(k4,[]);title('阈值为100的分割图像');
flag=zeros(h,w);%标记该点是否以检测过，1表示检测过
for m=2:h-1
    for n=2:w-1
        if k4(m,n)~=0&&flag(m,n)==0
            [k4,flag]=findline(k3,k4,flag,m,n);
        end
    end
end
subplot(224);imshow(k4,[]);title('修正后的分割图像');