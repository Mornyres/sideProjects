# Forest LeBlanc
# Practice with globular protein secondary structure prediction via NN

import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader

import pandas as pd

import numpy as np
import matplotlib.pyplot as plt

import os
import random

datapath = "../data/protein-secondary-structure.train"
aminoAcids = "ARNDCQEGHILKMFPSTWYV"


# only data between <> and <end>
with open(datapath, 'r') as f:
    x_train = []
    y_train = []
    for line in f:
        if (line[0] not in aminoAcids):
            continue
        print (line)
        x_train.append(line[0])
        y_train.append(line[1])
        break

# visualize raw data


## NN 1: 1988-style with PyTorch
# initialize NN

    # input is a string of variable length
    # each input is binary encoded to 20 bits and 1 null input
    # moving window (size 17 due to chains being up to 8 units from central point)
    # 357 total binary inputs 

    # randomize initial weights

## TODO: other NN topologies

# 
# classify as either helix(H) or sheet(E), or if certainty of both fails to meet threshold (0.37 starting point) classify as coil(_)
classes = ('coil','helix','sheet')

# hidden layer, etc

# visualize performance, measure accuracy

print("Exiting...")
