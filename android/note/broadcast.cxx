��Android�Ĺ㲥�����У�ActivityManagerService�����Ź㲥���ĵĽ�ɫ������ϵͳ�����й㲥��ע��ͷ���������

a��ע��㲥������:��ContextWrapper���н���ContextImpl����ʵ�֣�ContextWrapper.registerReceiver(xxx)

b����ActivityManagerService�У��㲥�ķ��ͺʹ������첽�ģ����ȴ��㲥������������Щ�㲥�ͷ����ˡ�
   �㲥��Ϣ���͵���ڣ�ContextWrapper.sendBroadcast

c��AndroidӦ�ó����͹㲥�Ĵ��¹��̣�
	1. �㲥���ͷ���ͨ��sendBroadcast��һ���㲥ͨ��Binder(���̼�ͨ��)���͸�ActivityManagerService�������ݹ㲥��Action�����ҵ���Ӧ�Ĺ㲥��������Ȼ�������㲥�Ž��Լ�����Ϣ������ȥ��
    2. ActivityManagerService����Ϣѭ���д�������㲥����ͨ��Binder(���̼�ͨ��)������㲥�ַ���ע��Ĺ㲥���շַ���ReceiverDispatcher��ReceiverDispatcher������㲥�Ž�MainActivity���ڵ��̵߳���Ϣ������ȥ
    3. ReceiverDispatcher���ڲ���Args��MainActivity���ڵ��߳���Ϣѭ���д�������㲥�������ǽ�����㲥�ַ�����ע���BroadcastReceiverʵ����onReceive�������д���
	
ContextWrapperʵ��:frameworks/base/core/java/android/content/ContextWrapper.java