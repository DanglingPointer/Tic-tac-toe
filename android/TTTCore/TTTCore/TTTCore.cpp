#include <mutex>
#include "TTTCore.h"
#include "TTTLogic.h"
//#include "old_logic.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TTTCore", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TTTCore", __VA_ARGS__))

/* This trivial function returns the platform ABI for which this dynamic native library is compiled.*/
const char * getPlatformABI()
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
	#define ABI "armeabi-v7a/NEON"
#else
	#define ABI "armeabi-v7a"
#endif
#else
	#define ABI "armeabi"
#endif
#elif defined(__i386__)
	#define ABI "x86"
#else
	#define ABI "unknown"
#endif
	LOGI("This dynamic shared library is compiled with ABI: %s", ABI);
	return "This native library is compiled with ABI: %s" ABI ".";
}

typedef std::lock_guard<std::mutex> lg_t;

std::mutex m_sync;
Gameplay<3> m_game;

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_aiTurn(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    m_game.AiTurn();
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_reset(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    m_game.Reset();
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setAiSideCross(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    m_game.AiSide = Mark::CROSS;
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setAiSideNought(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    m_game.AiSide = Mark::NOUGHT;
}

JNIEXPORT jint JNICALL Java_com_mikhailv_tttandroid_TTTCore_getAt(JNIEnv *, jobject, jint index)
{
    lg_t lock(m_sync);
    return (jint)m_game.pGrid->At(index);
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setCross(JNIEnv *, jobject, jint index)
{
    lg_t lock(m_sync);
    m_game.pGrid->SetCross(index);
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setNought(JNIEnv *, jobject, jint index)
{
    lg_t lock(m_sync);
    m_game.pGrid->SetNought(index);
}

JNIEXPORT jboolean JNICALL Java_com_mikhailv_tttandroid_TTTCore_isFilled(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    return (jboolean)m_game.pGrid->IsFilled();
}

JNIEXPORT jint JNICALL Java_com_mikhailv_tttandroid_TTTCore_isWon(JNIEnv *, jobject)
{
    lg_t lock(m_sync);
    return (jint)m_game.pGrid->IsWon();
}

JNIEXPORT jintArray JNICALL Java_com_mikhailv_tttandroid_TTTCore_getState(JNIEnv *penv, jclass)
{
    lg_t lock(m_sync);
    jintArray arr = penv->NewIntArray(m_game.pGrid->SIZE);
    jint *it = penv->GetIntArrayElements(arr, 0);
    for (int i = 0; i < m_game.pGrid->SIZE; ++i) {
        it[i] = (jint)m_game.pGrid->At(i);
    }
    return arr;
}

JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setState(JNIEnv *penv, jclass, jintArray arr)
{
    lg_t lock(m_sync);
    jsize len = penv->GetArrayLength(arr);
    jint *it = penv->GetIntArrayElements(arr, 0);
    for (int i = 0; i < len; ++i) {
        if (it[i] == 1)
            m_game.pGrid->SetCross(i);
        else if (it[i] == 2)
            m_game.pGrid->SetNought(i);
    }
    penv->ReleaseIntArrayElements(arr, it, JNI_ABORT); // do not copy back changes
}

