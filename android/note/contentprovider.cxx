ContentProvider����ڲ�ͬӦ�ó���֮�䴫�������ǻ������������ڴ������ʵ�ֵġ�
��Ϊ����Ӧ�ó������֮�������������ڴ�ķ�ʽ����������Ч���Ƿǳ��ߵģ���Ϊ����֮��ֻ��Ҫ����һ���ļ��������Ϳ����ˡ�

AndroidӦ�ó������Content Provider�е����ݸ���֪ͨ���ƺ�Androidϵͳ�еĹ㲥��Broadcast��֪ͨ���Ƶ�ʵ��˼·�����Ƶġ�
��Android�Ĺ㲥�����У������ǽ����߶��Լ�����Ȥ�Ĺ㲥����ע�ᣬ���ŵ������߷�����Щ�㲥ʱ�������߾ͻ�õ�֪ͨ�ˡ�

Content Provider�е����ݼ�ػ�����Androidϵͳ�еĹ㲥��������������Ҫ������
	һ��ǰ����ͨ��URI����֪ͨ�ķ����ߺͽ����߹�����һ��ģ���������ͨ��Intent��������
	����ǰ�ߵ�֪ͨע����������ContentService���������ݵģ�����������ActivityManagerService���������ݵ�
	����ǰ�߸���������ݸ���֪ͨ�������Ҫ�̳�ContentObserver�࣬������Ҫ�̳�BroadcastReceiver�ࡣ
֮���Ի�����Щ����������Content Proivder��������ݹ����ܱ�����ǽ�����URI�Ļ���֮�ϵģ����ר�����URI���������һ��֪ͨ���ƻ��ʵ�úͷ��㣬
��Androidϵͳ�Ĺ㲥������һ�ָ���ͨ�õ��¼�֪ͨ���ƣ��������÷�Χ����㷺һЩ��

ContentService���������̷���
	Androidϵͳ���� Zygote ����ʱ��������һ��System����������ϵͳ��һЩ�ؼ����񣬶�ContentService����Щ�ؼ�����֮һ��
	�� System �����У��������ϵͳ�ؼ��������ΪSystemServer�࣬��������һ���߳� SystemThread ��������Щ�ؼ�����
	�� SystemThread �л�ͨ�� ContentService.main(xxx) ������ ContentService��
 
ContentProvider�����ݱ��֪ͨ��
	ContentResolver.notifyChange
	ʵ�֣�frameworks/base/core/java/android/content/ContentResolver.java
 
ContentServiceʵ�֣�frameworks/base/core/java/android/content/ContentService.java
SystemServerʵ�֣�frameworks/base/services/java/com/android/server/SystemServer.java