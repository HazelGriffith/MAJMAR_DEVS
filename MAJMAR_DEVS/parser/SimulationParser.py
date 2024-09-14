# -*- coding: utf-8 -*-
"""
Created on Fri May  3 13:13:04 2024

@author: hazel
"""

import pandas as pd
import numpy as np
import scipy.stats as st

filepathBase = '../simulation_results/'
helos = [1, 3, 6]
shiptimes = [0,12,24,36,48,60,72,84,96,108,120,132,144,156,168,200000]

for k in helos:
    for j in shiptimes:
        filepathRemainder = 'Exp1V2/'+str(k)+' Helos/ship@'+str(j)+'h/R'
        filepath = filepathBase + filepathRemainder
        simulation_results = []
        livesSaved = []
        for i in range(100):
            filepath += str(i+1) + '.csv'
            simulation_results.append(pd.read_csv(filepath))
            filepath = filepathBase + filepathRemainder
            FOLs = simulation_results[i].loc[simulation_results[i]['model_name'] == 'FOL']
            data = FOLs.iloc[-1]['data']
            livesSaved.append(int(data.split(';')[-1]))
    
        livesSaved = np.array(livesSaved)
        print("Helos: "+str(k)+" ShipTime: "+str(j))
        print(np.mean(livesSaved))
        print(st.norm.interval(confidence=0.95, 
                    loc = np.mean(livesSaved), 
                    scale = st.sem(livesSaved))[1]-np.mean(livesSaved))
