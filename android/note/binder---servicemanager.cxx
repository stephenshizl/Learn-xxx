Դ�룺frameworks\native\cmds\servicemanager\service_manager.c

1��binger ��Ϊ�����֣�binder client��binder server��servicemanager

2��servicemanager ʵ���Ͼ���һ�� binder ����ֻ���������´������Ǹ� binder ����
   servicemanager �������������Server������Client�ṩ��ѯServerԶ�̽ӿڵĹ��ܡ�
   
3����ʼ��ʱ�Ļ�����������ͨ binder ����һ�������Ƕ���һ�����裺
   binder_become_context_manager(bs)
   ʹ�Լ��ɹ� binder �ػ��ߡ�   
   ��󣬾ͽ���һ������ѭ�����䵱Server�Ľ�ɫ���ȴ�Client������