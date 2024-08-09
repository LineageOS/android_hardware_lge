package org.lineageos.settings.device.dac;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.service.quicksettings.Tile;
import android.service.quicksettings.TileService;
import android.util.Log;

import org.lineageos.settings.device.dac.utils.QuadDAC;

public class QuadDACTileService extends TileService {

    private final static String TAG = "QuadDACTileService";

    private HeadsetPluggedTileReceiver headsetPluggedTileReceiver = new HeadsetPluggedTileReceiver();

    private QuadDAC DacControlInterface = new QuadDAC();

    @Override
    public void onClick() {
        super.onClick();
        try {
            if (DacControlInterface.isEnabled()) {
                DacControlInterface.disable();
                setTileInactive();
            } else {
                DacControlInterface.enable();
                setTileActive();
            }
        } catch(Exception e) {}
    }

    @Override
    public void onStartListening() {
        super.onStartListening();
        try {
            DacControlInterface.initialize();
        } catch(Exception e) {}
        IntentFilter filter = new IntentFilter(Intent.ACTION_HEADSET_PLUG);
        registerReceiver(headsetPluggedTileReceiver, filter);
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
            if(!DacControlInterface.dac_service_available) {
                setTileUnavailable();
                return;
            }
            if (intent.getAction().equals(Intent.ACTION_HEADSET_PLUG)) {
                int state = intent.getIntExtra("state", -1);
                switch(state)
                {
                    case 1: // Headset plugged in
                        try {
                            if (DacControlInterface.isEnabled()) {
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
