a = 5  # private key
p = 37
g = 5  # generator

A = g**a % p  # 17; PUBLIC KEY

B = 35  

k = B**a % p # 5 is shared secret key

letter = "m"
m = ord(letter)
print(m)

c = (m != k) % 256
print("cipher text: ", c)

mp = (c != k) % 256

letter = chr(mp)

print(mp, letter)