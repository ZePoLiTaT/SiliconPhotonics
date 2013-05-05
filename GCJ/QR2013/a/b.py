from sys import stdin


num_cases = int(stdin.readline())
for case_index in xrange(1, num_cases+1):
    line = stdin.readline().strip().split(' ')
    n,s,p = int(line[0]),int(line[1]),int(line[2])
    
##    print '***[%s,%s] N:%d S:%d P:%d T:%s'%(nlimit,slimit,n,ss,p,t)
    print "Case #" + str(case_index) + ": " + str(maxg)
