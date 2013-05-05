from sys import stdin

def validate(pos, vchar, curv, maxv):
	curv = curv+1 if pos in (vchar,'T') else curv;
	maxv = curv if maxv < curv else maxv;
	return [curv,maxv]

results = ['X won','O won', 'Game has not completed', 'Draw'];
num_cases = int(stdin.readline())
for case_index in xrange(1, num_cases+1):
  
	ttt = [stdin.readline().strip(),
	stdin.readline().strip(),
	stdin.readline().strip(),
	stdin.readline().strip()]
	stdin.readline()

	nxmax = nomax = 0;
	empty = False;

	nxd = nod = nxid = noid = 0;
	for i in xrange(4):
		[nxd, nxmax] = validate(ttt[i][i], 'X', nxd, nxmax);
		[nod, nomax] = validate(ttt[i][i], 'O', nod, nomax);	
		[nxid, nxmax] = validate(ttt[i][3-i], 'X', nxid, nxmax);
		[noid, nomax] = validate(ttt[i][3-i], 'O', noid, nomax);	
	
		nxh = noh = nxv = nov = 0;
		for j in xrange(4):
			empty = empty or ttt[i][j] == '.'; 

			[nxh, nxmax] = validate(ttt[i][j], 'X', nxh, nxmax);
			[noh, nomax] = validate(ttt[i][j], 'O', noh, nomax);
			[nxv, nxmax] = validate(ttt[j][i], 'X', nxv, nxmax);
			[nov, nomax] = validate(ttt[j][i], 'O', nov, nomax);
		
#		print nxh, nxv, nxd, nxid, nxmax
#		print noh, nov, nod, noid, nomax

	if(nxmax == 4):
		res = results[0];
	elif (nomax == 4):
		res = results[1];
	else:
		if empty:
			res = results[2];
		else:
			res = results[3];
			
	print "Case #" + str(case_index) + ": " + res
