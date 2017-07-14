package com.nebula.androidneuronsimulator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TextView resultTextView;
    private Button runButton;
//    private MyTask task;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        resultTextView = (TextView) findViewById(R.id.output_text);
        runButton = (Button) findViewById(R.id.run_simulation);
//        task = new MyTask(resultTextView, runButton);

        findViewById(R.id.run_simulation).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                task.execute(1);
                new MyTask(resultTextView, runButton).execute(1);
            }
        });

    }
}
