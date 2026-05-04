import numpy as np
import matplotlib.pyplot as plt
from numba import cuda
import time

@cuda.jit
def mandel(min_x,max_x,min_y,max_y,img,it):
    h,w=img.shape
    x,y=cuda.grid(2)
    if x<w and y<h:
        r=min_x+x*(max_x-min_x)/w
        im=min_y+y*(max_y-min_y)/h
        zr=zi=0.0; c=0
        while zr*zr+zi*zi<=4.0 and c<it:
            t=zr*zr-zi*zi+r
            zi=2.0*zr*zi+im
            zr=t; c+=1
        img[y,x]=c

def cpu(min_x,max_x,min_y,max_y,w,h,it):
    img=np.zeros((h,w),np.uint8)
    for y in range(h):
        for x in range(w):
            r=min_x+x*(max_x-min_x)/w
            im=min_y+y*(max_y-min_y)/h
            zr=zi=0.0; c=0
            while zr*zr+zi*zi<=4.0 and c<it:
                t=zr*zr-zi*zi+r
                zi=2.0*zr*zi+im
                zr=t; c+=1
            img[y,x]=c
    return img

w=h=800; it=100
min_x,max_x=-2.0,1.0
min_y,max_y=-1.5,1.5

s=time.time()
cpu_img=cpu(min_x,max_x,min_y,max_y,w,h,it)
print("CPU Time:",time.time()-s)

img=np.zeros((h,w),np.uint8)
d=cuda.to_device(img)

tpb=(16,16)
bpg=(int(np.ceil(w/tpb[0])),int(np.ceil(h/tpb[1])))

s=time.time()
mandel[bpg,tpb](min_x,max_x,min_y,max_y,d,it)
cuda.synchronize()
print("GPU Time:",time.time()-s)

res=d.copy_to_host()

plt.imshow(res,cmap='hot')
plt.title("Mandelbrot Set (CUDA Numba)")
plt.colorbar()
plt.show()
