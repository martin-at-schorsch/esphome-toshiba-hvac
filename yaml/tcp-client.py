import socket
import sys
import binascii
import time



def get_constants(prefix):
    """Create a dictionary mapping socket module constants to their names."""
    return dict( (getattr(socket, n), n)
                 for n in dir(socket)
                 if n.startswith(prefix)
                 )

families = get_constants('AF_')
types = get_constants('SOCK_')
protocols = get_constants('IPPROTO_')

SERVER='localhost'
SERVER_PORT=1000

# Create a TCP/IP socket
sock = socket.create_connection((SERVER, SERVER_PORT))

print  ('Family  :', families[sock.family])
print  ('Type    :', types[sock.type])
print ('Protocol:', protocols[sock.proto])


ge_svt_messages = [b'\x01\x03\xc0\x00\x00\x14\x79\xc5', b'\x01\x03\xc0\xb2\x00\x05\x19\xee', b'\x01\x03\xc0\x20\x00\x10\x79\xcc', b'\x01\x03\xc0\x30\x00\x03\x39\xc4']



try:
    
    while (1):
        for message in ge_svt_messages: 

            # Send data
            #message = b'This is the message.  It will be repeated.'

            print ( 'sending ' ,binascii.hexlify(message, '-'))
            sock.sendall(message)

            amount_received = 0
            amount_expected = len(message)

            while amount_received < amount_expected:
                data = sock.recv(64)
                amount_received += len(data)
                print ('received ' ,binascii.hexlify( data, '-'))
        time.sleep(10)


except socket.timeout:
    sock = socket.create_connection((SERVER, SERVER_PORT))    


except socket.close:
    sock = socket.create_connection((SERVER, SERVER_PORT))    


finally:
    print ('closing socket')
    sock.close()