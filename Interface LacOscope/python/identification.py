# -*- coding: utf-8 -*-
"""
Created on Mon Mar  1 14:30:53 2021

@author: Estos
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from skimage import color
from skimage import io
from skimage import morphology as skmo
from skimage import measure as skme

from morphocut.contrib.zooprocess import regionprop2zooprocess

import utils
import pathlib


plt.rcParams["image.cmap"]="gray"

p=str(pathlib.Path(__file__).parent.absolute())
im_path=str(pathlib.Path(__file__).parent.parent.absolute())+"/img/segmentation.jpg"

def traitement_image(I):
    
    kernel = skmo.disk(6)
    closing1=skmo.closing(I,kernel)
    utils.printProgressBar(1,8)

    counts, bins = np.histogram(closing1,50,[0,1])
    utils.printProgressBar(2,8)

    alpha=10
    beta=bins[np.argmax(counts)]*0.8
    contrast=np.clip((closing1-beta)*alpha,0,1)
    utils.printProgressBar(3,8)


    binarize=contrast<0.06
    utils.printProgressBar(4,8)
   
    kernel = skmo.disk(20)
    closing=skmo.closing(binarize,kernel)
    utils.printProgressBar(5,8)
    
    kernel = skmo.disk(10)
    opening=skmo.opening(closing,kernel)
    utils.printProgressBar(6,8)
    
    dilate = skmo.dilation(opening,kernel)
    utils.printProgressBar(7,8)
   
    
#    plt.figure()
#    plt.imshow(I,cmap="gray")
#    plt.title("Image de base")
#    
#    plt.figure()
#    plt.title("Closing")
#    plt.imshow(closing1,cmap="gray")
#    
#    plt.figure()
#    plt.hist(bins[:-1], bins, weights=counts)
#    
#    plt.figure()
#    plt.imshow(contrast,cmap="gray")
#    plt.title("Contrast")
#    
#    plt.figure()
#    plt.imshow(binarize,cmap="gray")
#    plt.title("Binarisation")
#    
#    plt.figure()
#    plt.title("closing")
#    plt.imshow(closing,cmap="gray")
#    
#    plt.figure()
#    plt.title("opening")
#    plt.imshow(opening,cmap="gray")
#    
#    plt.figure()
#    plt.title("Dilation")
#    plt.imshow(dilate,cmap="gray")
    
    res=closing1*dilate
    utils.printProgressBar(8,8)

#    plt.figure()
#    plt.axis('off')
#    plt.imshow(res)
    
    # utils.save_obj(res,"image_traitee")
    return res

def segmentation(img_orig,im_traitee):
    print("segmentation")
    L=skme.label(im_traitee>0.05)
    
    # I_label=color.label2rgb(L,image=img,bg_label=0)
    
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.imshow(img_orig)
    
    regions=skme.regionprops(L,intensity_image=L)
    i=1
    for region in regions:
        if region.area >= 200:
            minr, minc, maxr, maxc = region.bbox
            rect = mpatches.Rectangle((minc, minr), maxc - minc, maxr - minr,fill=False, edgecolor='red', linewidth=2)
            ax.add_patch(rect)
            plt.text(minc+(maxc-minc)/2,minr-30,str(i),color="red",size="x-large",weight="bold")
            i+=1
    
    ax.set_axis_off()
    plt.tight_layout()
    
    plt.savefig(im_path)
    
    return regions



def format_features(regions):
    labels_to_extract = ['object_%area',"object_bx",'object_by',"object_area_exc","object_circ.","object_circex","object_elongation",'object_height',
                         "object_intden","object_major","object_mean","object_minor","object_perim.","object_perimmajor","object_range"]
    
    features = np.zeros(len(labels_to_extract))
    features_final = np.zeros((len(regions),len(features)))
    print("format features")
    for j in range(len(regions)):
        print("format features region "+str(j))
        compt = 0
        d=regionprop2zooprocess(regions[j])
        try :
            for elem in labels_to_extract :
                features[compt] = d[elem]
                compt = compt+1
        except KeyError:
            features[compt] = 0
            print("\n"+str(elem) + " is not a valid label, please check")
        except ValueError:
            features[compt] = 0
            print("\n"+str(elem) + " is not a valid label, please check")
        for i in range(0,len(features)):
            if (str(features[i]) == "nan"):
                return "nan value"
            
        features_final[j,:]=features[:]
        
    return features_final



def prediction(path):
    print("python")
    img = color.rgb2gray(io.imread(path))  
    print("image chargee")
    img_traitee=traitement_image(img)
    print("image traitee")
    regions=segmentation(img,img_traitee)
    print("segmentation ok")
    features=format_features(regions)
    print("format features ok")
    forest = utils.load_model()
    print("load forest ok")
    predicted = forest.predict(features)
    print("CLASSE : "+str(predicted))
    return str(predicted)

#prediction("~/Desktop/segmenter/img/17_25_29_031485.jpg")
 

