#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  8 14:59:48 2021

@author: robin
"""


import os
import sys
import matplotlib.pyplot as plt
import numpy as np
import pickle
import joblib
import pathlib

p=str(pathlib.Path(__file__).parent.absolute())


def save_obj(obj, name ):
    with open(name + '.pkl', 'wb') as f:
        pickle.dump(obj, f, pickle.HIGHEST_PROTOCOL)

def load_obj(name ):
    with open(name + '.pkl', 'rb') as f:
        return pickle.load(f)

def liste(p):
    l=os.listdir(p)
    return str(len(l))+" fichiers trouvés:\n\n"+'\n'.join(l)

def load_model():
    """
    Returns
    -------
    model : model
    """
    model = joblib.load(p+"/random_forest_exported")
    return model

def ouverture_images(p1,p2):
    try:
      img1 = plt.imread(p1)
    except IOError:
      print ('Erreur sur ouverture du fichier ' + p1)
      sys.exit(1)
    try:
      img2 = plt.imread(p2)
    
    except IOError:
      print ('Erreur sur ouverture du fichier ' + p2)
      sys.exit(1)

    return img1,img2

def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 50, fill = '█', printEnd = "\r"):
    
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
        printEnd    - Optional  : end character (e.g. "\r", "\r\n") (Str)
    """
    
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end = printEnd)
    # Print New Line on Complete
    if iteration == total: 
        print()


def block_matching(I,J,x,y,b_size):
    dim_fen=20
    d=dim_fen-b_size
    d2=int(d/2)
    dx=[]
    dy=[]
    for i in range(d2,x-d2-b_size,b_size):    #coords de I
        d1x=[]
        d1y=[]
        for j in range(d2,y-d2-b_size,b_size):
            d3=[]
            for k in range(i-d2,i+d2):    #coords de J
                d4=[]
                for l in range(j-d2,j+d2):
                    somme=0
                    for m in range(b_size):
                        for n in range(b_size):
                            somme+=np.square(J[k+m][l+n]-I[i+m][j+n])
                    d4+=[somme]
                d3+=[d4]
            d1x+=[np.unravel_index(np.array(d3).argmin(), np.array(d3).shape)[1]-d2]
            d1y+=[np.unravel_index(np.array(d3).argmin(), np.array(d3).shape)[0]-d2]
        dx+=[d1x]
        dy+=[d1y]
        printProgressBar(i,x-d2-b_size)
    printProgressBar(1,1)
    return np.array(dx),np.array(dy)
