clc
clear all;
I = imread('..\lena.jpg');
I = rgb2gray(I);

[m,n] = size(I);

%全局均值
mg = mean(I(:));

%得到归一化直方图
hist = imhist(I);
hist = hist./(m*n);

%累计概率
histProb = zeros(256,1);
sum=0.0;
for i = 1:256
    if(hist(i)==0)
        continue
    end
    sum=sum+hist(i);
    histProb(i) = sum;
end


%累计均值
mk = zeros(256,1);
sum = 0;
for i=1:256
    if(hist(i)==0)
        continue;
    end
    sum = sum+i*hist(i);
    mk(i) = sum;
end

bar(mk)
%类间方差
sigma = zeros(256,1);
num1 = zeros(256,1);
num2 = zeros(256,1);
for i=1:256
    if(histProb(i)==0) continue;end
    if(histProb(i)>=0.99) break;end
    num1(i) = (mg.*histProb(i)-mk(i)).^2;
    num2(i) = histProb(i).*(1-histProb(i));
    sigma(i) = num1(i) ./ (num2(i) + eps);
end

[maxValue,index] = max(sigma)
imshow(I>=index)