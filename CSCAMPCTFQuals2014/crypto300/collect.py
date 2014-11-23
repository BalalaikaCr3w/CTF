#!/usr/bin/python
from socket import create_connection as cn

addr = ('178.63.58.69', 8085)

s = cn(addr)
l = open('log.txt', 'wb')
ctr = 0
while (True):
	ctr += 1
	s.send(64 * '\x00')
	response = s.recv(1024)
	if response.find(':0') != -1 or ctr % 5000 == 0:
		print response
		#print response.encode('hex')
	response = response.replace('Server response: ', '')
	l.write(response + '\n')

s.close()
l.close()
