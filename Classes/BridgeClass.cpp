//
// Created by Ashok Choudhary on 07/04/24.
//

#include <jni.h>
#include "BridgeClass.h"
#include "android/jni/JniHelper.h"

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void BridgeClass::showToast(const char* message) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    if (env) {
        jclass classID = env->FindClass("org/cocos2dx/cpp/AppActivity");
        if (classID) {
            jmethodID methodID = env->GetStaticMethodID(classID, "showToast", "(Ljava/lang/String;)V");
            if (methodID) {
                jstring jmsg = env->NewStringUTF(message);
                env->CallStaticVoidMethod(classID, methodID, jmsg);
                env->DeleteLocalRef(jmsg);
            }
        }
    }
#endif
}