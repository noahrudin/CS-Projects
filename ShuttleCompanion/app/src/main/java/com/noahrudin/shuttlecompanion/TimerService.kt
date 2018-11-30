package com.noahrudin.shuttlecompanion

import android.app.*
import android.content.Intent
import android.os.IBinder

import android.widget.Toast
import com.noahrudin.shuttlecompanion.Prefs.context
import java.util.*
import android.app.PendingIntent


class TimerService : Service() {

    override fun onCreate(){
        setAlarm()
        super.onCreate()
    }

    override fun onDestroy(){
        super.onDestroy()
    }


    fun setAlarm(){
        val manager = Prefs.alarm
        val dat = Date()
        val calAlarm = Calendar.getInstance()
        val calNow = Calendar.getInstance()
        val calDismissal = Calendar.getInstance()
        var numToAdd = 0
        calNow.time = dat
        calAlarm.time = dat
        calDismissal.time = dat
        //MESSING WITH TIME DISPLACEMENT HERE WORKS

        var counter = 0
        calDismissal.set(Calendar.HOUR_OF_DAY, Prefs.timeHours)
        calDismissal.set(Calendar.MINUTE, Prefs.timeMinutes)
        calDismissal.set(Calendar.SECOND, 0)


        while(counter < Prefs.arrivalMinutesForLocation.size){

            Prefs.timeOffset =(Prefs.arrivalMinutesForLocation[counter] - Prefs.timeBefore)


            calAlarm.set(Calendar.MINUTE, calNow.time.minutes+Prefs.timeOffset)
            calAlarm.set(Calendar.SECOND, 0)

          if(!calAlarm.before(calDismissal)){
              break
          }

            counter++
        }
        //now check for time extending beyond localMax

        //The next day
        if (calAlarm.before(calNow)) {
            calAlarm.add(Calendar.DATE, 1)
        }

        Prefs.calAlarm = calAlarm
        Prefs.calDismissal= calDismissal
        //Start alarm at this time

        Prefs.alarmTimeString = ""+calAlarm.time
            val myIntent = Intent(context, BroadcastReceiver::class.java)
            myIntent.putExtra("timebefore",Prefs.timeBefore)
            var pendingIntent = PendingIntent.getBroadcast(context, 0, myIntent, 0)
            manager.set(AlarmManager.RTC_WAKEUP, calAlarm.timeInMillis, pendingIntent)
            Toast.makeText(Prefs.context, "Alert set for "+ Prefs.timeBefore + " minutes before shuttle arrival at " + Prefs.location, Toast.LENGTH_SHORT).show()
    }

fun cancelAlarm(){
    val intent = Intent(Prefs.context, BroadcastReceiver::class.java)
    val pIntent = PendingIntent.getBroadcast(Prefs.context, 0,
            intent, PendingIntent.FLAG_UPDATE_CURRENT)
    Prefs.pIntent = pIntent
    Prefs.alarm.cancel(pIntent)
}
    override fun onBind(p0: Intent?): IBinder {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}
