old=set('')
f = open("D://old.txt")             # 返回一个文件对象
line = f.readline()             # 调用文件的 readline()方法
while line:
    # print line,                 # 后面跟 ',' 将忽略换行符
    # print(line, end = '')　　　# 在 Python 3中使用
    line=line.strip('\n')
    old.add(line)
    line = f.readline()
f.close()


new=set('')
f1 = open("D://new.txt")             # 返回一个文件对象
line1 = f1.readline()             # 调用文件的 readline()方法
while line1:
    # print line1,                 # 后面跟 ',' 将忽略换行符
    # print(line, end = '')　　　# 在 Python 3中使用
    line1=line1.strip('\n')
    new.add(line1)
    line1 = f1.readline()
f1.close()

old_and_new = old & new;
old_only = old - old_and_new;
new_only = new - old_and_new;


fa = open("D://rst_old_and_new.txt", 'w')
fb = open("D://rst_old_only.txt", 'w')
fc = open("D://rst_new_only.txt", 'w')

print('old_and_new --------------------------------------------------')
for i in old_and_new:
    print i
    fa.write(i)
    fa.write('\n')

print('')
print('old_only --------------------------------------------------')
for j in old_only:
    print j
    fb.write(j)
    fb.write('\n')

print('')
print('new_only --------------------------------------------------')
for k in new_only:
    print k
    fc.write(k)
    fc.write('\n')

fa.close()
fb.close()
fc.close()
