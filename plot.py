import numpy as np 
import matplotlib.pyplot as plt 


X=np.loadtxt('round.txt')
Y1=np.loadtxt('Analysis1.txt')
Y2=np.loadtxt('Analysis2.txt')
Y3=np.loadtxt('Analysis3.txt')
D1=np.loadtxt('change2.txt')
#D2=np.loadtxt('change2.txt')
#D3=np.loadtxt('change3.txt')
H1=np.loadtxt('diff2.txt')
#H2=np.loadetxt('diff2.txt')
#H3=np.loadetxt('diff3.txt')
#print(H1)
#X=[1,X]

plt.style.use('ggplot')
fig=plt.figure()
plt.axis('off')
plt.title('Analysis for total bits difference')
#plt.xlabel('iteration')
#plt.ylabel('number of different bits')

ax=fig.add_subplot(211)
#ax.semilogy(X,Y1)
#ax.semilogy(X,Y2)
#ax.semilogy(X,Y3)
ax.plot(X,Y1,color='b',linewidth=1,linestyle='-',marker='o')
ax.plot(X,Y2,color='r',linewidth=1.5,linestyle='--',marker='x')
ax.plot(X,Y3,color='g',linewidth=1.5,linestyle=':',marker='*')
#plt.show()
#plt.savefig('Analysis.png')

X1=np.arange(1,65,1)
#print(X1)

bx=fig.add_subplot(212)
bx.scatter(X1,((H1[1-1]+1)%1)*1,color='b',marker='o',label='1')
bx.scatter(X1,((H1[2-1]+2)%2)*2,color='r',marker='o',label='2')
bx.scatter(X1,((H1[3-1]+3)%3)*3,color='b',marker='o',label='3')
bx.scatter(X1,((H1[4-1]+4)%4)*4,color='r',marker='o',label='4')
bx.scatter(X1,((H1[5-1]+5)%5)*5,color='b',marker='o',label='5')
bx.scatter(X1,((H1[6-1]+6)%6)*6,color='r',marker='o',label='6')
bx.scatter(X1,((H1[7-1]+7)%7)*7,color='b',marker='o',label='7')
bx.scatter(X1,((H1[8-1]+8)%8)*8,color='r',marker='o',label='8')
bx.scatter(X1,((H1[9-1]+9)%9)*9,color='b',marker='o',label='9')
bx.scatter(X1,((H1[10-1]+10)%10)*10,color='r',marker='o',label='10')
bx.scatter(X1,((H1[11-1]+11)%11)*11,color='b',marker='o',label='11')
bx.scatter(X1,((H1[12-1]+12)%12)*12,color='r',marker='o',label='12')
bx.scatter(X1,((H1[13-1]+13)%13)*13,color='b',marker='o',label='13')
bx.scatter(X1,((H1[14-1]+14)%14)*14,color='r',marker='o',label='14')
bx.scatter(X1,((H1[15-1]+15)%15)*15,color='b',marker='o',label='15')
bx.scatter(X1,((H1[16-1]+16)%16)*16,color='r',marker='o',label='16')

plt.show()

