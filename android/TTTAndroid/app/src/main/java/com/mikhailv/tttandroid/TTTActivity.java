package com.mikhailv.tttandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Toast;

import java.util.Random;

public class TTTActivity extends Activity implements View.OnClickListener
{
    static final int EMPTY = 0;
    static final int CROSS = 1;
    static final int NOUGHT = 2;

    private static final String GRID_KEY = "TTTActivity.grid";
    private static final String PLAYER_KEY = "TTTActivity.crossPlayer";

    private ImageButton[] m_buttons;
    private Boolean m_crossPlayer;

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        getMenuInflater().inflate(R.menu.menu_ttt, menu);
        return super.onCreateOptionsMenu(menu);
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId()) {
            case R.id.menu_item_cross:
                TTTCore.reset();
                m_crossPlayer = true;
                TTTCore.setAiSideNought();
                updateAll();
                return true;
            case R.id.menu_item_nought:
                TTTCore.reset();
                m_crossPlayer = false;
                TTTCore.setAiSideCross();
                Random rand = new Random();
                TTTCore.setCross(rand.nextInt(9)); // first move random
                updateAll();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState); // calls onCreateOptionsMenu()
        setContentView(R.layout.activity_ttt);

        m_crossPlayer = null;
        m_buttons = new ImageButton[9];
        m_buttons[0] = (ImageButton)findViewById(R.id.imageButton0);
        m_buttons[1] = (ImageButton)findViewById(R.id.imageButton1);
        m_buttons[2] = (ImageButton)findViewById(R.id.imageButton2);
        m_buttons[3] = (ImageButton)findViewById(R.id.imageButton3);
        m_buttons[4] = (ImageButton)findViewById(R.id.imageButton4);
        m_buttons[5] = (ImageButton)findViewById(R.id.imageButton5);
        m_buttons[6] = (ImageButton)findViewById(R.id.imageButton6);
        m_buttons[7] = (ImageButton)findViewById(R.id.imageButton7);
        m_buttons[8] = (ImageButton)findViewById(R.id.imageButton8);

        for (ImageButton b : m_buttons) {
            b.setOnClickListener(this);
        }

        if (savedInstanceState != null && savedInstanceState.getIntArray(GRID_KEY) != null) {
            m_crossPlayer = savedInstanceState.getBoolean(PLAYER_KEY);
            int[] state = savedInstanceState.getIntArray(GRID_KEY);
            TTTCore.setState(state);

            if (m_crossPlayer) {
                TTTCore.setAiSideNought();
            } else {
                TTTCore.setAiSideCross();
            }
            updateAll();
        }
    }
    @Override
    protected void onSaveInstanceState(Bundle outState)
    {
        super.onSaveInstanceState(outState);
        if (m_crossPlayer != null) {
            int[] state = TTTCore.getState();
            outState.putBoolean(PLAYER_KEY, m_crossPlayer);
            outState.putIntArray(GRID_KEY, state);
        }
    }
    @Override
    public void onClick(View v)
    {
        int index = getButtonIndex(v);

        if (m_crossPlayer == null){
            Toast.makeText(getApplicationContext(), R.string.welcome_toast, Toast.LENGTH_SHORT)
                    .show();
            return;
        }
        if (TTTCore.getAt(index) != EMPTY || TTTCore.isWon() != EMPTY)
            return;

        if (m_crossPlayer)
            TTTCore.setCross(index);
        else
            TTTCore.setNought(index);

        if (!TTTCore.isFilled())
            TTTCore.aiTurn();
        updateAll();

        int winningSide = TTTCore.isWon();
        if (winningSide != EMPTY) { // impossible to win
            Toast.makeText(getApplicationContext(), R.string.loss_msg, Toast.LENGTH_SHORT).show();
        }
    }
    private int getButtonIndex(View button)
    {
        for (int i = 0; i < m_buttons.length; ++i) {
            if (button == m_buttons[i])
                return i;
        }
        throw new RuntimeException();
    }
    private void updateAll()
    {
        for (int i = 0; i < 9; ++i) {
            int what = TTTCore.getAt(i);
            if (what == EMPTY)
                m_buttons[i].setImageResource(0);
            else if (what == CROSS)
                m_buttons[i].setImageResource(R.drawable.ic_cross);
            else if (what == NOUGHT)
                m_buttons[i].setImageResource(R.drawable.ic_nought);
        }
    }
}
