# https://stats.stackexchange.com/questions/181/how-to-choose-the-number-of-hidden-layers-and-nodes-in-a-feedforward-neural-netw/136542#136542
def maxNeurons(n_input, n_output, n_samples, alpha):
    return n_samples/(alpha * (n_input+n_output))
