package com.mikhailv.tttandroid;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Button;
import android.widget.Toast;

import java.util.Random;

public class TTTActivity extends Activity implements View.OnClickListener
{
    static final int EMPTY = 0;
    static final int CROSS = 1;
    static final int NOUGHT = 2;

    private static final String GRID_KEY = "grid";
    private static final String PLAYER_KEY = "crossPlayer";

    private ImageButton[] m_buttons;

    private Boolean m_crossPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
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

        final Button crossBtn = (Button)findViewById(R.id.crossButton);
        final Button noughtBtn = (Button)findViewById(R.id.noughtButton);

        crossBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                m_crossPlayer = true;
                setButtonColor(crossBtn, noughtBtn);

                TTTCore.reset();
                TTTCore.setAiSideNought();
                updateAll();
            }
        });

        noughtBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                m_crossPlayer = false;
                setButtonColor(noughtBtn, crossBtn);

                TTTCore.reset();
                TTTCore.setAiSideCross();

                Random rand = new Random();
                TTTCore.setCross(rand.nextInt(9)); // first move random
                updateAll();
            }
        });

        if (savedInstanceState != null && savedInstanceState.getIntArray(GRID_KEY) != null) {
            m_crossPlayer = savedInstanceState.getBoolean(PLAYER_KEY);
            int[] state = savedInstanceState.getIntArray(GRID_KEY);
            TTTCore.setState(state);

            if (m_crossPlayer) {
                setButtonColor(crossBtn, noughtBtn);
                TTTCore.setAiSideNought();
            } else {
                setButtonColor(noughtBtn, crossBtn);
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

        if (TTTCore.getAt(index) != EMPTY || m_crossPlayer == null || TTTCore.isWon() != EMPTY)
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
            Toast.makeText(getApplicationContext(), "You lost!", Toast.LENGTH_SHORT).show();
        }
    }
    private void setButtonColor(Button selected, Button unselected)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            selected.setBackgroundColor(getColor(R.color.highlightedButton));
            unselected.setBackgroundColor(getColor(R.color.buttonGrey));
        } else {
            selected.setBackgroundColor(getResources().getColor(R.color.highlightedButton));
            unselected.setBackgroundColor(getResources().getColor(R.color.buttonGrey));
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
