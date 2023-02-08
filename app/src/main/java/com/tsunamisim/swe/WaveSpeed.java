package com.tsunamisim.swe;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.tsunamisim.swe.databinding.ActivityWavespeedBinding;

public class WaveSpeed extends AppCompatActivity {
    /// Loading the library
    static {
        System.loadLibrary("tsunamiSim");
    }

    /** Function for the "Get Wave Speeds" button
     * Launches the cpp code used to compute the wave speeds*/
    public void start(View view) {
        EditText a = (EditText) findViewById(R.id.e1);
        EditText b = (EditText) findViewById(R.id.e2);
        EditText c = (EditText) findViewById(R.id.e3);
        EditText d = (EditText) findViewById(R.id.e4);
        TextView text = (TextView) findViewById(R.id.textView);
        text.setText("");
        int w, x, y, z;


        if (a.getText().toString().isEmpty() || b.getText().toString().isEmpty() || c.getText().toString().isEmpty() || d.getText().toString().isEmpty()) {
            Toast.makeText(this, "Please fill out all fields with NUMBERS!", Toast.LENGTH_SHORT).show();
        } else {
            w = Integer.parseInt(a.getText().toString());
            x = Integer.parseInt(b.getText().toString());
            y = Integer.parseInt(c.getText().toString());
            z = Integer.parseInt(d.getText().toString());

            text.setText(main(w, x, y, z));
        }


    }
     /** It is a function needed to be implemented to have the back button in
      * the upper left corner*/
    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        this.finish();
        return super.onOptionsItemSelected(item);
    }

    /**Automatically generated function
     * It runs tasks that are written in it whenever the current View (a.k.a. the current screen)
     * is loaded*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        /// Screen title
        setTitle("Compute Wave Speeds");

        com.tsunamisim.swe.databinding.ActivityWavespeedBinding binding = ActivityWavespeedBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        /// Back button
        ActionBar a = getSupportActionBar();
        assert a != null;
        a.setDisplayHomeAsUpEnabled(true);

    }

    /**
     * A native method that is implemented by the 'tsunamiSim' native library,
     * which is packaged with this application.
     */

    public native String main(int a, int b, int c, int d);

}