Ashmem ���������ڴ���ϵͳ��Anonymous Shared Memory��
    �������������������ʽʵ�����ں˿ռ�ģ�ͬʱ����ϵͳ����ʱ����Ӧ�ó����ܲ��ṩ�˷��ʽӿڣ�
���У���ϵͳ����ʱ����ṩ��C/C++���ýӿڣ�����Ӧ�ó����ܲ��ṩ��Java���ýӿڡ�
    ���������ص�:
        һ���ܹ������ڴ����ϵͳ����Ч�ع�����ʹ�õ��ڴ��
        ������ͨ��Binder���̼�ͨ�Ż�����ʵ�ֽ��̼���ڴ湲��


��AndroidӦ�ó����ܲ㣬�ṩ��һ��MemoryFile�ӿ�����װ�����������ڴ��ļ��Ĵ�����ʹ�ã�
��ʵ����frameworks/base/core/java/android/os/MemoryFile.java�ļ���


Binder���̼�ͨ�Žӿڶ�����src/shy/luo/ashmem/IMemoryService.java

Ashmem��������ʵ����kernel/common/mm/ashmem.c�ļ���
JNI����native_openʵ����frameworks/base/core/jni/adroid_os_MemoryFile.cpp
ashmem_create_region���������������ڴ棬�������ʵ����system/core/libcutils/ashmem-dev.c�ļ���


Ashmem������������ô�������ڴ����ϵͳ����Ч�����ڴ��
�� ashmem_init ��ͨ������register_shrinker�������ڴ����ϵͳע��һ���ڴ�����㷨����
��Linux�ں��У���ϵͳ�ڴ����ʱ���ڴ����ϵͳ�ͻ�����ڴ�����㷨��
��ˣ����ڴ����ϵͳ�����ڴ����ʱ���ͻ���õ������ashmem_shrink��������Ashmem��������ִ���ڴ���ղ���

Ashmem��ͨ��Binder����ʵ�ֽ��̼乲��ģ�����ľ���ashmem�������ļ��������������
����Ĺؼ��ǣ�struct file�ṹ������ʾһ�����ļ��ṹ����Linuxϵͳ�У����ļ���
��struct file�ǿ����ڽ��̼乲��ģ������ļ���������һ����