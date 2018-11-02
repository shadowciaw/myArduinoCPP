a = 5  # private key
p = 37
g = 5  # generator

A = g**a % p  # 17; PUBLIC KEY

B = # neighbour's public key

k = B**a % p
