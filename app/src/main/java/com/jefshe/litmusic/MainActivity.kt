package com.jefshe.litmusic

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.github.mikephil.charting.charts.BarChart
import com.github.mikephil.charting.data.BarData
import com.github.mikephil.charting.data.BarDataSet
import com.github.mikephil.charting.data.BarEntry
import android.content.pm.PackageManager
import android.Manifest.permission.RECORD_AUDIO
import android.content.Context
import android.graphics.Color
import android.media.AudioManager
import android.view.View
import android.widget.Button
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.content.ContextCompat.getColorStateList


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
        if(isRecordPermissionGranted())
            requestRecordPermission()
        val audio = getSystemService(Context.AUDIO_SERVICE) as AudioManager
        val devices = audio.getDevices(AudioManager.GET_DEVICES_INPUTS)
        startEngine(devices.first().id)
    }

    fun toggleRecording(view: View) {
        val button = view as Button
        val recording = button.text.contains("Record");
        if (recording) {
            button.text = "■ Stop"
            button.backgroundTintList = getColorStateList(this, R.color.grayPrimary)
            setRecording(true)
        } else {
            button.text = "● Record"
            button.backgroundTintList = getColorStateList(this, R.color.redPrimary)
            setRecording(false)
        }
    }

    override fun onDestroy() {
        stopEngine()
        super.onDestroy()
    }


    private fun requestRecordPermission() {
        ActivityCompat.requestPermissions(this, arrayOf(RECORD_AUDIO),0)
    }

    private fun isRecordPermissionGranted(): Boolean {
        return ActivityCompat.checkSelfPermission(this, RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
    }

    external fun setRecording(isRecording: Boolean)
    external fun startEngine(deviceId: Int)
    external fun stopEngine()

}
