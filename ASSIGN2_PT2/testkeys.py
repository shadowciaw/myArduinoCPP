p = 2147483647
g = 16807

a = 4294957653
A = (g**a) % p
B = 1392753699
k = (B**a)%p

print("private key: ", a, "public key: ", A, "shared key: ", k)

'''
Your private key is: 4294957653
Your public key is: 903313486

got to handshake
I'm client
write cr and own key
got a byte!
other key is:1000001
sending ack
Other key is: 65
Your shared key is: 858345747
1392753699

'''