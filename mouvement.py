# -*- coding: utf-8 -*-
"""
Created on Mon Feb  8 09:10:09 2021

### MOUVEMENT MAIN ###

@author: Farès Mezdour / Lac0scope 2021
"""


#====================== IMPORT =========================


import matplotlib.pyplot as plt
import skimage.color as skco
import numpy as np
import time
import pathlib

import utils


#=================== TRAITEMENT IMAGE ===============================
  
### Redimensionnement et sous-échantillonage des images

im_path=str(pathlib.Path(__file__).parent.parent.absolute())+"/img/mouvement.jpg"

def estimation_mouvement(p1,p2):
    print("python ok")
    #sys.stdout=sys.__stdout__
    img1,img2=utils.ouverture_images(p1,p2)
    
    img2 = skco.rgb2gray(img2[::8,::8])
    img1 = skco.rgb2gray(img1[::8,::8])

    # img1 = img1[:121,:]
    # img2 = img2[:,:66]
    
    # img2 = skco.rgb2gray(img2[::4,::4])
    # img1 = skco.rgb2gray(img1[::8,::8])
    # img1 = img1[:172,:115]
    # img2 = img2[:,:]
     
    ### Affichage du plancton à deux instants différents
    
    # plt.figure()
    # plt.imshow(img1,cmap = 'gray')
    # plt.title("Plancton instant 1")
    # plt.figure()
    # plt.imshow(img2,cmap = 'gray')
    # plt.title("Plancton instant 2")
    
    ### Carte Binaire 
    
    I = abs(img2-img1)
    #plt.figure()
    #plt.imshow(I,cmap='gray')

    I=I>0.07*1
    #plt.figure()
    #plt.imshow(I)
    
    ### Calcul du mouvement par Block-Matching
    
    t=time.perf_counter()

    dx,dy=utils.block_matching(img1, img2, 308, 410, 16)
    t=time.perf_counter()-t

    print("Temps d'exécution block matching sur deux images:",t,"s")
    
    ### Affichage des vecteurs sur la Carte Binaire des Changements
    
    x=np.linspace(308,0,len(dx))
    y=np.linspace(0,410,len(dx[0]))
    plt.figure()
    plt.imshow(img1,cmap="gray")
    plt.quiver(y,x,np.flip(dx,axis=0),np.flip(-dy,axis=0), color='red')
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    plt.title(current_time)
    plt.savefig(im_path)
    # sys.stdout=sys.__stdout__
    return str("done")

# estimation_mouvement("/home/robin/Desktop/segmenter/img/15_06_10_832992.jpg","/home/robin/Desktop/segmenter/img/15_06_12_100663.jpg")