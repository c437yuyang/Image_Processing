clear;
clc;
X=round(rand(8)*100)   %产生随机矩阵
A=zeros(8);
for i=0:7
    for j=0:7
        if i==0
            a=sqrt(1/8);
        else
            a=sqrt(2/8);
        end            
        A(i+1,j+1)=a*cos(pi*(j+0.5)*i/8);
    end
end
Y=A*X*A'        %DCT变换
YY=dct2(X)      %Matlab自带的dct变换