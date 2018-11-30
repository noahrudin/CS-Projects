package com.noahrudin.shuttlecompanion

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView

class Main2Activity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main2)

        var okBtn: Button = findViewById(R.id.button4)
        var textview: TextView = findViewById(R.id.textView4)

        fun close(){
            android.os.Process.killProcess(android.os.Process.myPid())
            finish()
        }
        okBtn.setOnClickListener {
            close()
        }
    }
}
