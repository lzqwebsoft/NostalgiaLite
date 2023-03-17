#include <jni.h>
#include "Emulator.h"
#include "Bridge.h"

extern "C" {
using namespace emudroid;

Emulator *emu;

#ifndef BRIDGE_PACKAGE
#define BRIDGE_PACKAGE :-)
#endif

Bridge::Bridge(Emulator *emulator) {
    emu = emulator;
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(start)(JNIEnv *env, jobject obj, jint gfx, jint sfx,
                      jint general) {
    return (jboolean) emu->start(gfx, sfx, general);
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(readPalette)(JNIEnv *env, jobject obj, jintArray result) {
    return (jboolean) emu->readPalette(env, result);
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(loadGame)(JNIEnv *env, jobject obj, jstring path,
                         jstring batteryPath, jstring batteryFullPath) {
    jboolean isCopy;
    jboolean isCopy2;
    jboolean isCopy3;
    const char *fname = env->GetStringUTFChars(path, &isCopy);
    const char *fbattery = env->GetStringUTFChars(batteryPath, &isCopy2);
    const char *fbatteryFullPath = env->GetStringUTFChars(batteryFullPath, &isCopy3);
    bool success = emu->loadGame(fname, fbattery, fbatteryFullPath);
    env->ReleaseStringUTFChars(path, fname);
    env->ReleaseStringUTFChars(batteryPath, fbattery);
    env->ReleaseStringUTFChars(batteryFullPath, fbatteryFullPath);
    return success;
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(setBaseDir)(JNIEnv *env, jobject obj, jstring path) {
    jboolean isCopy;
    const char *fname = env->GetStringUTFChars(path, &isCopy);
    bool success = emu->setBaseDir(fname);
    env->ReleaseStringUTFChars(path, fname);
    return success;
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(enableCheat)(JNIEnv *env, jobject obj, jstring gg,
                            jint type) {
    jboolean isCopy;
    const char *cheat = env->GetStringUTFChars(gg, &isCopy);
    bool success = emu->enableCheat(cheat, type);
    env->ReleaseStringUTFChars(gg, cheat);
    return success;
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(enableRawCheat)(JNIEnv *env, jobject obj, jint addr, jint val, jint comp) {
    jboolean isCopy;
    bool success = emu->enableRawCheat(addr, val, comp);
    return success;
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(emulate)(JNIEnv *env, jobject obj, jint keys,
                        jint turbos, jint numFramesToSkip) {
    int res = emu->emulateFrame(keys, turbos, numFramesToSkip);
    return res;
}

JNIEXPORT jboolean
JNICALL BRIDGE_PACKAGE(render)(JNIEnv *env, jobject obj, jobject bitmap) {
    return emu->render(env, bitmap, -1, -1, NULL);
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(renderVP)(JNIEnv *env, jobject obj, jobject bitmap, int w, int h) {
    return emu->render(env, bitmap, w, h, NULL);
}


JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(renderGL)(JNIEnv *env, jobject obj) {
    return emu->renderGL();
}


JNIEXPORT jint JNICALL
BRIDGE_PACKAGE(getHistoryItemCount)(JNIEnv *env, jobject obj) {
    return emu->getHistoryItemCount();
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(loadHistoryState)(JNIEnv *env, jobject obj, int pos) {
    return emu->loadHistoryState(pos);
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(renderHistory)(JNIEnv *env, jobject obj, jobject bmp, int pos, int w, int h) {
    return emu->renderHistory(env, bmp, pos, w, h);
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(setViewPortSize)(JNIEnv *env, jobject obj, jint w, jint h) {
    return emu->setViewPortSize(w, h);
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(fireZapper)(JNIEnv *env, jobject obj, jint x, jint y) {
    return emu->fireZapper(x, y);
}


JNIEXPORT jint JNICALL
BRIDGE_PACKAGE(readSfxBuffer)(JNIEnv *env, jobject obj, jshortArray data) {
    return emu->readSfxBuffer(env, obj, data);
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(loadState)(JNIEnv *env, jobject obj, jstring path, int slot) {
    jboolean isCopy;
    const char *fname = env->GetStringUTFChars(path, &isCopy);
    bool success = emu->loadState(fname, slot);
    env->ReleaseStringUTFChars(path, fname);
    return success;
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(saveState)(JNIEnv *env, jobject obj, jstring path, int slot) {
    jboolean isCopy;
    const char *fname = env->GetStringUTFChars(path, &isCopy);
    bool success = emu->saveState(fname, slot);
    env->ReleaseStringUTFChars(path, fname);
    return success;
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(reset)(JNIEnv *env, jobject obj, jstring path) {
    return emu->reset();
}

JNIEXPORT jboolean JNICALL
BRIDGE_PACKAGE(stop)(JNIEnv *env, jobject obj) {
    return emu->stop();
}

}

static JavaVM *gJvm = nullptr;

static jmethodID openContentUri;

static jobject nativeActivity;
static jobject g_nativeActivity;

JNIEnv *getEnv() {
    JNIEnv *env;
    int status = gJvm->GetEnv((void **) &env, JNI_VERSION_1_6);
//    _assert_msg_(status >= 0, "'%s': Can only call getEnv if you've attached the thread already!", GetCurrentThreadName());
    return env;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pjvm, void *reserved) {
    gJvm = pjvm;  // cache the JavaVM pointer
    auto env = getEnv();
    return JNI_VERSION_1_6;
}

void Android_StorageSetNativeActivity(jobject nativeActivity) {
    g_nativeActivity = nativeActivity;
}

extern "C"
JNIEXPORT void JNICALL
Java_nostalgia_appnes_NesEmulatorActivity_registerCallbacks(JNIEnv *env, jobject obj) {
    nativeActivity = env->NewGlobalRef(obj);
    openContentUri = env->GetMethodID(env->GetObjectClass(obj), "openContentUri", "(Ljava/lang/String;Ljava/lang/String;)I");
    Android_StorageSetNativeActivity(nativeActivity);
}


int Android_OpenContentUriFd(const char *fname, const char *mode) {
    if (!g_nativeActivity) {
        return -1;
    }
    auto env = getEnv();
    const char *modeStr = "";
    if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
        modeStr = "r";
    } else if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
        modeStr = "w";
    } else if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0) {
        modeStr = "wa";
    } else {
        modeStr = "rw";
    }
    jstring j_filename = env->NewStringUTF(fname);
    jstring j_mode = env->NewStringUTF(modeStr);
    // j_mode must be "r", "w", "wt", "wa", "rw" or "rwt".
    int fd = env->CallIntMethod(g_nativeActivity, openContentUri, j_filename, j_mode);
    return fd;
}