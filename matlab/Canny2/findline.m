function [ff,flag1]=findline(k3,k4,flag,m,n)

flag1=flag;
  m1=m+1;n1=n+1;
  while(m~=m1||n~=n1)%��m��n���������仯������line�ѵ��յ�
  flagg=0;
  for i=1:3
      if(flagg==1) break;          
      end
      for j=1:3
          if k3(m-2+i,n-2+j)~=0
              k4(m-2+i,n-2+j)=255;
              m1=m-2+i;n1=n-2+j;%�µ�[m,n]��
              flag1(m,n)=1;%����Ѽ���
              flagg=1;break;
      end
      end
  end
    m=m1;n=n1;
  end
  ff=k4;
  
  