��ϵͳ�����ű�system/core/rootdir/init.rc�ļ��У����ǿ��Կ�������Zygote���̵Ľű����
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server  
    socket zygote stream 666  
    onrestart write /sys/android_power/request_state wake  
    onrestart write /sys/power/state on  
    onrestart restart media  
    onrestart restart netd  
�ؼ���service����init���̴���һ����Ϊ"zygote"�Ľ��̣����zygote����Ҫִ�еĳ�����/system/bin/app_process��������Ҫ����app_process�Ĳ�����
ϵͳ�����ű�system/core/rootdir/init.rc����init���̽���ִ�У�init���̵�Դ����λ��system/core/init/init.c������service_start����������init.rc�ļ��е�service����


1��
Zygoteִ�г���system/bin/app_process�������main����
ʵ�֣�frameworks/base/cmds/app_process/app_main.cpp

2��
��zygote���������У�����ִ�� runtime.start("com.android.internal.os.ZygoteInit", startSystemServer);  
runtime.start()���յ��õ��� AndroidRuntime.start������������ϵͳ����ʱ�⡣
����Ҫ�����������飺
	һ�ǵ���startVM�����������
	���ǵ���startRegע��JNI������
	���ǵ�����com.android.internal.os.ZygoteInit���main������
AndroidRuntime��ʵ�֣�frameworks/base/core/jni/AndroidRuntime.cpp

3��
zygoteInit.main()
����Ҫ������������: 
	һ�ǵ���registerZygoteSocket����������һ��socket�ӿڣ�������ActivityManagerServiceͨѶ��
	���ǵ���startSystemServer����������SystemServer�����
	���ǵ���ZygoteInit.runSelectLoopMode��������һ������ѭ����ǰ�洴����socket�ӿ��ϵȴ�ActivityManagerService���󴴽��µ�Ӧ�ó�����̡�
ʵ�֣�frameworks/base/core/java/com/android/internal/os/ZygoteInit.java


4��
�ϸ������еĵڶ�������---����startSystemServer����������SystemServer�����
	a��ZygoteInit.startSystemServer
	Zygote����ͨ��Zygote.forkSystemServer��������һ���µĽ���������SystemServer���������ֵpid��0�ĵط������µĽ���Ҫִ�е�·�������´����Ľ��̻�ִ��handleSystemServerProcess����
		   
	b��ZygoteInit.handleSystemServerProcess
	������Zygote���̴������ӽ��̻�̳�Zygote�������ϸ�����ĵ�һ�������д�����Socket�ļ�����������������ӽ����ֲ����õ����������Ҫ����closeServerSocket���ر�����
	����������ŵ���RuntimeInit.zygoteInit��������һ��ִ������SystemServer����Ĳ���

	c��RuntimeInit.zygoteInit
	�����ִ������������
	    һ������zygoteInitNative������ִ��һ��Binder���̼�ͨ�Ż��Ƶĳ�ʼ������������������֮����������е�Binder����Ϳ��Խ��н��̼�ͨ����
		���������ϸ����贫������com.android.server.SystemServer���main������

5��
Zygote���̾���������ˣ������ܽ�һ�£�
    1. ϵͳ����ʱinit���̻ᴴ��Zygote���̣�Zygote���̸������AndroidӦ�ó����ܲ���������̵Ĵ���������������
    2. Zygote���̻����ȴ���һ��SystemServer���̣�SystemServer���̸�������ϵͳ�Ĺؼ���������������PackageManagerService��Ӧ�ó�������������ActivityManagerService��
    3. ��������Ҫ����һ��AndroidӦ�ó���ʱ��ActivityManagerService��ͨ��Socket���̼�ͨ�Ż��ƣ�֪ͨZygote����Ϊ���Ӧ�ó��򴴽�һ���µĽ��̡�

		
SystemServerʵ�֣���frameworks/base/services/java/com/android/server/SystemServer.java
ZygoteInitʵ�֣���frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
ZygoteInit.runSelectLoopModeʵ�֣���frameworks/base/core/java/com/android/internal/os/ZygoteInit.java