package org.lineageos.settings.device.dac;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.os.Bundle;
import android.provider.Settings;
import androidx.preference.Preference;
import androidx.preference.PreferenceCategory;
import androidx.preference.PreferenceFragment;
import androidx.preference.Preference.OnPreferenceChangeListener;
import androidx.preference.PreferenceFragmentCompat;
import androidx.preference.SwitchPreference;
import androidx.preference.ListPreference;
import androidx.preference.SeekBarPreference;

import android.util.Log;
import android.view.MenuItem;

import org.lineageos.settings.device.dac.ui.BalancePreference;
import org.lineageos.settings.device.dac.utils.Constants;
import org.lineageos.settings.device.dac.utils.QuadDAC;

import java.util.ArrayList;

import vendor.lge.hardware.audio.dac.control.V1_0.AdvancedFeature;
import vendor.lge.hardware.audio.dac.control.V1_0.HalFeature;
import vendor.lge.hardware.audio.dac.control.V1_0.IDacAdvancedControl;
import vendor.lge.hardware.audio.dac.control.V1_0.IDacHalControl;
import vendor.lge.hardware.audio.dac.control.V1_0.Range;

public class QuadDACPanelFragment extends PreferenceFragment
        implements OnPreferenceChangeListener {

    private static final String TAG = "QuadDACPanelFragment";

    private SwitchPreference quaddac_switch;
    private ListPreference sound_preset_list, digital_filter_list, mode_list;
    private BalancePreference balance_preference;
    private SeekBarPreference avc_volume;

    private HeadsetPluggedFragmentReceiver headsetPluggedFragmentReceiver;

    private IDacAdvancedControl dac;

    private IDacHalControl dhc;

    private ArrayList<Integer> dac_features;
    private ArrayList<Integer> dhc_features;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        headsetPluggedFragmentReceiver = new HeadsetPluggedFragmentReceiver();
        try {
            dac = IDacAdvancedControl.getService(true);
            dhc = IDacHalControl.getService(true);

            dac_features = dac.getSupportedAdvancedFeatures();
            dhc_features = dhc.getSupportedHalFeatures();
        } catch(Exception e) {
            Log.d(TAG, "onCreatePreferences: " + e.toString());
        }
        addPreferencesFromResource(R.xml.quaddac_panel);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {

        try {
            if (preference instanceof SwitchPreference) {

                boolean set_dac_on = (boolean) newValue;

                if (set_dac_on) {
                    enableExtraSettings();
                    QuadDAC.enable(dhc, dac);
                    return true;
                } else {
                    disableExtraSettings();
                    QuadDAC.disable(dhc);
                    return true;
                }
            }
            if (preference instanceof ListPreference) {
                if (preference.getKey().equals(Constants.HIFI_MODE_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int mode = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setDACMode(dac, mode);
                    return true;

                } else if (preference.getKey().equals(Constants.DIGITAL_FILTER_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int digital_filter = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setDigitalFilter(dhc, digital_filter);
                    return true;

                } else if (preference.getKey().equals(Constants.SOUND_PRESET_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int sound_preset = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setSoundPreset(dhc, sound_preset);
                    return true;
                }
                return false;
            }

            if (preference instanceof SeekBarPreference) {
                if (preference.getKey().equals(Constants.AVC_VOLUME_KEY)) {
                    if (newValue instanceof Integer) {
                        Integer avc_vol = (Integer) newValue;

                        //avc_volume.setSummary( ((double)avc_vol) + " db");

                        QuadDAC.setAVCVolume(dac, avc_vol);
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        } catch (Exception e) {
            Log.d(TAG, "onPreferenceChange: " + e.toString());
        }
        return false;
    }

    @Override
    public void onResume() {
        IntentFilter filter = new IntentFilter(Intent.ACTION_HEADSET_PLUG);
        getActivity().registerReceiver(headsetPluggedFragmentReceiver, filter);
        super.onResume();
    }

    @Override
    public void onPause() {
        getActivity().unregisterReceiver(headsetPluggedFragmentReceiver);
        super.onPause();
    }

    @Override
    public void addPreferencesFromResource(int preferencesResId) {
        super.addPreferencesFromResource(preferencesResId);
        // Initialize preferences
        AudioManager am = getContext().getSystemService(AudioManager.class);

        quaddac_switch = (SwitchPreference) findPreference(Constants.DAC_SWITCH_KEY);
        quaddac_switch.setOnPreferenceChangeListener(this);

        sound_preset_list = (ListPreference) findPreference(Constants.SOUND_PRESET_KEY);
        sound_preset_list.setOnPreferenceChangeListener(this);

        digital_filter_list = (ListPreference) findPreference(Constants.DIGITAL_FILTER_KEY);
        digital_filter_list.setOnPreferenceChangeListener(this);

        mode_list = (ListPreference) findPreference(Constants.HIFI_MODE_KEY);
        mode_list.setOnPreferenceChangeListener(this);

        avc_volume = (SeekBarPreference) findPreference(Constants.AVC_VOLUME_KEY);
        avc_volume.setOnPreferenceChangeListener(this);

        balance_preference = (BalancePreference) findPreference(Constants.BALANCE_KEY);

        try {
            if (dhc_features.contains(HalFeature.QuadDAC)) {
                quaddac_switch.setVisible(true);
            }
            if (dhc_features.contains(HalFeature.SoundPreset)) {
                sound_preset_list.setVisible(true);
                sound_preset_list.setValueIndex(dhc.getFeatureValue(HalFeature.SoundPreset));
            }
            if (dhc_features.contains(HalFeature.DigitalFilter)) {
                digital_filter_list.setVisible(true);
                digital_filter_list.setValueIndex(dhc.getFeatureValue(HalFeature.DigitalFilter));
            }
            if (dhc_features.contains(HalFeature.BalanceLeft)
                    && dhc_features.contains(HalFeature.BalanceRight)) {
                balance_preference.setVisible(true);
                balance_preference.setDhc(dhc);
            }
            if (dac_features.contains(AdvancedFeature.AVCVolume)) {
                avc_volume.setVisible(true);
                Range range = dac.getSupportedAdvancedFeatureValues(AdvancedFeature.AVCVolume).range;
                avc_volume.setMin((int)range.min);
                avc_volume.setMax((int)range.max);
                avc_volume.setValue(dac.getFeatureValue(AdvancedFeature.AVCVolume));
            }
            if (dac_features.contains(AdvancedFeature.HifiMode)) {
                mode_list.setVisible(true);
                mode_list.setValueIndex(dac.getFeatureValue(AdvancedFeature.HifiMode));
            }
        } catch(Exception e) {
            Log.d(TAG, "addPreferencesFromResource: " + e.toString());
        }

        try {
            if (am.isWiredHeadsetOn()) {
                quaddac_switch.setEnabled(true);
                if (QuadDAC.isEnabled(dhc)) {
                    quaddac_switch.setChecked(true);
                    enableExtraSettings();
                } else {
                    quaddac_switch.setChecked(false);
                    disableExtraSettings();
                }
            } else {
                quaddac_switch.setEnabled(false);
                disableExtraSettings();
                if (QuadDAC.isEnabled(dhc)) {
                    quaddac_switch.setChecked(true);
                }
            }
        } catch (Exception e) {
            Log.d(TAG, "addPreferencesFromResource2: " + e.toString());
        }
    }

    private void enableExtraSettings()
    {
        sound_preset_list.setEnabled(true);
        digital_filter_list.setEnabled(true);
        mode_list.setEnabled(true);
        avc_volume.setEnabled(true);
        balance_preference.setEnabled(true);
    }

    private void disableExtraSettings()
    {
        sound_preset_list.setEnabled(false);
        digital_filter_list.setEnabled(false);
        mode_list.setEnabled(false);
        avc_volume.setEnabled(false);
        balance_preference.setEnabled(false);
    }

    private class HeadsetPluggedFragmentReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_HEADSET_PLUG)) {
                int state = intent.getIntExtra("state", -1);
                switch(state)
                {
                    case 1: // Headset plugged in
                        quaddac_switch.setEnabled(true);
                        if(quaddac_switch.isChecked())
                        {
                            enableExtraSettings();
                        }
                        break;
                    case 0: // Headset unplugged
                        quaddac_switch.setEnabled(false);
                        disableExtraSettings();
                        break;
                    default: break;
                }
            }
        }
    }

    public IDacHalControl getDhc() { return dhc; }

}
