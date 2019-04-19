package com.jefshe.litmusic

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.github.mikephil.charting.charts.BarChart
import com.github.mikephil.charting.data.BarData
import com.github.mikephil.charting.data.BarDataSet
import com.github.mikephil.charting.data.BarEntry
import android.content.pm.PackageManager
import android.Manifest.permission
import android.Manifest.permission.RECORD_AUDIO
import androidx.core.app.ActivityCompat




class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("native-lib");
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val chart = findViewById(R.id.chart) as BarChart
        val data = BarDataSet(listOf(BarEntry(1.2f, 1.4f)), "Spectrum")
        chart.setData(BarData(data));
        chart.invalidate(); // refresh
    }

    public override fun onResume() {
        // Check we have the record permission
        if (isRecordPermissionGranted())
            startEngine()
        else
            requestRecordPermission()
        super.onResume()
    }

    private fun requestRecordPermission() {
        ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.RECORD_AUDIO),0)
    }

    private fun isRecordPermissionGranted(): Boolean {
        return ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
    }

    external fun setRecording(isRecording: Boolean)
    external fun startEngine()
    external fun stopEngine()

}
