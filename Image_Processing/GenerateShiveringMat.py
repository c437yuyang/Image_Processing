# coding:UTF-8
import numpy as np

Mn = np.array(([0, 2], [3, 1]))
print Mn.shape
Un = np.ones_like(Mn)
print Un

for i in xrange(3):
    Mn1 = np.hstack((4 * Mn, 4 * Mn + 2 * Un))
    Mn2 = np.hstack((4 * Mn + 3 * Un, 4 * Mn + Un))
    Mn = np.vstack((Mn1, Mn2))
    Un = np.ones_like(Mn)
    print Mn.shape

print Mn

with open('shiveringMat.txt','w') as f:
    for i in xrange(Mn.shape[0]):
        for j in xrange(Mn.shape[1]):
            f.write(str(Mn[i][j]) + '\t')
        f.write('\n')