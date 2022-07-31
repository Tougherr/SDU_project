from ec import EC_mul
from gmssl import sm3, func
from Crypto.Util.number import inverse
import sys
import socket
import binascii
from random import randint

P = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
N = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
A = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
B = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
G_X = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
G_Y = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
G = (G_X, G_Y)



address = ('127.0.0.1', 1234)
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    client.connect(address)
    print("connection established！")
except Exception:
    print('connection failed！')
    sys.exit()
else:

    d1 = randint(1,N-1)
    P1 = EC_mul(inverse(d1,P),G)
    x,y = hex(P1[0]),hex(P1[1])
    
    client.sendto(x.encode('utf-8'), address)
    client.sendto(y.encode('utf-8'), address)

    msg = "SM2 2P sign of myx"
    msg = hex(int(binascii.b2a_hex(msg.encode()).decode(), 16)).upper()[2:]
    ID_user = "Tougherr"
    ID_user = hex(int(binascii.b2a_hex(ID_user.encode()).decode(), 16)).upper()[2:]
    ENTL_A = '{:04X}'.format(len(ID_user) * 4)
    ide = ENTL_A + ID_user + '{:064X}'.format(A) + '{:064X}'.format(B) + '{:064X}'.format(G_X) + '{:064X}'.format(G_Y)
    
    Z = sm3.sm3_hash(func.bytes_to_list(ide.encode()))
    M=Z + msg
    e = sm3.sm3_hash(func.bytes_to_list(M.encode()))
    k1 = randint(1,N-1)
    Q1 = EC_mul(k1,G)
    x,y = hex(Q1[0]),hex(Q1[1])

    client.sendto(x.encode('utf-8'),address)
    client.sendto(y.encode('utf-8'),address)
    client.sendto(e.encode('utf-8'),address)

    r,_ = client.recvfrom(1024)
    r = int(r.decode(),16)
    s2,_ = client.recvfrom(1024)
    s2 = int(s2.decode(),16)
    s3,_ = client.recvfrom(1024)
    s3 = int(s3.decode(),16)
    s=((d1 * k1) * s2 + d1 * s3 - r)%N
    print(f"Signature : {hex(r)} {hex(s)}")
    client.close()