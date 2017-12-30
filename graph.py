import numpy as np 
import matplotlib.pyplot as plt 

plt.style.use('ggplot')

values=np.loadtxt('score.txt')
feature=['random','length','uniquet','variety','average']

N=len(values)

angles=np.linspace(0,2*np.pi,N,endpoint=False)

values=np.concatenate((values,[values[0]]))
angles=np.concatenate((angles,[angles[0]]))

def normfun(x,mu, sigma):
    pdf = np.exp(-((x - mu)**2) / (2* sigma**2)) / (sigma * np.sqrt(2*np.pi))
    return pdf

data = np.loadtxt('random.txt')

mean = data.mean()
std = data.std()

X = np.arange(-5,15)
Y = normfun(X,mean,std)

fig=plt.figure()
ax=fig.add_subplot(111,polar=True)
ax.plot(angles,values,'o-',linewidth=2)

ax.fill(angles,values,alpha=0.25)

ax.set_thetagrids(angles*180/np.pi,feature)

ax.set_ylim(0,10)

plt.title('evaluation for the safety of your password')

ax.grid(True)

bx=fig.add_subplot(111)

bx.plot(X,Y,color='r',lw=3)

bx.hist(data,bins=15,color='y',normed=True)
bx.title('score distribution')
bx.xlabel('score')
bx.ylabel('probability')
#plt.plot([8.8,8.8],[0,0.45],linestyle='--',lw=2)

plt.show()

