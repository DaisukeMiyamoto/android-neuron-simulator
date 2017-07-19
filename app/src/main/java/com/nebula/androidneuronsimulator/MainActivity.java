package com.nebula.androidneuronsimulator;

import android.app.Activity;
//import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

//public class MainActivity extends AppCompatActivity {
public class MainActivity extends Activity {
    private TextView resultTextView;
    private Button runButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        resultTextView = (TextView) findViewById(R.id.output_text);
        runButton = (Button) findViewById(R.id.run_simulation);

        findViewById(R.id.run_simulation).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new MyTask(MainActivity.this, resultTextView, runButton).execute(1);
            }
        });

        findViewById(R.id.get_info).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resultTextView.setText(MachineInfo.getBuildInfo());
                resultTextView.append(MachineInfo.getAvailableProcessors());
                resultTextView.append(MachineInfo.getCpuInfo());
            }
        });
    }
}
