function dctVector = zigzagCoder(A, needNumCoefficient)
%// Select elements of a square matrix using zig-zag pattern
%// Author : zouxy  
%// Date   : 2013-10-28  
%// HomePage : http://blog.csdn.net/zouxy09  
%// Email  : zouxy09@qq.com

%% input: A : square matrix 
%%        needNumCoefficient : how many coefficient you want to maintain
%% output: dctVector: vector contain coefficients by col

	if size(A, 1) ~= size(A, 2)
		display('Warning: your matrix should be square!');
		if size(A, 1) > size(A, 2)
			A = A(1:size(A, 2), 1:size(A, 2));
		else
			A = A(1:size(A, 1), 1:size(A, 1));
		end
	end
	
	if needNumCoefficient > (size(A, 1) * size(A, 2))
		needNumCoefficient = size(A, 1) * size(A, 2);
	end
	
	A = A';
	count = 1;    
	for dim_sum = 2 : (size(A, 1) + size(A, 2))
		if mod(dim_sum, 2) == 0
			for i = 1 : size(A, 1)
				if dim_sum - i <= size(A, 1) & dim_sum - i > 0
					dctVector(count) = A(i, dim_sum - i);
					count = count + 1;
				end
			end
		else
			for i = 1 : size(A, 1)
				if dim_sum - i <= size(A, 1) & dim_sum - i >0
					dctVector(count) = A(dim_sum - i, i);
					count = count + 1;
				end                   
			end   
		end
	end
	dctVector = dctVector(1:needNumCoefficient);
	dctVector = dctVector';
	
end