AndroidӦ�ó����ܲ㴴��Ӧ�ó���Ľ���


��AndroidӦ�ó����ܲ��У�����ActivityManagerService�������ΪAndroidӦ�ó��򴴽��µĽ��̵ģ�����������һ����ϵͳ�����Ĺ����д����Ķ�������֮�С�
AndroidӦ�ó����ܲ㴴����Ӧ�ó�����̾��������ص㣬һ�ǽ��̵���ں�����ActivityThread.main�����ǽ�����Ȼ֧��Binder���̼�ͨ�Ż��ƣ��������ص㶼���ڽ��̵ĳ�ʼ��������ʵ�ֵġ�

ActivityManagerService�����µĽ����Ǵ����Ա����startProcessLocked��ʼ�ġ�
����startProcessLocked�е�����Process.start ������ʼΪӦ�ó��򴴽��µĽ��̣�ע�⣬������һ����һ������Ϊ"android.app.ActivityThread"��
����ǽ��̳�ʼ��ʱҪ���ص�Java���ˣ����������ص�����֮�󣬾ͻ��������ľ�̬��Ա����main��Ϊ���̵���ڵ㡣

��ϸ������£�
---> Process.startViaZygote
---> zygoteSendArgsAndGetPid
---> sZygoteWriterд���� [������sZygoteWriter��һ����openZygoteSocketIfNeeded�����򿪵�Socketд����]
---> д���������ZygoteInit.runSelectLoopMode�������� ---> peers.get(index)�õ�ZygoteConnection����[��ʾһ��Socket����]�����ŵ���ZygoteConnection.runOnce��������
---> Zygote.forkAndSpecialize �����ӽ��� ---> �ӽ���[pid==0]�е���ZygoteConnection.handleChildProc --->  RuntimeInit.zygoteInit 
---> RuntimeInit.zygoteInitNative ִ��Binder���������ʼ�� ---> RuntimeInit.invokeStaticMainִ�������ļ��ض���[�����classNameΪ"android.app.ActivityThread"]
---> ClassLoader.loadClass����className����Ȼ���ȡ���ľ�̬��Ա����main 
---> û��ֱ�ӵ��þ�̬��Ա����main������ͨ���׳��쳣ZygoteInit.MethodAndArgsCaller����ZygoteInit.main�����ڲ�������쳣ʱ�ٵ�����
     Ϊʲô�أ�ע��˵����Ϊ�������ջ����������android.app.ActivityThread���main���������Լ��ǽ��̵���ں���������ʵ�ϣ��ڴ�֮ǰ�Ѿ����˴����Ĺ�����
---> �쳣�����е��� android.app.ActivityThread���main�����ͱ�ִ�� ---�� Looper.loop() �����������Ժ�Ϳ������������������Activity����Service�ˡ�



Process.startʵ�֣�frameworks/base/core/java/android/os/Process.java
ActivityThread.mainʵ�֣�frameworks/base/core/java/android/app/ActivityThread.java
ZygoteInit.runSelectLoopModeʵ�֣�frameworks/base/core/java/com/android/internal/os/ZygoteInit.java		
ZygoteConnection.handleChildProcʵ�֣�frameworks/base/core/java/com/android/internal/os/ZygoteConnection.java	
ActivityManagerService.startProcessLockedʵ�֣�frameworks/base/services/java/com/android/server/am/ActivityManagerService.java

RuntimeInit.zygoteInitʵ�֣�frameworks/base/core/java/com/android/internal/os/RuntimeInit.java
RuntimeInit.zygoteInitNative��һ��Native����, ʵ���� frameworks/base/core/jni/AndroidRuntime.cpp �ļ��У���ʵ���е� gCurRuntime ����ʵ�ʾ���
һ��AppRuntime�����AppRuntime�࣬������frameworks/base/cmds/app_process/app_main.cpp��
