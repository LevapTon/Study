from time import monotonic
import sort_by_inserting as prog


start = monotonic()

res = (prog.make_lst())

finish = monotonic()

print(res[0:10])
print('%.6f' %(finish - start))
