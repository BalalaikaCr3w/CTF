import binascii
import base64
import time
import pprint

NUM_SIGMAS = 5


def rc4_init(key,needed_iter):
    keylength = len(key)
    S = list(range(256))
    j = 0
    for i in range(needed_iter):
        j = (j + S[i] + ord(key[i % keylength])) % 256
        S[i], S[j] = S[j], S[i]  # swap
    return S,j

def inverse(perm):
    inversed = [0] * len(perm)
    for i, p in enumerate(perm):
        inversed[p] = i
    return inversed


def rc4_crypt(S):
    i = 0
    j = 0
    while True:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]  # swap
        K = S[(S[i] + S[j]) % 256]
        yield K

def get_A_i(S_3,j_3,X,i,S_3_inv):
	t0 = j_3
	for l in range(3,i+4):
		t0 += S_3[l]
	t1 = 3+i-(ord(X[2+i]))
	return S_3_inv[t1]-t0


def compute_sigmas_vote(cur_IV,cur_X):
	global sigmas
	S_3,j_3 = rc4_init(cur_IV,3)	
	S_3_inv = inverse(S_3)
	for i in range(NUM_SIGMAS):
		sigmas[i].append(get_A_i(S_3,j_3,cur_X,i,S_3_inv))


def get_key(res_sigmas):
	t_key = (256+res_sigmas[0])%256
	if t_key>=128:
		t_key = 256-t_key
	key = chr(t_key)
	for i in range(1,len(res_sigmas)):
		t_key = (256+res_sigmas[i]-res_sigmas[i-1])%256
		if t_key>=128:
			t_key = 256-t_key
		key += chr(t_key)
	return key


#initi sigma's votes
sigmas = []
for i in range(NUM_SIGMAS):
	sigmas.append([])


Xs = []
IVs = []

t0 = time.clock()
buf = open('version2_data.txt','rb').read()
#buf = open('version1_data.txt','rb').read()
for line in buf.split('\n'):
	els = line.split(':')
	Xs.append(binascii.unhexlify(els[0]))
	IV_str = "%02X%02X%02X" % (int(els[1]),int(els[2]),int(els[3]))
	IVs.append(base64.b16decode(IV_str))

del buf
t1 = time.clock()


print(str(len(Xs))+' pairs have been read in '+str((t1-t0))+' seconds!')
print('make votes...')

t0 = time.clock()
for t in range(len(IVs)):
	compute_sigmas_vote(IVs[t],Xs[t])
t1 = time.clock()

print('votes ready in '+str((t1-t0))+' seconds!')


for i in range(NUM_SIGMAS):
	print(i,min(sigmas[i]),max(sigmas[i]))
print('\n\n\n')

#open('sigmas_votes.txt','wb').write(str(sigmas))

def do_auto_gues():
	res_sigmas = []
	for i in range(NUM_SIGMAS):
		vote_freq = {}
		for j in range(256):
			vote_freq[j] = 0
		for vote in sigmas[i]:
			vote = (256*2+vote) % 256
			vote_freq[vote] += 1
		max_vote_freq = vote_freq[0]
		max_vote_freq_id = 0
		for j in range(0,len(vote_freq)):
			if j not in vote_freq:
				continue
			if max_vote_freq< vote_freq[j]:
				max_vote_freq=vote_freq[j]
				max_vote_freq_id = j
		print('sigma_'+str(i)+' max = '+str(max_vote_freq_id)+" : "+str(max_vote_freq))
		res_sigmas.append(max_vote_freq_id)
	return get_key(res_sigmas)

def do_manual_gues():
	res_sigmas = []
	for i in range(NUM_SIGMAS):
		vote_freq = {}
		for j in range(256):
			vote_freq[j] = 0
		for vote in sigmas[i]:
			vote = (256*2+vote) % 256
			vote_freq[vote] += 1
		print('____ sigma '+str(i)+' ____')
		max_vote_freq = vote_freq[0]
		max_vote_freq_id = 0
		for j in range(2,len(vote_freq)-2):
			if j not in vote_freq:
				continue
			if vote_freq[j] == max(vote_freq[j-2],vote_freq[j-1],vote_freq[j],vote_freq[j+1],vote_freq[j+2]):
				print('loc_max: '+str(j)+" : "+str(vote_freq[j]))
			if max_vote_freq< vote_freq[j]:
				max_vote_freq=vote_freq[j]
				max_vote_freq_id = j
		print('\tabs_max: '+str(max_vote_freq_id)+" : "+str(max_vote_freq))
		print('____ sigma '+str(i)+' ____')		
		while True:
			cand = int(raw_input('enter sigma candidate: '))
			test_sigmas = res_sigmas + [cand]
			print('\tkey = '+get_key(test_sigmas).__repr__())
			bOk = raw_input('ok? (y=yes)')
			if bOk=='y':
				res_sigmas.append(cand)
				break		
	return get_key(res_sigmas)


print('auto guess key = '+do_auto_gues())
#print("\n\n\n\n")
#print('manual guess key = '+do_manual_gues())
