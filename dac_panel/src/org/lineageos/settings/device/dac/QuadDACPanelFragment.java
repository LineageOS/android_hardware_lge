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
import org.lineageos.settings.device.dac.ui.ButtonPreference;
import org.lineageos.settings.device.dac.utils.Constants;
import org.lineageos.settings.device.dac.utils.QuadDAC;

import java.util.ArrayList;

import vendor.lge.hardware.audio.dac.control.V2_0.Feature;
import vendor.lge.hardware.audio.dac.control.V2_0.IDacControl;
import vendor.lge.hardware.audio.dac.control.V2_0.Range;

public class QuadDACPanelFragment extends PreferenceFragment
        implements OnPreferenceChangeListener {

    private static final String TAG = "QuadDACPanelFragment";

    private SwitchPreference quaddac_switch;
    private ListPreference sound_preset_list, digital_filter_list, mode_list;
    private BalancePreference balance_preference;
    private SeekBarPreference avc_volume;

    /*** Custom filter UI props ***/

    /* Shape and symmetry selectors */
    private ListPreference custom_filter_shape, custom_filter_symmetry;

    /* Filter stage 2 coefficients (refer to the kernel's es9218.c for more info) */
    private static SeekBarPreference[] custom_filter_coeffs = new SeekBarPreference[14];

    /* Button to reset custom filter's coefficients, if needed. */
    private ButtonPreference custom_filter_reset_coeffs_button;

    private HeadsetPluggedFragmentReceiver headsetPluggedFragmentReceiver;

    private IDacControl dac;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        headsetPluggedFragmentReceiver = new HeadsetPluggedFragmentReceiver();
        try {
            QuadDAC.initialize();
        } catch(Exception e) {
            Log.d(TAG, "onCreatePreferences: " + e.toString());
        }
        addPreferencesFromResource(R.xml.quaddac_panel);
    }

    public static void setCoeffSummary(int index, int value) {
        custom_filter_coeffs[index].setValue(value);
        custom_filter_coeffs[index].setSummary("Coefficient " + index + " : 0." + value);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {

        try {
            if (preference instanceof SwitchPreference) {

                boolean set_dac_on = (boolean) newValue;

                if (set_dac_on) {
                    QuadDAC.enable();
                    enableExtraSettings();
                    return true;
                } else {
                    QuadDAC.disable();
                    disableExtraSettings();
                    return true;
                }
            }
            if (preference instanceof ListPreference) {
                if (preference.getKey().equals(Constants.HIFI_MODE_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int mode = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setDACMode(mode);
                    return true;

                } else if (preference.getKey().equals(Constants.DIGITAL_FILTER_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int digital_filter = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setDigitalFilter(digital_filter);

                    /* Custom filter panel should only show up with Filter [3] (fourth one) selected */
                    if(QuadDAC.getSupportedFeatures().contains(Feature.CustomFilter) && digital_filter == 3)
                        enableCustomFilter();
                    else
                        disableCustomFilter();

                    return true;

                } else if (preference.getKey().equals(Constants.SOUND_PRESET_KEY)) {
                    ListPreference lp = (ListPreference) preference;

                    int sound_preset = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setSoundPreset(sound_preset);
                    return true;
                } else if(preference.getKey().equals(Constants.CUSTOM_FILTER_SHAPE_KEY))
                {
                    ListPreference lp = (ListPreference) preference;

                    int filter_shape = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setCustomFilterShape(filter_shape);
                    return true;

                } else if(preference.getKey().equals(Constants.CUSTOM_FILTER_SYMMETRY_KEY))
                {
                    ListPreference lp = (ListPreference) preference;

                    int filter_symmetry = lp.findIndexOfValue((String) newValue);
                    QuadDAC.setCustomFilterSymmetry(filter_symmetry);
                    return true;

                }
                return false;
            }

            if (preference instanceof SeekBarPreference) {
                if (preference.getKey().equals(Constants.AVC_VOLUME_KEY)) {
                    if (newValue instanceof Integer) {
                        Integer avc_vol = (Integer) newValue;

                        //avc_volume.setSummary( ((double)avc_vol) + " db");

                        QuadDAC.setAVCVolume(avc_vol);
                        return true;
                    } else {
                        return false;
                    }
                }
                else { /* This assumes the only other seekbars are for the custom filter. Extend as needed. */
                    for(int i = 0; i < 14; i++) {
                        if(preference.getKey().equals(Constants.CUSTOM_FILTER_COEFF_KEYS[i]))
                        {
                            if (newValue instanceof Integer) {
                                Integer coeffVal = (Integer) newValue;

                                setCoeffSummary(i, coeffVal);

                                QuadDAC.setCustomFilterCoeff(i, coeffVal);
                                return true;
                            } else
                                return false;
                        }
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

        custom_filter_shape = (ListPreference) findPreference(Constants.CUSTOM_FILTER_SHAPE_KEY);
        custom_filter_shape.setOnPreferenceChangeListener(this);

        custom_filter_symmetry = (ListPreference) findPreference(Constants.CUSTOM_FILTER_SYMMETRY_KEY);
        custom_filter_symmetry.setOnPreferenceChangeListener(this);

        custom_filter_reset_coeffs_button = (ButtonPreference) findPreference(Constants.RESET_COEFFICIENTS_KEY);
        custom_filter_reset_coeffs_button.setOnPreferenceChangeListener(this);

        for(int i = 0; i < 14; i++)
        {
            custom_filter_coeffs[i] = (SeekBarPreference) findPreference(Constants.CUSTOM_FILTER_COEFF_KEYS[i]);
            custom_filter_coeffs[i].setOnPreferenceChangeListener(this);
        }

        mode_list = (ListPreference) findPreference(Constants.HIFI_MODE_KEY);
        mode_list.setOnPreferenceChangeListener(this);

        avc_volume = (SeekBarPreference) findPreference(Constants.AVC_VOLUME_KEY);
        avc_volume.setOnPreferenceChangeListener(this);

        balance_preference = (BalancePreference) findPreference(Constants.BALANCE_KEY);

        try {
            if (QuadDAC.getSupportedFeatures().contains(Feature.QuadDAC)) {
                quaddac_switch.setVisible(true);
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.SoundPreset)) {
                sound_preset_list.setVisible(true);
                sound_preset_list.setValueIndex(QuadDAC.getSoundPreset());
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.DigitalFilter)) {
                digital_filter_list.setVisible(true);
                digital_filter_list.setValueIndex(QuadDAC.getDigitalFilter());
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.BalanceLeft)
                    && QuadDAC.getSupportedFeatures().contains(Feature.BalanceRight)) {
                balance_preference.setVisible(true);
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.AVCVolume)) {
                avc_volume.setVisible(true);
                Range range = QuadDAC.getSupportedFeatureValues(Feature.AVCVolume).range;
                avc_volume.setMin((int)range.min);
                avc_volume.setMax((int)range.max);
                avc_volume.setValue(QuadDAC.getAVCVolume());
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.HifiMode)) {
                mode_list.setVisible(true);
                mode_list.setValueIndex(QuadDAC.getDACMode());
            }
            if (QuadDAC.getSupportedFeatures().contains(Feature.CustomFilter)) {
                custom_filter_shape.setVisible(true);
                custom_filter_shape.setValueIndex(QuadDAC.getCustomFilterShape());
                custom_filter_symmetry.setVisible(true);
                custom_filter_symmetry.setValueIndex(QuadDAC.getCustomFilterSymmetry());
                for(int i = 0; i < 14; i++)
                {
                    custom_filter_coeffs[i].setVisible(true);
                    custom_filter_coeffs[i].setValue(QuadDAC.getCustomFilterCoeff(i));
                    setCoeffSummary(i, QuadDAC.getCustomFilterCoeff(i));
                }
            }
        } catch(Exception e) {
            Log.d(TAG, "addPreferencesFromResource: " + e.toString());
        }

        try {
            if (am.isWiredHeadsetOn()) {
                quaddac_switch.setEnabled(true);
                if (QuadDAC.isEnabled()) {
                    quaddac_switch.setChecked(true);
                    enableExtraSettings();
                    /* Apply the dac's saved settings right when the panel loads up */
                    QuadDAC.enabledSetup();
                } else {
                    quaddac_switch.setChecked(false);
                    disableExtraSettings();
                }
            } else {
                quaddac_switch.setEnabled(false);
                disableExtraSettings();
                if (QuadDAC.isEnabled()) {
                    quaddac_switch.setChecked(true);
                }
            }
        } catch (Exception e) {
            Log.d(TAG, "addPreferencesFromResource2: " + e.toString());
        }
    }

    private void enableExtraSettings()
    {
        ArrayList<Integer> supportedFeatures = QuadDAC.getSupportedFeatures();
        digital_filter_list.setEnabled(true);
        mode_list.setEnabled(true);
        avc_volume.setEnabled(true);
        balance_preference.setEnabled(true);
        if(supportedFeatures.contains(Feature.SoundPreset))
            sound_preset_list.setEnabled(true);
        if(supportedFeatures.contains(Feature.CustomFilter))
            enableCustomFilter();
    }

    private void disableExtraSettings()
    {
        digital_filter_list.setEnabled(false);
        mode_list.setEnabled(false);
        avc_volume.setEnabled(false);
        balance_preference.setEnabled(false);
        sound_preset_list.setEnabled(false);
        disableCustomFilter();
    }

    private void enableCustomFilter() {
        custom_filter_shape.setEnabled(true);
        custom_filter_symmetry.setEnabled(true);
        for(int i = 0; i < 14; i++){
            custom_filter_coeffs[i].setEnabled(true);
        }

        custom_filter_reset_coeffs_button.setEnabled(true);

        try {
            /* To apply the custom filter's settings */
            QuadDAC.setCustomFilterShape(QuadDAC.getCustomFilterShape());
        } catch (Exception e) {}
    }

    private void disableCustomFilter() {
        custom_filter_shape.setEnabled(false);
        custom_filter_symmetry.setEnabled(false);
        try {
            for(int i = 0; i < 14; i++){
                custom_filter_coeffs[i].setEnabled(false);
            }
        } catch (Exception e) {}

        custom_filter_reset_coeffs_button.setEnabled(false);
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

    public IDacControl getDhc() { return dac; }

}
