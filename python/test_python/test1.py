old=set('')
f = open("D://old.txt")             # ����һ���ļ�����
line = f.readline()             # �����ļ��� readline()����
while line:
    # print line,                 # ����� ',' �����Ի��з�
    # print(line, end = '')������# �� Python 3��ʹ��
    line=line.strip('\n')
    old.add(line)
    line = f.readline()
f.close()


new=set('')
f1 = open("D://new.txt")             # ����һ���ļ�����
line1 = f1.readline()             # �����ļ��� readline()����
while line1:
    # print line1,                 # ����� ',' �����Ի��з�
    # print(line, end = '')������# �� Python 3��ʹ��
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
