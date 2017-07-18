package com.nebula.androidneuronsimulator;

import android.os.Build;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/**
 * get various type of information from Android Device
 * Created by nebula on 7/17/17.
 */

class MachineInfo {

    static String getBuildInfo() {
        String str = "";
        str += "Brand: " + Build.BRAND + "\n";
        str += "Manufacture: " + Build.MANUFACTURER + "\n";
        str += "Device: " + Build.DEVICE + "\n";
        str += "Model: " + Build.MODEL + "\n";
        str += "Codename: " + Build.VERSION.CODENAME + "\n";
        str += "Release: " + Build.VERSION.RELEASE + "\n";
        str += "\n";
        return str;
    }

    static String getAvailableProcessors() {
        int core = Runtime.getRuntime().availableProcessors();
        return "Processors: " + Integer.toString(core) + "\n\n";
    }

    static String getCpuInfo() {

        String str = "";
        try {
            BufferedReader reader = new BufferedReader(new FileReader("/proc/cpuinfo"));
            String line;
            while((line = reader.readLine()) != null) {
                str += line + "\n";
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return str;
    }
}
