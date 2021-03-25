#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 09:07:56 2021

@author: robin
"""


import imp
import pip

liste_modules=["matplotlib","numpy","skimage","time","pathlib","utils","morphocut","os","sys","pickle","joblib","sklearn"]

echec="Echec verification module: "
def module_check():
    for m in liste_modules:
        try: 
            imp.find_module(m)
        except:
            ImportError()
            print("La bibliothèque "+m+" est manquante. Le programme va tenter de l'installer...")
            if pip.main(['install', m]):
                print("\r Echec")
                return echec+m
            print("\r Ok")
    return "Verification modules Python ok"

# print(module_check())
    