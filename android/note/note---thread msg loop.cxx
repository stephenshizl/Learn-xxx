һ��
�߳���Ϣѭ������
  1��
	Ӧ�ó�������ʱ�����̻߳�׼����һ������Ϣѭ����
  2��
	Androidϵͳ�ṩ��HandlerThread�࣬�����д���������Ϣѭ�����ܵ����̵߳����á�
    HandlerThread��ʵ����frameworks/base/core/java/android/os/HandlerThread.java�ļ��У�
  3��
	�������̲߳���ֱ�Ӳ���Ӧ�ó����UI����ˣ���Ҫͨ����Ӧ�ó�������߳��з�����Ϣ��֪ͨӦ�ó������̸߳��½����ϵ����ؽ��ȡ�
	AndroidϵͳΪ������Ա�ṩ��һ���첽�����ࣨAsyncTask����ʵ��������˵�Ĺ��ܣ���������һ�����߳���ִ�м�������ͬʱͨ�����̵߳���Ϣѭ������ø���Ӧ�ó������Ļ��ᡣ
		A. MyTask��̳���AsyncTask�࣬�����Ҳ��һ���첽�����ࣻ
		B. MyTask���doInBackground�������ں�̨�����߳������еģ���ʱ���������Բ���Ӧ�ó���Ľ��棻
		C. MyTask���onProgressUpdate��onPostExecute����������Ӧ�ó�������߳���ִ�У����ǿ��Բ���Ӧ�ó���Ľ��档
	AsyncTask�ඨ����frameworks/base/core/java/android/os/AsyncTask.java�ļ���
	 
����
������Ϣ�������̣�
	SystemServer ---> WindowManagerService ---> InputManager
	WindowManagerService.main
	ʵ�֣�frameworks/base/services/java/com/android/server/WindowManagerService.java�ļ�
	InputManager��Java��ʵ�֣�frameworks/base/services/java/com/android/server/InputManager.java
	InputManager��C++��ʵ���У��ֱ𴴽���һ��InputReader�����һ��InputDispatcher����ǰ�߸����ȡϵͳ�еļ�����Ϣ�����߸���Ѽ�����Ϣ�ַ���ȥ��
 
