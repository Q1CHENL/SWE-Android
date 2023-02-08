package com.tsunamisim.swe;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class HomeScreen extends AppCompatActivity {

    /**Function that takes you to the "Compute Wave Speeds" Screen*/
    public void computeWaveSpeeds(View view) {
        Intent intent = new Intent(this, WaveSpeed.class);
        startActivity(intent);
    }

    /**Function that takes you to the "SWE1D" Screen*/
    public void SWE1D(View view) {
        Intent intent = new Intent(this, SWE1D.class);
        startActivity(intent);
    }
    /**Function that takes you to the "SWE" Screen*/
    public void SWE(View view) {
        Intent intent = new Intent(this, SWE.class);
        startActivity(intent);
    }

    /**Automatically generated function
     * It runs tasks that are written in it whenever the current View (a.k.a. the current screen)
     * is loaded*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_screen);

        setTitle("Tsunami Simulation");
        ImageView imageView = (ImageView) findViewById(R.id.imageView);
        imageView.setImageResource(R.drawable.the_great_wave_off_kanagawa_7107112_1280);
        
    }
}