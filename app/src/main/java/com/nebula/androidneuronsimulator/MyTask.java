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
        try {
            Thread.sleep((Integer)value[0]*1000);
            result_text = runSimulation();
        } catch (InterruptedException e){
        }
        return result_text;
    }

    @Override
    protected void onPostExecute(String result){
        resultTextView.append(result);
        resultTextView.append("finish simulation\n");
        runButton.setEnabled(true);
    }
}

