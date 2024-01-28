package org.lineageos.settings.device.dac;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.service.quicksettings.Tile;
import android.service.quicksettings.TileService;
import android.util.Log;

import org.lineageos.settings.device.dac.utils.QuadDAC;

import vendor.lge.hardware.audio.dac.control.V2_0.IDacControl;

public class QuadDACTileService extends TileService {

    private final static String TAG = "QuadDACTileService";

    private HeadsetPluggedTileReceiver headsetPluggedTileReceiver = new HeadsetPluggedTileReceiver();

    private IDacControl dac;

    private boolean dac_service_available = false;

    @Override
    public void onClick() {
        super.onClick();
        try {
            if (QuadDAC.isEnabled(dac)) {
                QuadDAC.disable(dac);
                setTileInactive();
            } else {
                QuadDAC.enable(dac);
                setTileActive();
            }
        } catch(Exception e) {}
    }

    @Override
    public void onStartListening() {
        super.onStartListening();

        IntentFilter filter = new IntentFilter(Intent.ACTION_HEADSET_PLUG);
        registerReceiver(headsetPluggedTileReceiver, filter);

        AudioManager am = getSystemService(AudioManager.class);

        try {
            dac = IDacControl.getService(true);
            dac_service_available = true;
        } catch(Exception e)
        { }

        if(!am.isWiredHeadsetOn())
        {
            setTileUnavailable();
	    return;
        }

        try {
            if (QuadDAC.isEnabled(dac)) {
                setTileActive();
            } else {
                setTileInactive();
            }
        } catch(Exception e) {}
        if(!dac_service_available) {
            setTileUnavailable();
        }
    }

    @Override
    public void onStopListening() {
        super.onStopListening();

        unregisterReceiver(headsetPluggedTileReceiver);

    }

    private void setTileActive()
    {
        Tile quaddactile = getQsTile();
        quaddactile.setState(Tile.STATE_ACTIVE);
        quaddactile.setLabel(getResources().getString(R.string.quad_dac_on));
	    quaddactile.updateTile();
    }

    private void setTileInactive()
    {
        Tile quaddactile = getQsTile();
        quaddactile.setState(Tile.STATE_INACTIVE);
        quaddactile.setLabel(getResources().getString(R.string.quad_dac_off));
	    quaddactile.updateTile();
    }

    private void setTileUnavailable()
    {
        Tile quaddactile = getQsTile();
        quaddactile.setState(Tile.STATE_UNAVAILABLE);
        quaddactile.setLabel(getResources().getString(R.string.quad_dac_unavail));
	    quaddactile.updateTile();
    }

    private class HeadsetPluggedTileReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_HEADSET_PLUG)) {
                int state = intent.getIntExtra("state", -1);
                switch(state)
                {
                    case 1: // Headset plugged in
                        try {
                            if (QuadDAC.isEnabled(dac)) {
                                setTileActive();
                            } else {
                                setTileInactive();
                            }
                        } catch(Exception e) {}
                        break;
                    case 0: // Headset unplugged
                        setTileUnavailable();
                        break;
                    default: break;
                }
            }
        }
    }

}
