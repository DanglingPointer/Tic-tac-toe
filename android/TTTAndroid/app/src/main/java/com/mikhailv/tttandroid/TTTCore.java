package com.mikhailv.tttandroid;

/**
 * Created by MikhailV on 30.09.2016.
 */

public class TTTCore
{
    public static native void aiTurn();
    public static native void reset();

    public static native void setAiSideCross();
    public static native void setAiSideNought();

    // 0 = empty, 1 = cross, 2 = nought
    public static native int getAt(int index);
    public static native void setCross(int index);
    public static native void setNought(int index);
    public static native boolean isFilled();
    public static native int isWon();

    public static native int[] getState();
    public static native void setState(int[] state);

    static {
        System.loadLibrary("TTTCore");
    }
}
