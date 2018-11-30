package com.noahrudin.shuttlecompanion


import android.support.v4.app.Fragment
import android.os.Bundle
import android.support.design.widget.FloatingActionButton
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.webkit.WebResourceRequest
import android.webkit.WebView
import android.webkit.WebViewClient
import android.widget.Toast


/**
 * Created by Noah Rudin on 4/21/2018.
 */


class Tab1: Fragment(){

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {

        val rootView = inflater.inflate(R.layout.tab1, container, false)

        var mWebView:WebView = rootView.findViewById(R.id.webview)
        mWebView.loadUrl("http://shuttle.boisestate.edu")

        //Enable JS
        var webSettings = mWebView.settings
        webSettings.javaScriptEnabled = true

        //Open only in WebView
        mWebView.webViewClient = MyWebViewClient()

        var statusButton: FloatingActionButton = rootView.findViewById(R.id.fab)
        var helpButton: FloatingActionButton = rootView.findViewById(R.id.floatingActionButton2)
        statusButton.setOnClickListener {
            if (Prefs.timeBefore == -1 || Prefs.localMax == 0) {
                Toast.makeText(Prefs.context, "No Alarm Set", Toast.LENGTH_SHORT).show()
            }else{
                Toast.makeText(Prefs.context, "Alert set for "+ Prefs.alarmTimeString, Toast.LENGTH_SHORT).show()
            }
        }
        helpButton.setOnClickListener{
            Toast.makeText(Prefs.context, "Swipe left to begin using Shuttle Companion", Toast.LENGTH_SHORT).show()
        }
        return rootView
    }

}

@Suppress("OverridingDeprecatedMember")
class MyWebViewClient: WebViewClient(){
    override fun shouldOverrideUrlLoading(view: WebView?, request: WebResourceRequest?): Boolean {

        view?.loadUrl(request?.url.toString())
        return true

    }

    override fun shouldOverrideUrlLoading(view: WebView?, url: String?): Boolean {
        view?.loadUrl(url)
        return true
    }
}