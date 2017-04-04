
x=imread('..\lena.jpg');   
a=rgb2gray(x);   
figure(1);
imshow(a,[]);  
%[hist x]=imhist(a);   
[m,n]=size(a);   
N=m*n;   
L=256;   
     
for i=1:L  
    hist(i)=length(find(a==(i-1)));  
    f(i)=hist(i)/(N);  
end  

%找到最后一个出现概率不为0的灰度级
for i=1:L   
    if hist(i)~=0   
        st=i-1;   
        break;   
    end   
end   

for i=L:-1:1   
    if hist(i)~=0   
        nd=i-1;   
        break;   
    end   
end   
%f=hist(st+1:nd+1);  %f是每个灰度出现的概率   
p=st;   q=nd-st;   
u=0;   
for i=1:q   
    u=u+f(i)*(p+i-1);  %u是像素的平均值    
    ua(i)=u;           %ua（i）是前i个像素的平均灰度值   
end;   
   
for i=1:q   
    w(i)=sum(f(1:i));  %w（i）是前i个像素的累加概率   
end;   
  
w=w+0.0001;  
   
d=(u*w-ua).^2./(w.*(1-w));   
[y,tp]=max(d);  %可以取出数组的最大值及取最大值的点   
th=tp+p;   
  
for i=1:m   
    for j=1:n   
        if a(i,j)>th   
            a(i,j)=0;   
        else   
            a(i,j)=255;   
        end   
    end   
end    
figure(2);
imshow(a,[]);  