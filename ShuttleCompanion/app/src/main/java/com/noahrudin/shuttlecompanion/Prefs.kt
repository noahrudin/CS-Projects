package com.noahrudin.shuttlecompanion

import android.annotation.SuppressLint
import android.app.AlarmManager
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.os.PowerManager
import android.widget.TimePicker
import org.jsoup.select.Elements
import java.net.URL
import java.sql.Time
import java.time.LocalTime
import java.util.*
import java.util.concurrent.TimeoutException


@SuppressLint("StaticFieldLeak")
/**
 * Created by Noah Rudin on 4/21/2018.
 */

object Prefs{
    //Alarm Prefs
    var shuttleRouteIndex: Int = 0 //0 for downtown, 1 for on campus
    var shuttleRoute: String = "Orange Downtown"
    var location: String = ""
    var timeBefore: Int = -1
    var timeHours: Int = -1
    var timeMinutes: Int = -1
    var timeString: String = ""
    var context: Context? = null
    lateinit var alarm: AlarmManager
    var alarmTimeString: String = ""
    var notificationId = 0
    lateinit var prefsNot: NotificationManager
    lateinit var channel : NotificationChannel
    lateinit var powerManager: PowerManager
    val url = URL("http://shuttle.boisestate.edu/m/Route.aspx?RouteID=21")
    //Shuttle parse prefs
    var arrivalMinutes: MutableList<String> = mutableListOf()
    var arrivalData: MutableList<String> = mutableListOf()

    var arrivalMinutesForLocation :MutableList<Int> = mutableListOf()
    lateinit var shuttles : String
    var dataString: String = ""
    var numShuttles: Int = 0
    lateinit var links: Elements
    var choiceIndex: Int  = 0
    var errorCode: Int =0

    var timeOffset:Int =0
    var localMax: Int =0

    lateinit var calAlarm: Calendar
    lateinit var calDismissal: Calendar
    lateinit var pIntent: PendingIntent
}
