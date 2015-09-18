//=============================================================================
// Copyright (c) 2014 Nicolas Froment

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//=============================================================================

package com.lasconic;

import org.qtproject.qt5.android.QtNative;

import com.google.android.gms.analytics.HitBuilders;
import com.google.android.gms.analytics.Logger;
import com.google.android.gms.analytics.Tracker;
import com.google.android.gms.analytics.GoogleAnalytics;

import java.lang.String;
import java.util.HashMap;
import android.util.Log;

import ru.forsk.darkstories.R;

public class QGoogleAnalytics
{
    /**
    * Enum used to identify the tracker that needs to be used for tracking.
    *
    * A single tracker is usually enough for most purposes. In case you do need multiple trackers,
    * storing them all in Application object helps ensure that they are created only once per
    * application instance.
    */
    public enum TrackerName {
        APP_TRACKER, // Tracker used only in this app.
    }

    private static HashMap<TrackerName, Tracker> mTrackers = new HashMap<TrackerName, Tracker>();

    protected QGoogleAnalytics()
    {
       //Log.d("lasconic", "QGoogleAnalytics()");
    }

    private static synchronized Tracker getTracker(TrackerName trackerId) {
        if (QtNative.activity() == null)
            return null;
        if (!mTrackers.containsKey(trackerId)) {
            GoogleAnalytics analytics = GoogleAnalytics.getInstance(QtNative.activity().getApplication());
            Tracker t = (trackerId == TrackerName.APP_TRACKER) ? analytics.newTracker(R.xml.analytics) : analytics.newTracker("");
            mTrackers.put(trackerId, t);
        }
        return mTrackers.get(trackerId);
    }

    public static void sendHit(String screenName) {
        // Get tracker.
        Tracker t = getTracker(TrackerName.APP_TRACKER);

        if (t == null)
            return;

        // Set screen name.
        t.setScreenName(screenName);

        // Send a screen view.
        t.send(new HitBuilders.AppViewBuilder().build());
    }

    public static void sendEvent(String category, String action, String label, long value) {
        // Get tracker.
        Tracker t = getTracker(TrackerName.APP_TRACKER);
        // Build and send an Event.
        HitBuilders.EventBuilder eventBuilder = new HitBuilders.EventBuilder().setCategory(category)
            .setAction(action);
        if (label != null && !label.equals(""))
            eventBuilder.setLabel(label);
        if (value > 0)
            eventBuilder.setValue(value);
        t.send(eventBuilder.build());
    }
}
