Androidϵͳ������(home)Ӧ�ó���Launcher�������Լ�������


Androidϵͳ�������Ĺ����У�������һ��Ӧ�ó���������PackageManagerService �������Ĺ��� �Լ� ����ʵ��˫������ͼ������Ӧ�ó���ȹ��ܡ�
Androidϵͳ������(home)Ӧ�ó���Launcher�������������£�

1��SystemServer.main
   ʵ�֣�frameworks/base/services/java/com/android/server/SystemServer.java
2��SystemServer.init1
   ���������һ��JNI����
   ʵ�֣�frameworks/base/services/jni/com_android_server_SystemServer.cpp
3��libsystem_server.system_init
   ����system_initʵ����libsystem_server����
   ʵ�֣�frameworks/base/cmds/system_server/library/system_init.cpp
4��
   ��󣬳�ʼ��SurfaceFlinger��MediaPlayerService��CameraService�ȷ���
   ������ͨ��ϵͳȫ��Ψһ��AndroidRuntimeʵ����callStatic������SystemServer��init2����
5��AndroidRuntime.callStatic
   ʵ�֣�frameworks/base/core/jni/AndroidRuntime.cpp
6��SystemServer.init2
   �����ﴴ����ServerThread��run�����У�����PackageManagerService��ActivityManagerServcie�ȷ���
   ʵ�֣�frameworks/base/services/java/com/android/server/SystemServer.java
7��PackageManagerService.main
   ���︺����Ѱ�װ��Ӧ�ó��������õ���package��provider��service��receiver��activity����Ϣ������PackageManagerService������
   ʵ�֣�frameworks/base/services/java/com/android/server/PackageManagerService.java
   ��ʱ���Ѱ�װ��Ӧ�ó���ֻ���൱����PackageManagerService������ע����ˣ������Ҫ��Android�����Ͽ�����ЩӦ�ó��򣬻���Ҫ��һ��HomeӦ�ó���
   �����PackageManagerService�����а���ע���Ӧ�ó���ȡ�����������Ѻõķ�ʽ��������չ�ֳ����������Կ��ͼ�����ʽ��
   ��Androidϵͳ�У������ϵͳ���Ѿ���װ��Ӧ�ó�����������չ�ֳ�����HomeӦ�ó������Launcher��
8��ActivityManagerService.setSystemProcess
   ʵ�֣�frameworks/base/services/java/com/android/server/am/ActivityManagerServcie.java
   ����ὫActivityManagerServiceʵ����ӵ�ServiceManager���йܣ������Ϳ���ͨ��ServiceManager.getService�ӿ����������ȫ��Ψһ��ActivityManagerServiceʵ���ˡ�
   �����ṹ��ActivityStack��ͨ��CATEGORY_HOME���͵�Intentȥ����Launcher(һ��ֻ��LauncherӦ�ó���ע����HOME���͵�Activity)��
   ��Launcher��onCreate�����н��ѽ�����Activity��Action����ΪIntent.ACTION_MAIN������Category����ΪIntent.CATEGORY_LAUNCHER��Activity��Ϣ��
   ���� ApplicationInfo �����С�
   Launcher�����ApplicationInfoչʾ����ͼ�꣬ͬʱ������ͨ��Launcher(��ʵ����һ��Activity)��Ӧclick���¼������������ĳһӦ�õȸ��ֹ��ܡ�

   
Androidϵͳ��HomeӦ�ó���Launcher����ActivityManagerService�����ģ���ActivityManagerService��PackageManagerServiceһ���������ڿ���ʱ��SystemServer��������ġ�
SystemServer�������������PackageManagerService������������װϵͳ��Ӧ�ó���
ϵͳ�е�Ӧ�ó���װ�����Ժ�SystemServer�����������Ҫͨ��ActivityManagerService������HomeӦ�ó���Launcher�ˣ�
Launcher��������ʱ����ͨ��PackageManagerServic��ϵͳ���Ѿ���װ�õ�Ӧ�ó����Կ��ͼ�����ʽչʾ�������ϣ������û��Ϳ���ʹ����ЩӦ�ó����ˡ�
	