package com.nebula.androidneuronsimulator;

import android.os.AsyncTask;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by nebula on 7/14/17.
 */

public class MyTask extends AsyncTask<Object, Object, String> {
    private TextView resultTextView;
    private Button runButton;

    static {
        System.loadLibrary("native-lib");
    }
    public native String runSimulation();


    public MyTask(TextView textView, Button button) {
        super();
        this.resultTextView = textView;
        this.runButton = button;
    }

    @Override
    protected void onPreExecute() {
        resultTextView.append("start simulation\n");
        runButton.setEnabled(false);
    }

    @Override
    protected String doInBackground(Object... value) {
        String result_text = "";
        long start_time;
        long stop_time;
        double calc_time, mflops, flop=2;
        long loop_max = 100000000; // TODO: get loop_max from java

        start_time = System.currentTimeMillis();
        result_text = runSimulation();
        stop_time = System.currentTimeMillis();

        calc_time = (stop_time - start_time)/1000.0;
        mflops = flop * loop_max / calc_time / 1000.0 / 1000.0;
        return result_text + "MULADD: " + String.format("%.3f", mflops) + " MFLOPS\n";
    }

    @Override
    protected void onPostExecute(String result){
        resultTextView.append(result);
        resultTextView.append("finish simulation\n");
        runButton.setEnabled(true);
    }
}

