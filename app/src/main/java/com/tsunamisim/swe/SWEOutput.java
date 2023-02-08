package com.tsunamisim.swe;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.MenuItem;
import android.widget.TextView;

import com.tsunamisim.swe.R;

public class SWEOutput extends AppCompatActivity {

    private String text;

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
        setContentView(R.layout.activity_sweoutput);

        setTitle("SWE Output");
        /// Back button
        ActionBar a = getSupportActionBar();
        assert a != null;
        a.setDisplayHomeAsUpEnabled(true);

        text = getIntent().getStringExtra("swe");

        TextView textView = (TextView) findViewById(R.id.textView5);
        textView.setMovementMethod(new ScrollingMovementMethod());

        textView.setText(text);
    }
}