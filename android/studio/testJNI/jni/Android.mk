##############################################################################
#
# A��jni �У�C/C++ �� Java ֮��Ĺ�������ͨ������ʵ�ֵģ�
# B������ C/C++ ����ͨ�� env ���½���õ����е� Java �����/��Ա���������䷽����
# C������ Java ���޷��õ� C++ �Ķ���ֻ��ͨ���� C++ ����󶨵� Java ��������
#    C++ �л�ȡ��Ӧ�� Java ���󣬽�����ȡ C++ �����ַ��ǿתΪ��Ӧ����Ȼ��ʹ�á�
#
# �ڵ�ǰĿ¼����ndk�е�ndk-build.cmd���������Ϊ(���ִ�Сд)��Android.mk
#
# ����so�ļ�ʱ�����ܴ�libǰ׺��.so��׺���磺System.loadLibrary("TestJni");
#
# ��jni��ʹ��android��log�������Ҫ��ӣ�
#		a��mk�����ӣ�LOCAL_LDLIBS += -llog
#		b��c/c++������ #include <android/log.h> (Ŀ¼���̶�������ndk������log.h��ȷ��)
#       c������ʹ�÷����ο���android/log.h �ļ�
#
# ����X86ƽ̨��
# APP_ABI 		:= x86
# ʹ��android-9���ϵ�ƽ̨��
# APP_PLATFORM 	:= android-9
#
##############################################################################

# C/C++��������Ŀ¼��Ҳ�������ǵ�jniĿ¼
LOCAL_PATH		:= $(call my-dir)

# һ������ģ�����
include $(CLEAR_VARS)

# �����Դ�ļ�
LOCAL_SRC_FILES	:= \
	com_example_testjni_JniFooA.c \
	nativefun.cpp \
	JniTypeUtils.cpp \
	JniFooB.cpp

# �������ɵ�Ŀ�����
LOCAL_MODULE 	:= libTestJni

LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)

# ��־ʹ��
LOCAL_LDLIBS += -llog

# ����ʱ��Ҫ���ⲿ��
LOCAL_SHARED_LIBRARIES := libutils

LOCAL_PRELINK_MODULE 	:= false
LOCAL_MODULE_TAGS 		:=optional

# ָ��Ҫ����ɶ�̬��
include $(BUILD_SHARED_LIBRARY)

# Ϊ��ʹ��std(��ʵ����Ȼ����ʹ��std::string����Ҫ������������)
APP_CFLAGS += -fexceptions  
APP_STL := gnustl_static  
