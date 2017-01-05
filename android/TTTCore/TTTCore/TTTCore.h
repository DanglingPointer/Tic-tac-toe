#pragma once
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    aiTurn
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_aiTurn(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    reset
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_reset(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    setAiSideCross
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setAiSideCross(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    setAiSideNought
    * Signature: ()V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setAiSideNought(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    getAt
    * Signature: (I)I
    */
    // 0 = empty, 1 = cross, 2 = nought
    JNIEXPORT jint JNICALL Java_com_mikhailv_tttandroid_TTTCore_getAt(JNIEnv *, jobject, jint);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    setCross
    * Signature: (I)V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setCross(JNIEnv *, jobject, jint);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    setNought
    * Signature: (I)V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setNought(JNIEnv *, jobject, jint);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    isFilled
    * Signature: ()Z
    */
    JNIEXPORT jboolean JNICALL Java_com_mikhailv_tttandroid_TTTCore_isFilled(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    isWon
    * Signature: ()Z
    */
    JNIEXPORT jint JNICALL Java_com_mikhailv_tttandroid_TTTCore_isWon(JNIEnv *, jobject);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    getState
    * Signature: ()[I
    */
    JNIEXPORT jintArray JNICALL Java_com_mikhailv_tttandroid_TTTCore_getState
    (JNIEnv *, jclass);

    /*
    * Class:     com_mikhailv_tttandroid_TTTCore
    * Method:    setState
    * Signature: ([I)V
    */
    JNIEXPORT void JNICALL Java_com_mikhailv_tttandroid_TTTCore_setState
    (JNIEnv *, jclass, jintArray);

#ifdef __cplusplus
}
#endif