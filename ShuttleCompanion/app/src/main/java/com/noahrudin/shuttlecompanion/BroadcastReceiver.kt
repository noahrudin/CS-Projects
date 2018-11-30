package com.noahrudin.shuttlecompanion

/**
 * Created by Noah Rudin on 4/22/2018.
 */
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.support.v4.app.NotificationCompat
import android.app.NotificationChannel
import android.graphics.Color
import android.media.RingtoneManager
import android.os.Build




class BroadcastReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context?, intent: Intent?) {
        //TODO: Wake Screen on notification
        //TODO: Extend beyond current shuttle cycle (Not going to happen on this release)

        //tentatively done
        //TODO: Kill app and keep notification



            val extras = intent!!.extras
            var timeBefore = extras.getInt("timebefore")

            if (timeBefore != 1) {
                sendNotification(context!!, "Shuttle Companion", "Your shuttle is " + timeBefore + " minutes away!")
            } else {
                sendNotification(context!!, "Shuttle Companion", "Your shuttle is " + timeBefore + " minute away!")
            }
    }

    fun sendNotification(context: Context, title: String, body: String){
        var prefsNot: NotificationManager?=null
        val alarmSound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION)
        val pattern = longArrayOf(250, 200, 250, 200)
        if(Build.VERSION.SDK_INT >=Build.VERSION_CODES.O){
            val notificationManager = context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            val notificationChannel = NotificationChannel("26", "My Notifications", NotificationManager.IMPORTANCE_HIGH)
            notificationChannel.description = "Channel description"
            notificationChannel.enableLights(true)
            notificationChannel.lightColor = Color.BLUE
            notificationChannel.vibrationPattern = pattern
            notificationChannel.enableVibration(true)

            prefsNot?.createNotificationChannel(notificationChannel)

            val builder = NotificationCompat.Builder(context, "26")
                    .setVibrate(pattern)
                    .setSound(alarmSound)
                    .setSmallIcon(R.mipmap.icon_24_mdpi)
                    .setContentTitle(title)
                    .setContentText(body)


            notificationManager.notify(26, builder.build())
        }else {
            val notification = NotificationCompat.Builder(context)
                    .setSound(alarmSound)
                    .setSmallIcon(R.mipmap.icon_24_mdpi)
                    .setBadgeIconType(R.mipmap.icon_24_mdpi)
                    .setLights(Color.BLUE, 500, 500)
                    .setVibrate(pattern)
                    .setContentTitle(title)
                    .setContentText(body)
                    .setPriority(NotificationManager.IMPORTANCE_MAX)

            val pendingIntent: PendingIntent = PendingIntent.getActivity(context, 0, Intent(context, TimerService::class.java), 0)

            notification.setContentIntent(pendingIntent)
            val notificationManager = context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            notificationManager.notify(1, notification.build())
        }

    }

}