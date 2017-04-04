%���Һ���ͼ�� �� ���Ӿ��Ȱ�������ͼ��
t=0:0.01:10;
f=sin(t);
y1=rand(size(f));
y2=y1+f;
subplot(311);
plot(t,f),title('����'),grid on; 
subplot(312);
plot(t,y1),title('���Ȱ�����'),grid on;
subplot(313);
plot(t,y2,'g'),title('����'),grid on;


figure
t=0:0.01:10;
f=sin(t);
y1=0.3*randn(size(f));
y2=y1+f;
subplot(311);
plot(t,f),title('����'),grid on; 
subplot(312);
plot(t,y1),title('��˹������'),grid on;
subplot(313);
plot(t,y2,'g'),title('����'),grid on;


Fs = 1000;                        %ȡ��Ƶ��
nfft=1024;                         %1024��DFT
p = 0:round(nfft/2-1);
k =p*Fs/nfft;
t = 0:1/Fs:1;

A = rand(1,1001);
[cor1 lag1] = xcorr(A,'unbiased');   % lag���źŵ���ʱ���߳�ǰ
figure(1);
subplot(211),plot(lag1/Fs,cor1),title('���Ȱ���������غ���');
fc1 = fft(cor1,nfft);
cm = abs(fc1);
subplot(212),plot(k,10*log10(cm(p+1))),title('���Ȱ������������ܶ�');

B = randn(1,1001);
[cor2 lag2] = xcorr(B,'unbiased');
figure(2);
subplot(211),plot(lag2/Fs,cor2),title('��˹����������غ���');
fc2 = fft(cor2,nfft);
cm2 = abs(fc2);
subplot(212),plot(k,10*log10(cm2(p+1))),title('��˹�������������ܶ�');