close all;  
clear all;  
clc;  
  
f = imread('测试图_三个灰度级_2.bmp');  
f = double(f);
f = mat2gray(f,[0 255]);  
[M,N] = size(f);  
  
figure();  
subplot(1,2,1);  
imshow(f,[0 1]);  
xlabel('a).Original image');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(f,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
%% ---------------gaussian-------------------  
a = 0;  
b = 0.08;  
n_gaussian = a + b .* randn(M,N);  
  
g_gaussian = f + n_gaussian;   
  
figure();  
subplot(1,2,1);  
imshow(g_gaussian,[0 1]);  
xlabel('a).Ruselt of Gaussian noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_gaussian,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
  
%% ---------------rayleigh-------------------  
a = -0.2;  
b = 0.03;  
n_rayleigh = a + (-b .* log(1 - rand(M,N))).^0.5;  
  
g_rayleigh = f + n_rayleigh;   
  
figure();  
subplot(1,2,1);  
imshow(g_rayleigh,[0 1]);  
xlabel('a).Ruselt of Rayleigh noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_rayleigh,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
%% ---------------Erlang-------------------  
a = 25;  
b = 3;  
n_Erlang = zeros(M,N);   
  
for j=1:b  
    n_Erlang = n_Erlang + (-1/a)*log(1 - rand(M,N));  
end  
  
g_Erlang = f + n_Erlang;   
  
figure();  
subplot(1,2,1);  
imshow(g_Erlang,[0 1]);  
xlabel('a).Ruselt of Erlang noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_Erlang,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
  
%% ---------------Exponential-------------------  
a = 9;  
n_Ex = (-1/a)*log(1 - rand(M,N));   
  
g_Ex = f + n_Ex;  
  
figure();  
subplot(1,2,1);  
imshow(g_Ex,[0 1]);  
xlabel('a).Ruselt of Exponential noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_Ex,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
  
%% ---------------Uniform-------------------  
a = 0;  
b = 0.3;  
n_Uniform = a + (b-a)*rand(M,N);  
  
g_Uniform = f + n_Uniform;  
  
figure();  
subplot(1,2,1);  
imshow(g_Uniform,[0 1]);  
xlabel('a).Ruselt of Uniform noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_Uniform,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.014]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  
  
%% ---------------Salt & pepper-------------------  
a = 0.05;  
b = 0.05;  
x = rand(M,N);  
  
g_sp = zeros(M,N);  
g_sp = f;  
  
g_sp(find(x<=a)) = 0;  
g_sp(find(x > a & x<(a+b))) = 1;  
  
figure();  
subplot(1,2,1);  
imshow(g_sp,[0 1]);  
xlabel('a).Ruselt of Salt & pepper noise');  
  
subplot(1,2,2);  
x = linspace(-0.2,1.2,358);  
h = hist(g_sp,x)/(M*N);  
Histogram = zeros(358,1);  
for y = 1:256  
    Histogram = Histogram + h(:,y);  
end  
bar(-0.2:1/255:1.2,Histogram);  
axis([-0.2 1.2 0 0.3]),grid;  
xlabel('b).The Histogram of a');  
ylabel('Number of pixels');  