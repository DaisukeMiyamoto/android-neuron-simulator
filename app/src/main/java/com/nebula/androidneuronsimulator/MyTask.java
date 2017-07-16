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
    public native String checkOMP();
    public native String runBenchmarkTriad(int num_threads);
    public native String runBenchmarkHH();


    public MyTask(TextView textView, Button button) {
        super();
        this.resultTextView = textView;
        this.runButton = button;
    }

    @Override
    protected void onPreExecute() {
        resultTextView.append("Start Benchmarks\n");
        runButton.setEnabled(false);
    }

    @Override
    protected String doInBackground(Object... value) {
        String result_text = "";
        long start_time;
        long stop_time;
        double calc_time;

        start_time = System.currentTimeMillis();
        result_text += "[Conditions]\n";
        result_text += checkOMP();
        result_text += "\n[TRIAD]\n";
        result_text += runBenchmarkTriad(1);
        result_text += runBenchmarkTriad(4);
        result_text += "\n[Hodgkin-Huxley]\n";
        result_text += runBenchmarkHH();
        stop_time = System.currentTimeMillis();

        calc_time = (stop_time - start_time)/1000.0;
        return result_text + "\nTOTAL TIME: " + Double.toString(calc_time) + "\n";
    }

    @Override
    protected void onPostExecute(String result){
        resultTextView.append(result);
        resultTextView.append("Finish Benchmarks\n");
        runButton.setEnabled(true);
    }
}

