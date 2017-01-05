package com.mikhailv.tttandroid;

import android.app.Application;
import android.widget.Toast;

/**
 * Created by MikhailV on 05.01.2017.
 */

public class TTTApplication extends Application
{
    @Override
    public void onCreate()
    {
        super.onCreate();
        Toast.makeText(getApplicationContext(), R.string.welcome_toast, Toast.LENGTH_LONG).show();
    }
}
