package ru.forsk.AdCtl;

import org.qtproject.qt5.android.bindings.QtActivity;

import android.util.Log;
import android.net.Uri;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ContentResolver;
import java.io.File;

public class AdCtlUtil {
    public static void sendText(Activity context,String text) {
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);
//        sendIntent.putExtra(Intent.EXTRA_TEXT, text);
//        sendIntent.setType("text/plain");
        context.startActivity(Intent.createChooser(sendIntent, text));
    }

    public static void shareIntent(String data,QtActivity context){
        Log.d("ASD","ASD");
//        Uri uri = Uri.fromFile(data);
        File f = new File(data);
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);
        sendIntent.setType("image/png");
        sendIntent.putExtra(Intent.EXTRA_STREAM,Uri.fromFile(f));
        context.startActivity(Intent.createChooser(sendIntent, "Share Score"));
    }
}
