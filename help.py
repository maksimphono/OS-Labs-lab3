from numpy import uint64

def cow(pte):
    return (pte >> 8) & uint64(1)

def w(pte):
    return (pte >> 2) & uint64(1)

def pa(pte):
    return hex((pte >> 10) << 12)
