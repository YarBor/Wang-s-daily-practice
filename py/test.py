a = int (input())
arr={}
for i in range(0,a):
    ii = input()
    jj = float(input())
    arr.update({jj:ii})
h = list(arr.keys())
h.sort()
# h.
print('{:.2f},{}'.format(float(h[a-1]),arr[h[a-1]]))

print('{:.2f},{}'.format(float(h[0]),arr[h[0]]))