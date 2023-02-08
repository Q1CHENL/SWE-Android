package com.tsunamisim.swe;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.tsunamisim.swe.R;

import java.io.File;

public class SWE1D extends AppCompatActivity {

    static {
        System.loadLibrary("tsunamiSim");
    }

    /** Function for the "Start" button
     * Launches the cpp code used to compute the SWE1D*/
    public void start(View view) {
        Spinner ss = findViewById(R.id.swe1d_scenario_spinner);
        EditText a = findViewById(R.id.swe1size);
        EditText b = findViewById(R.id.swe1dtime);
        String dirName = ((EditText) findViewById(R.id.swe1d_dirname)).getText().toString();
        int size, time;
        String scenarioName = ss.getSelectedItem().toString();
        File swe1d_dir = new File(Environment.getExternalStorageDirectory() + "/" + dirName);
        swe1d_dir.mkdirs();

        if (a.getText().toString().isEmpty() || b.getText().toString().isEmpty()) {
            Toast.makeText(this, "Please fill out all fields correctly!", Toast.LENGTH_SHORT).show();
        } else {
            size = Integer.parseInt(a.getText().toString());
            time = Integer.parseInt(b.getText().toString());
            String outPut = main(scenarioName, size, time, dirName);

            Intent intent = new Intent(view.getContext(), SWE1DOutput.class);
            intent.putExtra("swe1dOutput", outPut);
            startActivity(intent);
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
        setContentView(R.layout.acticity_swe1d);
        setTitle("SWE1D");

        Spinner scenarioSelector = (Spinner) findViewById(R.id.swe1d_scenario_spinner);
        String[] items = new String[]{
                "DamBreakScenario",
                "ShockShockScenario",
                "RareRareScenario",
                "SubcriticalScenario",
                "SupercriticalScenario"
        };
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, items);

        scenarioSelector.setSelection(0);
        scenarioSelector.setAdapter(adapter);
        scenarioSelector.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view,
                                       int position, long id) {
                ((TextView) parent.getChildAt(0)).setTextSize(19);
                Log.v("item", (String) parent.getItemAtPosition(position));
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
            /// Back button

        });

        /// Back button
        ActionBar a = getSupportActionBar();
        assert a != null;
        a.setDisplayHomeAsUpEnabled(true);
    }


    /**
     * A native method that is implemented by the 'tsunamiSim' native library,
     * which is packaged with this application.
     */
    public native String main(String scenarioName, int x, int time_step, String dirName);
}