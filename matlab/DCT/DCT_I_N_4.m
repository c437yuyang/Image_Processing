clear;
clc;
X=[
    61    19    50    20
    82    26    61    45
    89    90    82    43
    93    59    53    97] %原始的数据
A=zeros(4);
for i=0:3
    for j=0:3
        if i==0
            a=sqrt(1/4);
        else
            a=sqrt(2/4);
        end            
        A(i+1,j+1)=a*cos(pi*(j+0.5)*i/4); %生成变换矩阵
    end
end
Y=A*X*A'   %DCT变换后的矩阵
X1=A'*Y*A  %DCT反变换恢复的矩阵