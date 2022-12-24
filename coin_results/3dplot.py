import pandas as pd
import numpy as np
from matplotlib import cm
import matplotlib.pyplot as plt

df = pd.read_csv('C-Coin.txt',sep=" "*10,skiprows=4,header=None)
print(df)

prob = float(input("Fixed Prob:"))
diff = int(input("Fixed diff:"))
print(prob,diff)

df_prob = df[df[1]==prob].copy()
df_diff = df[df[2]==diff].copy()

plt.tight_layout()
plt.grid()
plt.xlabel("Valor de diferencia")
plt.ylabel("Valor Esperado")
plt.title("Probabilidad fija a {} y Valor Esperado".format(prob))
ticks = [i for i in range(0,len(df_prob[3]))]
plt.xticks(ticks,labels=df_prob[2])
plt.plot(ticks,df_prob[3])
plt.show()

plt.tight_layout()
plt.grid()
ticks = [i for i in range(0,len(df_prob[4]))]
plt.xticks(ticks,labels=df_prob[2])
plt.title("Probabilidad fija a {} y Media Tiradas".format(prob))
plt.xlabel("Valor de diferencia")
plt.ylabel("Media Tiradas")
plt.plot(ticks,df_prob[4])
plt.show()

plt.title("Diferencia fija a {} y Valor Esperado".format(diff))
plt.xlabel("Valor de probabilidad")
plt.ylabel("Valor Esperado")

ticks = [i for i in range(0,len(df_diff[3]))]
plt.xticks(ticks,labels=df_diff[1])
plt.tight_layout()
plt.grid()
plt.plot([i for i in range(0,len(df_diff[3]))],df_diff[3])
plt.show()
plt.title("Diferencia fija a {} y Media Tiradas".format(diff))
ticks = [i for i in range(0,len(df_diff[4]))]
plt.xticks(ticks,labels=df_diff[1])
plt.xlabel("Valor de probabilidad")
plt.ylabel("Media Tiradas")
plt.tight_layout()
plt.grid()
plt.plot([i for i in range(0,len(df_diff[4]))],df_diff[4])
plt.show()

fig, ax = plt.subplots(subplot_kw={"projection":"3d"})
surf = ax.plot_trisurf(df[1],df[2],df[3],cmap=cm.RdBu,linewidth=0,antialiased=False)
ax.set_title("Probabilidad, Diferencia y Valor Esperado")
ax.view_init(20,45)
plt.show()
fig, ax = plt.subplots(subplot_kw={"projection":"3d"})
surf = ax.plot_trisurf(df[1],df[2],df[4],cmap=cm.coolwarm,linewidth=0,antialiased=False)
ax.set_title("Probabilidad, Diferencia y Media Tiradas")
ax.view_init(20,45)
plt.show()
