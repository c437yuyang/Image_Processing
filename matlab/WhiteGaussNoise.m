%正弦函数图形 及 叠加均匀白噪声的图形
t=0:0.01:10;
f=sin(t);
y1=rand(size(f));
y2=y1+f;
subplot(311);
plot(t,f),title('正弦'),grid on; 
subplot(312);
plot(t,y1),title('均匀白噪声'),grid on;
subplot(313);
plot(t,y2,'g'),title('叠加'),grid on;


figure
t=0:0.01:10;
f=sin(t);
y1=0.3*randn(size(f));
y2=y1+f;
subplot(311);
plot(t,f),title('正弦'),grid on; 
subplot(312);
plot(t,y1),title('高斯白噪声'),grid on;
subplot(313);
plot(t,y2,'g'),title('叠加'),grid on;


Fs = 1000;                        %取样频率
nfft=1024;                         %1024点DFT
p = 0:round(nfft/2-1);
k =p*Fs/nfft;
t = 0:1/Fs:1;

A = rand(1,1001);
[cor1 lag1] = xcorr(A,'unbiased');   % lag是信号的延时或者超前
figure(1);
subplot(211),plot(lag1/Fs,cor1),title('均匀白噪声自相关函数');
fc1 = fft(cor1,nfft);
cm = abs(fc1);
subplot(212),plot(k,10*log10(cm(p+1))),title('均匀白噪声功率谱密度');

B = randn(1,1001);
[cor2 lag2] = xcorr(B,'unbiased');
figure(2);
subplot(211),plot(lag2/Fs,cor2),title('高斯白噪声自相关函数');
fc2 = fft(cor2,nfft);
cm2 = abs(fc2);
subplot(212),plot(k,10*log10(cm2(p+1))),title('高斯白噪声功率谱密度');