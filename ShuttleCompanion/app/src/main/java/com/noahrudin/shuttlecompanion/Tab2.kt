package com.noahrudin.shuttlecompanion


import android.content.Intent
import android.os.AsyncTask
import android.support.v4.app.Fragment
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import io.netty.channel.ConnectTimeoutException

import org.jsoup.Jsoup
import org.jsoup.nodes.Document


/**
 * Created by Noah Rudin on 4/21/2018.
 */

class Tab2  : Fragment(){
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
    //variable declarations
        var rootView = inflater.inflate(R.layout.tab2, container, false)
        var saveBtn: ImageButton = rootView.findViewById(R.id.imageButton)
        var clearBtn: ImageButton = rootView.findViewById(R.id.imageButton2)
        var aboutBtn: ImageButton = rootView.findViewById(R.id.imageButton4)
        val timePicker: TimePicker = rootView.findViewById(R.id.timePicker)
        var editText: EditText = rootView.findViewById(R.id.editText2)
        var locationSpinner :Spinner
        var routeSpinner: Spinner
        var locationChoice  = ""
        var notificationId = 0
        Prefs.shuttleRouteIndex =0
        Prefs.notificationId = notificationId

        //refreshOrange()
        if(Prefs.errorCode == 404){
            val intent = Intent(Prefs.context, Main2Activity::class.java)
            startActivity(intent)
        }

        locationSpinner = rootView.findViewById(R.id.spinner)
        routeSpinner = rootView.findViewById(R.id.spinner2)




        var choices = arrayOf("Please Make A Selection","University at the SUB ","University at the B ", "ChrisWay/ILC/Plaza West Lot ", "Brady Parking Garage ", "Lusk and Sherwood ", "Lusk and Royal ", "Downtown ", "St. Luke's Hospital ", "Broadway Park Center ", "University & Denver", "University & Grant", "Bronco Circle", "Micron Engineering Building", "REC Center")
//        val choices = arrayOf("Please Make A Selection","University at the SUB ","Administration Building ","ChrisWay/ILC","Brady Parking Garage ","Micron Business & Economics Bldg ","Towers Hall Dorms ","Science & Education Bldg ","Library ","Theatre Lane ","Chaffee Hall ","Chaffee Hall Front Entr. ","Caven-Williams Sports Complex ","Bleymaier Football Ctr. ","Albertsons Stadium East Entr. ","Stadium South Lot ","Academic & Career Ctr.", "Bronco Lane ")
            val routeChoice = arrayOf("Orange Downtown", "Blue On Campus")

        routeSpinner.adapter = ArrayAdapter<String>(Prefs.context, android.R.layout.simple_list_item_1, routeChoice)
        routeSpinner.onItemSelectedListener =  object : AdapterView.OnItemSelectedListener{
            override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
                if(Prefs.errorCode == 404 || Prefs.errorCode == 405){
                    val intent = Intent(Prefs.context, Main2Activity::class.java)
                    startActivity(intent)
                }
                Prefs.shuttleRoute = routeChoice.get(position)
                Prefs.shuttleRouteIndex = position

                if(position == 0){
                    choices = arrayOf("Please Make A Selection","University at the SUB ","University at the B ", "ChrisWay/ILC/Plaza West Lot ", "Brady Parking Garage ", "Lusk and Sherwood ", "Lusk and Royal ", "Downtown ", "St. Luke's Hospital ", "Broadway Park Center ", "University & Denver", "University & Grant", "Bronco Circle", "Micron Engineering Building", "REC Center")
                    locationSpinner.adapter = ArrayAdapter<String>(Prefs.context, android.R.layout.simple_list_item_1, choices)
                    refreshOrange()
                }else{
                    choices = arrayOf("Please Make A Selection","Student Union Transit Ctr ","Administration Building ","ChrisWay/ILC","Brady Parking Garage ","Micron Business & Economics Bldg ","Towers Hall Dorms ","Science & Education Bldg ","Library ","Theatre Lane ","Chaffee Hall ","Chaffee Hall Front Entr. ","Caven-Williams Sports Complex ","Bleymaier Football Ctr. ","Albertsons Stadium East Entr. ","Stadium South Lot ","Academic & Career Ctr.", "Bronco Lane ")
                    locationSpinner.adapter = ArrayAdapter<String>(Prefs.context, android.R.layout.simple_list_item_1, choices)
                    refreshBlue()
                }
            }

            override fun onNothingSelected(p0: AdapterView<*>?) {
                Prefs.shuttleRoute = "Orange Downtown"
                Prefs.shuttleRouteIndex= 0
            }

        }

            locationSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener{
                override fun onNothingSelected(p0: AdapterView<*>?) {
                    locationChoice = "Please Make A Selection"
                }

                override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
                    if(Prefs.errorCode == 404){
                        val intent = Intent(Prefs.context, Main2Activity::class.java)
                        startActivity(intent)
                    }
                    var index = 0
                    if(position == 0){
                        index = 1
                        locationChoice = choices.get(index)
                    }else{
                        locationChoice = choices.get(position)
                        index = position
                    }


                    Prefs.choiceIndex = index-1
                    Prefs.location = locationChoice
                    Prefs.arrivalMinutesForLocation.clear()
                    //Populates List of Arrival Time Values

                   getData()

                }

            }


        saveBtn.setOnClickListener {
            getData()
            if(Prefs.errorCode == 404){
                val intent = Intent(Prefs.context, Main2Activity::class.java)
                startActivity(intent)
            }
            if(editText.text.isEmpty()){
                Prefs.timeBefore = 5
            }else{
                Prefs.timeBefore = editText.text.toString().toInt()
            }
                Prefs.timeHours = timePicker.hour
                Prefs.timeMinutes = timePicker.minute

           if((Prefs.timeMinutes * 1) == 0 || Prefs.timeMinutes == 0){

               Prefs.timeString = "" + Prefs.timeHours+ ":" +Prefs.timeMinutes + "0"
           }else{
               Prefs.timeString = "" + Prefs.timeHours+ ":" +Prefs.timeMinutes
           }


            if(Prefs.localMax>0) {
                TimerService().setAlarm()
            }else{
                Toast.makeText(Prefs.context, "No alarm can be set at this time", Toast.LENGTH_LONG).show()
            }

            if(Prefs.shuttleRouteIndex==1){
                refreshBlue()
            }else{
                refreshOrange()
            }


        }
        clearBtn.setOnClickListener{

            Prefs.timeBefore = -1
            Prefs.timeHours = -1
            Prefs.timeMinutes = -1
            Prefs.timeString = ""
            //Prefs.location = ""
            TimerService().cancelAlarm()
           Toast.makeText(Prefs.context, "Alarm Canceled.", Toast.LENGTH_SHORT).show()
        }

        aboutBtn.setOnClickListener{
            val intent = Intent(Prefs.context, AboutActivity::class.java)
            startActivity(intent)
        }
        return rootView
    }

    private fun getData() {
        var countData = 0
        var countShuttles = 0
        var isNum = false

        //Clear old variables
        Prefs.arrivalMinutes.clear()
        Prefs.arrivalMinutesForLocation.clear()
        Prefs.localMax = 0
        while(countData < Prefs.arrivalData.size){
            isNum = false
            isNum = Prefs.arrivalData[countData].matches("-?\\d+(\\.\\d+)?".toRegex())
            if(isNum == true){
                countShuttles++
                if(countShuttles>Prefs.numShuttles){
                    Prefs.arrivalMinutes.add(Prefs.arrivalData.get(countData))
                }

            }else if(Prefs.arrivalData[countData].contains("Arriving")){
                countShuttles++
                if(countShuttles>Prefs.numShuttles){
                    Prefs.arrivalMinutes.add("0")
                }
            }
            countData ++
        }
        //arrivalMinutes now has all arrival ETAs

        var numToAdd = Prefs.numShuttles
        while(numToAdd>0){
            Prefs.arrivalMinutesForLocation.add(Prefs.arrivalMinutes.get((Prefs.numShuttles * Prefs.choiceIndex)+numToAdd-1).toInt())
            if(Prefs.arrivalMinutes.get((Prefs.numShuttles * Prefs.choiceIndex)+numToAdd-1).toInt()>Prefs.localMax){
                Prefs.localMax = Prefs.arrivalMinutes.get((Prefs.numShuttles * Prefs.choiceIndex)+numToAdd-1).toInt()
            }
            numToAdd --
        }
        Prefs.arrivalMinutesForLocation.sort()
    }

    private fun refreshOrange(){
        ParseOrange().execute()
    }
    private fun refreshBlue(){
        ParseBlue().execute()
    }


    inner class ParseOrange: AsyncTask<String, String, String>() {

        override fun onPreExecute() {
            super.onPreExecute()

        }

        override fun doInBackground(vararg p0: String?): String {

            Prefs.arrivalMinutes.clear()
            Prefs.dataString = ""
            var Result: String = "";


            try {
                var doc: Document = Jsoup.connect("http://shuttle.boisestate.edu/m/Route.aspx?RouteID=21").get()

                var title: String = doc.title()
                Prefs.links = doc.select("body > form > div > span > table > tbody > *")
                //Number of shuttles and Shuttle Names
                Prefs.shuttles = Prefs.links[0].text()


                Result = Prefs.links.text()

            } catch(Ex: ConnectTimeoutException) {
                Log.d("", "Connection Error " + Ex.message)
                return 404.toString()
            } catch (Ex: IndexOutOfBoundsException){
                Log.d("", "Connection Error " + Ex.message)
                return 405.toString()
            }
            return Result

        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
            if(result == "404"){
                Prefs.errorCode = 404
                return
            }
            if(result == "405"){
                Prefs.errorCode = 405
                return
            }
            var shuttleTemp = Prefs.shuttles.split("Stop ", " ", "\n")
            var timeTemp = Prefs.links.text().split(" ", "Stop ", "Shuttle ", "University at the SUB ","University at Joyce (The B) ", "ChrisWay/ILC/Plaza West Lot ", "Brady Parking Garage ", "Lusk and Sherwood ", "Lusk and Royal ", "Downtown ", "St. Luke's Hospital ", "Broadway Park Center ", "University Drive & Denver Ave ", "University Drive & Grant Ave ", "Bronco Circle ", "Micron Engineering Building ", "REC Center ", "min")
            Prefs.arrivalData = timeTemp.toMutableList()
            Prefs.shuttles = shuttleTemp.toString()
            Prefs.numShuttles = (shuttleTemp.size-1)/2
            var count =1


            while (count < Prefs.links.size){
                Prefs.dataString += (Prefs.links[count].text()+"\n")
                count ++
            }

            return


        }
    }
    inner class ParseBlue: AsyncTask<String, String, String>() {

        override fun onPreExecute() {
            super.onPreExecute()

        }

        override fun doInBackground(vararg p0: String?): String {

            Prefs.arrivalMinutes.clear()
            Prefs.dataString = ""
            var Result: String = "";


            try {
                var doc: Document = Jsoup.connect("http://shuttle.boisestate.edu/m/Route.aspx?RouteID=20").get()

                var title: String = doc.title()
                Prefs.links = doc.select("body > form > div > span > table > tbody > *")
                //Number of shuttles and Shuttle Names
                Prefs.shuttles = Prefs.links[0].text()

                Result = Prefs.links.text()

            } catch(Ex: ConnectTimeoutException) {
                Log.d("", "Connection Error " + Ex.message)
                return 404.toString()
            } catch (Ex: IndexOutOfBoundsException){
                Log.d("", "Connection Error " + Ex.message)
                return 405.toString()
            }
            return Result

        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
            if(result == "404"){
                Prefs.errorCode = 404
                return
            }
            if(result == "405"){
                Prefs.errorCode = 405
                return
            }
            var shuttleTemp = Prefs.shuttles.split("Stop ", " ", "\n")
            var timeTemp = Prefs.links.text().split(" ", "Stop ", "Shuttle ","University at the SUB ","Administration Building ","ChrisWay/ILC","Brady Parking Garage ","Micron Business & Economics Bldg ","Towers Hall Dorms ","Science & Education Bldg ","Library ","Theatre Lane ","Chaffee Hall ","Chaffee Hall Front Entr. ","Caven-Williams Sports Complex ","Bleymaier Football Ctr. ","Albertsons Stadium East Entr. ","Stadium South Lot ","Academic & Career Ctr.", "Bronco Lane ", "min")
            Prefs.arrivalData = timeTemp.toMutableList()
            Prefs.shuttles = shuttleTemp.toString()
            Prefs.numShuttles = (shuttleTemp.size-1)/2
            var count =1


            while (count < Prefs.links.size){
                Prefs.dataString += (Prefs.links[count].text()+"\n")
                count ++
            }

            return


        }
    }


}

