һ��
startService �����½����������Զ�������ԭ�������

���ȣ�Activity��ͨ�� startService(new Intent("service name")); ����������Activity�̳���ContextWrapper�࣬
��startService����������ContextWrapper����ʵ�ֵġ������ֵ�����ActivityManagerProxy���startService��ʵ����������Ĳ�����
	
�ܽ��������ǣ�Androidϵͳͨ������Binder���̼�ͨ����������½�������������Ĺ��̣����β����ֱ��ǣ�
    1. �������̵��õ�ActivityManagerService�����У�����½��̵Ĵ�����
    2. ���½��̵��õ�ActivityManagerService�����У���ȡҪ���½��������ķ���������Ϣ��
    3. ��ActivityManagerService�����ֻص��½����У����ս��������������� 

	
ContextWrapperʵ�֣�frameworks/base/core/java/android/content/ContextWrapper.java	
ActivityManagerServiceʵ�֣�frameworks/base/services/java/com/android/server/am/ActivityManagerService.java


����
bindService ������һ����Ӧ�ó�������ڲ�����Service�ķ���������ʵ��Ҳ��ContextWrapper����ʵ�ֵġ�
AndroidӦ�ó���󶨷���bindService���Ĺ��̣�����Ϊ��
    1. MainActivity����bindService����֪ͨActivityManagerService����Ҫ����CounterService�������ActivityManagerService������MainActivity���ڵĽ����ڲ���CounterService�������������ҵ�������onCreate������
    2. ActivityManagerService��CounterService���������󣬼�������CounterService��onBind������Ҫ��CounterService����һ��Binder���������
    3. ActivityManagerService��CounterService���õ����Binder����󣬾Ͱ�������MainActivity���������Binder������Ϊ�������ݸ�MainActivity�ڲ������ServiceConnection�����onServiceConnected������
    4. MainActivity�ڲ������ServiceConnection�����onServiceConnected�����ڵõ����Binder����󣬾�ͨ������getService��ͬ�������CounterService�ӿڡ�