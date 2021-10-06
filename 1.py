s = "abcabcbc"

dic = {}
res = 0
j = 0
for i in range(len(s)):
    if s[i] not in dic:
        dic[s[i]] = i
    else:
        # print(dic[s[i]]+1, ' ', j)
        j = max(j, dic[s[i]]+1)
        # print("j: ", j)
        dic[s[i]] = i
        # print("i: ",i)
    res = max(res, i-j+1)
    for key,loc in dic.items():
        print(key, loc)
    print("====")
    print("res: ",res)
dic['a'] = 0
dic['b'] = 1
dic['c'] = 2
print(dic['c']+1)

print(res)
