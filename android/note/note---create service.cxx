���棺
�ڷ���ˣ�a��Service����ʵ�֣��̳�Service�ӿ�   b��Binder����ʵ�֣��̳�Binder�ӿڣ���ʵ�� [IPC�����ӿ�] 
�ڿͻ��ˣ�����service����ע�� ServiceConnection �ص����ڻص��ӿ��л�ȡ�� IBinder ����ͨ��ǿת�� IBinder ����ת����[IPC�����ӿ�]
 
AIDL (Android Interface Definition Language) ��һ��IDL ���ԣ��������ɿ�����Android�豸����������֮����н��̼�ͨ��(interprocess communication, IPC)�Ĵ��롣 
AIDL IPC����������ӿڵģ���COM��Corbaһ�������Ǹ���������������ʹ�ô������ڿͻ��˺�ʵ�ֶ˴������ݡ�
AIDL��һ�ֽӿڶ������ԣ�����Լ���������̼��ͨѶ���򣬹����������ɴ��룬ʵ��Android�豸�ϵ��������̼�ͨ��(IPC)�� 
����һ��aidl�ļ������岽��ܼ򵥵ģ�
aidl�Ķ����interface�Ķ�������Ƶģ�ʹ��interface�ؼ��֣���aidl�в��������η�(public,protected,private)����Ȼ����Ȼ���ļ��ĺ�׺��.java�ĳ�.aidl����ʱ��gen�ļ�������ͻ���һ����֮��Ӧ��java�ļ���
����Ǳ������ɵ����顣���aidl�ӿ��ж����һ�㶼��Client��Service�����Ľӿڡ�
��������һ��StudentQuery.aidl�ļ�ʾ����
	package cn.itcast.aidl;  
	//ע��û���κεķ���Ȩ�����η�  
	interface StudentQuery {  
		//ͨ��number������ѧ����name  
		String queryStudent(int number);  
	}  

�ٷ��ĵ��ر��������Ǻ�ʱʹ��AIDL�Ǳ�Ҫ�ģ�ֻ��������ͻ��˴Ӳ�ͬ��Ӧ�ó���Ϊ�˽��̼��ͨ�Ŷ�ȥ�������service���Լ��������service������̡߳�
�ڷ���ˣ�a��Service����ʵ�֣��̳�Service�ӿ�  b��Binder����ʵ�֣���Ҫʵ��Binder���󣬵��Ǽ̳е��� [aidl�ļ��������Ľӿ���.Stub] ��(���ʵ����Binder)
�ڿͻ��ˣ���Ϊ�ǿ�Ӧ�÷��ʵģ�����ʹ����ʽIntent�������񣬲�ע�� ServiceConnection �ص����ڻص��ӿ��л�ȡ�� IBinder ���󣬲�ͨ�� [aidl�ļ��������Ľӿ���.Stub].asInterface() ������ IBinder ����ת���ɺͷ���˽����Ľӿ�  
          �������aidl�ļ�����package��ͬaidl�ļ�һ�𿽱����ͻ��ˣ����������Զ��ڿͻ��˵�genĿ¼��ͬ������ [aidl�ļ���.java] �Ľӿ��ļ�
 
 