function R = imnoise_exp(M, N, a)  
% input:  
%       ����rayleigh�ֲ������������ͣ�  
%       M,N���������ͼ�����Ĵ�С  
%       a,b�����������ķֲ�����  
% output:  
%       R: ���������ͼ�������������Ϊdouble��  
   % ���������ֲ�����  
%    R = a + (-b*log(1 - rand(M, N))).^0.5;  
   R = -1/a.*log(1-rand(M, N));
end 