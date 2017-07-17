package com.nebula.androidneuronsimulator;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by nebula on 7/14/17.
 */

final class MyTask extends AsyncTask<Object, Integer, String> implements DialogInterface.OnCancelListener {
    private TextView resultTextView;
    private Button runButton;
    private Context context;
    private ProgressDialog dialog;
    private int numTask;

    static {
        System.loadLibrary("native-lib");
    }
    public native String checkOMP();
    public native String runBenchmarkTriad(int num_threads);
    public native String runBenchmarkHH(int num_threads);


    public MyTask(Context context, TextView textView, Button button) {
        super();
        this.resultTextView = textView;
        this.runButton = button;
        this.context = context;
        this.numTask = 6;
    }

    @Override
    protected void onPreExecute() {

        resultTextView.append("Start Benchmarks\n");
        runButton.setEnabled(false);
        dialog = new ProgressDialog(context);
        dialog.setTitle("Processing...");
        dialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        dialog.setMax(numTask);
        dialog.setProgress(0);
        dialog.setCancelable(true);
        dialog.setOnCancelListener(this);
        dialog.show();
    }

    @Override
    protected String doInBackground(Object... value) {
        String result_text = "";
        long start_time;
        long stop_time;
        double calc_time;
        int task_finished = 0;

        start_time = System.currentTimeMillis();

        result_text += "[Conditions]\n";
        result_text += checkOMP();

        result_text += "\n[TRIAD]\n";
        result_text += runBenchmarkTriad(1);
        publishProgress(++task_finished);
        result_text += runBenchmarkTriad(2);
        publishProgress(++task_finished);
        result_text += runBenchmarkTriad(0);
        publishProgress(++task_finished);

        result_text += "\n[Hodgkin-Huxley (euler, notable)]\n";
        result_text += runBenchmarkHH(1);
        publishProgress(++task_finished);
        result_text += runBenchmarkHH(2);
        publishProgress(++task_finished);
        result_text += runBenchmarkHH(0);
        publishProgress(++task_finished);

        stop_time = System.currentTimeMillis();
        calc_time = (stop_time - start_time)/1000.0;

        return result_text + "\nTOTAL TIME: " + Double.toString(calc_time) + "\n";
    }

    @Override
    protected void onProgressUpdate(Integer... progress) {
        dialog.setProgress(progress[0]);
    }

    @Override
    protected void onPostExecute(String result){
        resultTextView.append(result);
        resultTextView.append("Finish Benchmarks\n");
        runButton.setEnabled(true);

        if(dialog != null && dialog.isShowing()) {
            dialog.dismiss();
        }
    }

    @Override
    protected void onCancelled() {
        dialog.dismiss();
    }

    @Override
    public void onCancel(DialogInterface idialog){
        runButton.setEnabled(true);
        cancel(true);
    }
}

