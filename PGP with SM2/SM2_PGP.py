import secrets
from gmssl import sm2
from gmssl.sm4 import CryptSM4, SM4_ENCRYPT, SM4_DECRYPT



class PGP:
    def __init__(self):
        #SM2公私钥
        self.private_key = '00B9AB0B828FF68872F21A837FC303668428DEA11DCD1B24429D0C99E24EED83D5'
        self.public_key = 'B9C9A6E04E9C91F7BA880429273747D7EF5DDEB0BB2FF6317EB00BEF331A83081A6994B8993F3F5D6EADDDB81872266C87C018FB4162F5AF347B483E24620207'
        self.SM2 = sm2.CryptSM2(public_key=self.public_key, private_key=self.private_key)

    def sender(self,msg):
        session_key=secrets.token_bytes(16)
        SM4_encrypt=CryptSM4()
        SM4_encrypt.set_key(session_key,SM4_ENCRYPT)
        Encrypted_data=SM4_encrypt.crypt_ecb(bytes(msg,encoding='utf-8'))
        PKEnc_data=self.SM2.encrypt(session_key)
        return Encrypted_data,PKEnc_data

    def receiver(self,Encrypt_data,PKEnc_data):
        session_key=self.SM2.decrypt(PKEnc_data)
        SM4_encrypt=CryptSM4()
        SM4_encrypt.set_key(session_key,SM4_DECRYPT)
        message=SM4_encrypt.crypt_ecb(Encrypt_data)
        return message


if __name__=='__main__':
    m='abcdefg'

    test=PGP()
    a,b=test.sender(m)
    m_=test.receiver(a,b)
    print(m_.decode('utf-8'))