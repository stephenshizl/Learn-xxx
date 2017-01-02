binder�ں�ʵ�֣�
������kernel\common\drivers\staging\android\binder.c
ʵ�֣�kernel\common\drivers\staging\android\binder.h


device_initcall(binder_init);
������ע��binder�豸��ret = misc_register(&binder_miscdev);
�����豸��Ϊ�ַ��豸�ķ�װ��Ϊ�ַ��豸�ṩ�ļ򵥵ı�̽ӿڡ�

binder_proc�ṹ��
	threads����������binder_proc���������ڴ����û�������̣߳��������������max_threads��������
	node������������binder_proc�����ڵ�Binderʵ��, ��binder_node->rb_node�γɵĺ�����ĸ���
	refs_by_desc����refs_by_node����������binder_proc�����ڵ�Binder���ã�
	�����õ��������̵�Binderʵ�壬���ֱ������ַ�ʽ����֯�������refs_by_desc���Ծ������keyֵ����֯��
	refs_by_node�������õ�ʵ��ڵ�ĵ�ֵַ����keyֵ����֯�����Ƕ��Ǳ�ʾͬһ��������
	ֻ������Ϊ���ڲ����ҷ�������������������ʾ��


�û��ռ�����binder_ioctl����ʱ���õ���
struct binder_write_read {
	signed long	write_size;	/* bytes to write */
	signed long	write_consumed;	/* bytes consumed by driver */
	unsigned long	write_buffer;
	signed long	read_size;	/* bytes to read */
	signed long	read_consumed;	/* bytes consumed by driver */
	unsigned long	read_buffer;
};
���У��������ݵĴ洢write_bufffer��read_buffer��ָ�����struct binder_transaction_data���������� 
union {
	size_t handle;
    void *ptr;
} target;
�ṹ��
���е�handle����ָҪʹ�õ�binder_node��desc, ͨ�����ֵ������binder_proc��refs_by_desc��������ҵ�Ҫʹ�õ�binderʵ�壬
ptr��ָ��binder_node�ĵ�ֵַ, ͨ�����ֵ������binder_proc��refs_by_node��������ҵ�Ҫʹ�õ�binderʵ�塣
ǰһ�֣��������û��ͻ�������binderʵ��Ĳ�������һ��������binder����˴�����ɺ����Ӧʱ����binderʵ�塣
(�ͻ�������ʱͨ��handler�ҵ���Ӧ��binderʵ�壬�ҵ�������̣�
�����Ӧ��ʱ��ͨ��binder_thread��transaction_stack�ṹ�б���������ߵ�binder_thread��Ϣ���ҵ�������)


binder_open��ÿ�ε��ö�������һ�� binder_proc �ṹ
binder_ioctl��ÿ�����ô˺������̶߳�������һ�� binder_thread������������ proc->threads.rb_node ���ڵ�ĺ����
binder_node��ÿ��binderʵ�嶼���Ӧһ�� binder_node��
	�ṹ���е�rb_node��dead_node���һ�������塣
	������Binderʵ�廹������ʹ�ã���ʹ��rb_node������proc->nodes����ʾ�ĺ��������ú����������֯����������̵�����Binderʵ�壻
	������Binderʵ�������Ľ����Ѿ����٣������Binderʵ���ֱ��������������ã������Binderʵ��ͨ��dead_node���뵽һ����ϣ����ȥ��š�
	proc��Ա�������Ǳ�ʾ���Binderʵ�������ڽ����ˡ�
	refs��Ա���������������˸�Binderʵ���Binder����������������һ������	
ÿ��binder_proc�и�todo���У�binder_proc�е�ÿ��binder_threadҲ��һ��todo���С�