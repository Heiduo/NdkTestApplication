#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
//
// Created by hengaigaoke on 2020/5/26.
//

//定义打印宏
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"JNI",__VA_ARGS__);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ndktestapplication_NDKHelper_getNDKPrint(JNIEnv *env, jclass clazz) {
    std::string hello = "my Custom JNI Function";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ndktestapplication_NDKHelper_sendIntArrayToJNI(JNIEnv *env, jclass thiz,
                                                                jintArray int_array) {
    jint *javaArray = env->GetIntArrayElements(int_array, NULL);

    int32_t length = env->GetArrayLength(int_array);
    for (int k = 0; k < length; k++) {
        int tmp = *(javaArray + k);
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "数据的值：%d", tmp);
        //对值做修改
        *(javaArray + k) = tmp + 10;
    }
    //将在JNI里做的修改传至JAVA层
    /*
     * 0:  刷新java数组 并 释放c/c++数组
       1 = JNI_COMMIT:
       只刷新java数组
       2 = JNI_ABORT
       只释放*/
    env->ReleaseIntArrayElements(int_array, javaArray, 0);
    std::string hello = "测试数组传递";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ndktestapplication_NDKHelper_sendStringArrayToJNI(JNIEnv *env, jclass clazz,
                                                                   jobjectArray string_array) {
    jint strLength = env->GetArrayLength(string_array);
    for (int i = 0; i < strLength; i++) {
        jstring str = static_cast<jstring>(env->GetObjectArrayElement(string_array, i));
        const char *s = env->GetStringUTFChars(str, NULL);
        LOGE("获取java的参数:%s", s);
    }
    std::string hello = "测试字符串数组传递";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ndktestapplication_NDKHelper_parseUser(JNIEnv *env, jclass clazz, jobject user) {
    jclass beanCls = env->GetObjectClass(user);

    //调用getName
    jmethodID jniGetName = env->GetMethodID(beanCls, "getName", "()Ljava/lang/String;");
    jstring nameValue = static_cast<jstring>(env->CallObjectMethod(user, jniGetName));
    const char *s = env->GetStringUTFChars(nameValue, NULL);
    LOGE("getName()=:%s", s);

    //调用setName
    jmethodID jniSetName = env->GetMethodID(beanCls, "setName", "(Ljava/lang/String;)V");
    jstring nameStr = env->NewStringUTF("嘿哆");
    env->CallVoidMethod(user, jniSetName, nameStr);

    //调用静态方法printInfo
    jmethodID printInfo = env->GetStaticMethodID(beanCls, "printInfo", "()V");
    env->CallStaticVoidMethod(beanCls, printInfo);

    //获取成员属性name
    jfieldID fid = env->GetFieldID(beanCls, "name", "Ljava/lang/String;");
    jstring fieldNameStr = static_cast<jstring>(env->GetObjectField(user, fid));

    const char *cNameStr = env->GetStringUTFChars(fieldNameStr, NULL);
    LOGE("属性name = :%s", cNameStr);

    //修改属性
    jstring changeNameStr = env->NewStringUTF("修改属性name");
    env->SetObjectField(user, fid, changeNameStr);

    std::string hello = "JNI 解析 实体类";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndktestapplication_NDKHelper_createBean(JNIEnv *env, jclass clazz) {
    jclass cld = env->FindClass("com/example/ndktestapplication/Bean");
    //获取构造函数
    jmethodID construct = env->GetMethodID(cld, "<init>", "(Ljava/lang/String;)V");
    //利用class与MethodId创建Java对象
    jstring nameStr = env->NewStringUTF("JNI 创建了 Bean 对象");
    jobject bean = env->NewObject(cld, construct, nameStr);

    jmethodID jniCall = env->GetMethodID(cld, "jniCall", "()V");
    env->CallVoidMethod(bean, jniCall);
}


JavaVM *_vm;

int JNI_OnLoad(JavaVM *vm, void *re) {
    _vm = vm;
    return JNI_VERSION_1_2;
}

void *threadTask(void *args) {
    //使用子线程env
    JNIEnv *env;
    jint i = _vm->AttachCurrentThread(&env, 0);
    if (i != JNI_OK) {
        return 0;
    }

    //假设，到此下载文件完成，该通知MainActivity了
    //获得MainActivity的class对象
    jclass cls = env->GetObjectClass(static_cast<jobject>(args));

    // 反射获得方法
    jmethodID updateUI = env->GetMethodID(cls,"updateUI", "()V");
    env->CallVoidMethod(static_cast<jobject>(args), updateUI);

    //释放全局变量
    env->DeleteGlobalRef(static_cast<jobject>(args));
    //分离
    _vm->DetachCurrentThread();
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndktestapplication_MainActivity_downloadThread(JNIEnv *env, jclass clazz,jobject activity) {
    // TODO: implement downloadThread()
    pthread_t pid;

    //变为全局变量
    jobject globalClazz = env->NewGlobalRef(activity);

    //启动线程
    pthread_create(&pid, NULL, threadTask, globalClazz);
}






