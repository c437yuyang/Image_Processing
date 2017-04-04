clear;
clc;
i=imread('lena.jpg');
k=rgb2y(i);%��ȡh�����������ȷ���

k1=gaosi(k);%�����˲�
k1=k1';%��ͼ�����ת�ã�Ϊ��һ�������˲���׼���������˲�==ת�ú�����˲���
k1=gaosi(k1);
k1=k1';%��ԭ


%�����ݶȵĴ�С�ͷ���
[h,w]=size(k);
for m=2:h-1
    for n=2:w-1
        zz1=k1(m,n-1)+k1(m+1,n-1);
        zz2=k1(m,n)+k1(m+1,n);
        zz3=k1(m,n-1)+k1(m,n);
        zz4=k1(m+1,n-1)+k1(m+1,n);
        kp(m,n)=0.5*(zz2-zz1);
        kq(m,n)=0.5*(zz3-zz4);
        kfu(m,n)=sqrt((kp(m,n)^2)+(kq(m,n)^2));%�ݶȴ�С
        kjiao(m,n)=atan(kq(m,n)/(kp(m,n)+0.001));%�ݶȷ���,0.001��ֹ��ĸΪ0
    end
end


%�Ǽ���ֵ����
%���Ƚ��ݶȷ��򻮷�Ϊ4������0,45,90,135���Լ����ǵķ����ӳ��ߣ�
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


%���ո�������ֱ��ж�
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

%������ֵ�ָ�
k3=k2;%��t1Ϊ��ֵ�ָ��ľ���
k4=k2;%��t2Ϊ��ֵ�ָ��ľ���
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
subplot(221);imshow(i);title('ԭͼ��');
subplot(222);imshow(k3,[]);title('��ֵΪ50�ķָ�ͼ��');
subplot(223);imshow(k4,[]);title('��ֵΪ100�ķָ�ͼ��');
flag=zeros(h,w);%��Ǹõ��Ƿ��Լ�����1��ʾ����
for m=2:h-1
    for n=2:w-1
        if k4(m,n)~=0&&flag(m,n)==0
            [k4,flag]=findline(k3,k4,flag,m,n);
        end
    end
end
subplot(224);imshow(k4,[]);title('������ķָ�ͼ��');