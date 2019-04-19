package com.jefshe.litmusic

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.github.mikephil.charting.charts.BarChart
import com.github.mikephil.charting.data.BarData
import com.github.mikephil.charting.data.BarDataSet
import com.github.mikephil.charting.data.BarEntry


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chart = findViewById(R.id.chart) as BarChart
        val data = BarDataSet(listOf(BarEntry(1.2f, 1.4f)), "Spectrum")
        chart.setData(BarData(data));
        chart.invalidate(); // refresh
        setContentView(R.layout.activity_main)
    }

    external fun setRecording(isRecording: Boolean)

}
