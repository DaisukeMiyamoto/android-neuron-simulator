package com.nebula.androidneuronsimulator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromRun();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.run_simulation).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                runSimulation(v);
            }
        });

    }

    private void runSimulation(View v) {
        TextView tv = (TextView) findViewById(R.id.output_text);
        tv.append(stringFromRun());
    }

}
