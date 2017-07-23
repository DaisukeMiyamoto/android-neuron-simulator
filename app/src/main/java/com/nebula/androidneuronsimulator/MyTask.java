package com.nebula.androidneuronsimulator;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.widget.Button;
import android.widget.TextView;

/**
 * AsyncTask for benchmark
 * Created by nebula on 7/14/17.
 */

final class MyTask extends AsyncTask<Object, Integer, String> implements DialogInterface.OnCancelListener {
    private TextView resultTextView;
    private Button runButton;
    private Context context;
    private ProgressDialog dialog;
    private int numTask;
    private int processors;

    static {
        System.loadLibrary("native-lib");
    }
    public native String checkOMP();
    public native String getCpuFeatures();
    public native String runBenchmarkDaxpy(int num_threads);
    public native String runBenchmarkIz(int num_threads);
    public native String runBenchmarkHH(int num_threads);


    MyTask(Context context, TextView textView, Button button) {
        super();
        this.resultTextView = textView;
        this.runButton = button;
        this.context = context;
        this.numTask = 9;
        this.processors = Runtime.getRuntime().availableProcessors();
        if (processors > 4){
            this.numTask += 3;
        }
    }

    @Override
    protected void onPreExecute() {

        resultTextView.setText("Start Benchmarks\n");
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
        result_text += getCpuFeatures();
        result_text += checkOMP();

        result_text += "\n[DAXPY]\n";
        result_text += runBenchmarkDaxpy(1);
        publishProgress(++task_finished);
        result_text += runBenchmarkDaxpy(2);
        publishProgress(++task_finished);
        result_text += runBenchmarkDaxpy(4);
        publishProgress(++task_finished);
        if (processors > 4) {
            result_text += runBenchmarkDaxpy(0);
            publishProgress(++task_finished);
        }

        result_text += "\n[Izhikevich]\n";
        result_text += runBenchmarkIz(1);
        publishProgress(++task_finished);
        result_text += runBenchmarkIz(2);
        publishProgress(++task_finished);
        result_text += runBenchmarkIz(4);
        publishProgress(++task_finished);
        if (processors > 4) {
            result_text += runBenchmarkIz(0);
            publishProgress(++task_finished);
        }

        result_text += "\n[Hodgkin-Huxley (euler, notable)]\n";
        result_text += runBenchmarkHH(1);
        publishProgress(++task_finished);
        result_text += runBenchmarkHH(2);
        publishProgress(++task_finished);
        result_text += runBenchmarkHH(4);
        publishProgress(++task_finished);
        if (processors > 4) {
            result_text += runBenchmarkHH(0);
            publishProgress(++task_finished);
        }

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
        resultTextView.append("Benchmark Canceled\n");
        runButton.setEnabled(true);
        cancel(true);
    }
}

