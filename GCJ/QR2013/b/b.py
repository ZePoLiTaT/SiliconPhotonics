from sys import stdin
#NOSIRVE
n=m=0
lawn=[]

def findCompLine(mif, maf, mic, mac, hf, hc):
	h = 0
	f = c = -1

	for i in xrange(mif, maf):
		comp = lawn[i][mic]	
		countf = 0
		for j in xrange(mic,mac):
#			print comp, lawn[i][j]
			if comp == lawn[i][j] and lawn[i][j] <= hf:
				countf = countf + 1
			else:
				break				
#		print ".......", countf
		if countf == (mac-mic):
			f = i
			h = comp
			print f,c,h
			return [f,c,h]			

	for j in xrange(mic,mac):
		comp = lawn[mif][j]	
		countf = 0
		for i in xrange(mif, maf):
			print comp, lawn[i][j], hc
			if comp == lawn[i][j] and lawn[i][j] <= hc:
				countf = countf + 1
			else:
				break
		print ".......", countf, (maf-mif)
		if countf == (maf-mif):
			c = j
			h = comp		
			print "Si Cols",c,comp
			break
			
	print f,c,h
	return [f,c,h]
'''	
lawn=[[1,2,3],[1,4,3],[4,4,3]]
print "***"
findCompLine(0,3,0,3,10,10)
print "**-*"
'''

count = 1
def pattern(mif, maf, mic, mac, hf, hc):
	global count
	count = count + 1
	if ( count > 10):
		return True
		
	print "pattern(%d,%d,%d,%d,%d,%d)"%(mif, maf, mic, mac, hf, hc)
	if(mif < 0 or maf > n or mic < 0 or mac > m or (maf-mif)<=0 or (mac-mic)<=0):
		return True
	
	[f,c,h] = findCompLine(mif,maf,mic,mac, hf, hc)
	
	if f != -1:
		return pattern(mif, f, mic, mac, h, hc) and pattern(f+1, maf, mic, mac, h, hc)
	elif c != -1:
		return pattern(mif, maf, mic, c, hf, h)	and pattern(mif, maf, c+1, mac, hf, h)	
	else:
		return False

	
num_cases = int(stdin.readline())
for case_index in xrange(1): #(1, num_cases+1):
	n, m = [int(x) for x in stdin.readline().split()]
	lawn=[]
	for ni in xrange(n):
		lawn.append([int(x) for x in stdin.readline().split()])
	print n,m
	print lawn
	pattern(0,n,0,m,100,100)
#	print "Case #" + str(case_index) + ": " + str(numfspal)


