package com.noahrudin.shuttlecompanion

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView

class AboutActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_about)

        var textView:TextView = findViewById(R.id.textView5)
        var okBtn: Button = findViewById(R.id.button)
        textView.text = "Thanks for using Boise State's Shuttle Companion! To use this app:\n\n1: Choose a shuttle line and pickup location. The default pickup location is on University Drive, at the SUB.\n\n" +
                "2. Set the time of the meeting or class dismissal. This is set to current time by default (Note that Boise State shuttles do not provide ETAs beyond one full route cycle, so the alarm cannot" +
                " be set beyond 20 minutes from current time).\n\n3: Set your preferred notification time. This is how long before the shuttle's arrival you will receive a notification.\n\n4: Press the save icon to set an alarm. " +
                "This alarm will be functional even if the app process is killed." +
                "\n\n5: To cancel the alarm at any time, simply press the clear key in the bottom left corner.\n\nPlease email any bugs to noahrudin@gmail.com and enjoy!\nMost app crashes occur due to the Shuttle Tracker itself" +
                " failing to load. The only known remedy at this time is to reload at a later time once the Shuttle Tracker is operational."
        okBtn.setOnClickListener{
            finish()
        }
    }

}
