
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

%�ҵ����һ�����ָ��ʲ�Ϊ0�ĻҶȼ�
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
%f=hist(st+1:nd+1);  %f��ÿ���Ҷȳ��ֵĸ���   
p=st;   q=nd-st;   
u=0;   
for i=1:q   
    u=u+f(i)*(p+i-1);  %u�����ص�ƽ��ֵ    
    ua(i)=u;           %ua��i����ǰi�����ص�ƽ���Ҷ�ֵ   
end;   
   
for i=1:q   
    w(i)=sum(f(1:i));  %w��i����ǰi�����ص��ۼӸ���   
end;   
  
w=w+0.0001;  
   
d=(u*w-ua).^2./(w.*(1-w));   
[y,tp]=max(d);  %����ȡ����������ֵ��ȡ���ֵ�ĵ�   
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