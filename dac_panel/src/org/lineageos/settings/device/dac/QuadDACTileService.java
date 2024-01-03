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
import org.lineageos.settings.device.dac.QuadDACPanelActivity;

public class QuadDACTileService extends TileService {

    private final static String TAG = "QuadDACTileService";

    private Tile QuadDACTile;

    private HeadsetPluggedTileReceiver headsetPluggedTileReceiver = new HeadsetPluggedTileReceiver();

    @Override
    public void onClick() {
        super.onClick();

        Intent intent = new Intent(this, QuadDACPanelActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivityAndCollapse(intent);
    }

    @Override
    public void onStartListening() {
        super.onStartListening();

        QuadDACTile = getQsTile();

        IntentFilter filter = new IntentFilter(Intent.ACTION_HEADSET_PLUG);
        registerReceiver(headsetPluggedTileReceiver, filter);

        AudioManager am = getSystemService(AudioManager.class);

        if(!am.isWiredHeadsetOn())
        {
            setTileUnavailable();
	        return;
        }

        try {
            if (QuadDAC.isEnabled()) {
                setTileActive();
            } else {
                setTileInactive();
            }
        } catch(Exception e) {}

        if(!QuadDAC.dac_service_available) {
            setTileUnavailable();
        }
    }

    @Override
    public void onStopListening() {
        unregisterReceiver(headsetPluggedTileReceiver);
    }

    private void setTileActive()
    {
        QuadDACTile.setState(Tile.STATE_ACTIVE);
        QuadDACTile.setLabel(getResources().getString(R.string.quad_dac_on));
	    QuadDACTile.updateTile();
    }

    private void setTileInactive()
    {
        QuadDACTile.setState(Tile.STATE_INACTIVE);
        QuadDACTile.setLabel(getResources().getString(R.string.quad_dac_off));
	    QuadDACTile.updateTile();
    }

    private void setTileUnavailable()
    {
        QuadDACTile.setState(Tile.STATE_UNAVAILABLE);
        QuadDACTile.setLabel(getResources().getString(R.string.quad_dac_unavail));
	    QuadDACTile.updateTile();
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
                            if (QuadDAC.isEnabled()) {
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
