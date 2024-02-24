package org.lineageos.settings.device.dac.utils;

import android.media.AudioSystem;
import android.os.RemoteException;
import android.os.SystemProperties;

import java.util.ArrayList;

import vendor.lge.hardware.audio.dac.control.V2_0.Feature;
import vendor.lge.hardware.audio.dac.control.V2_0.FeatureStates;
import vendor.lge.hardware.audio.dac.control.V2_0.IDacControl;

public class QuadDAC {

    private static final String TAG = "QuadDAC";

    private QuadDAC() {}

    private static IDacControl dac;

    private static ArrayList<Integer> dac_features;

    public static boolean dac_service_available = false;

    public static void initialize() throws RemoteException {
        dac = IDacControl.getService(true);
        dac_features = dac.getSupportedFeatures();
        dac_service_available = true;
    }

    public static void enable() throws RemoteException
    {
        try {
            dac.setHifiDacState(true);

            int mode = getDACMode();
            int left_balance = getLeftBalance();
            int right_balance = getRightBalance();
            int avc_vol = getAVCVolume();
            int digital_filter = getDigitalFilter();
            setDACMode(mode);
            setLeftBalance(left_balance);
            setRightBalance(right_balance);
            setAVCVolume(avc_vol);
            setDigitalFilter(digital_filter);

            // Sound presets are disabled on the open-source audio HAL.
            if(dac_features.contains(Feature.SoundPreset)) {
                int sound_preset = getSoundPreset();
                setSoundPreset(sound_preset);
            }

            // Kernel-side implementation needed for custom filters
            if(dac_features.contains(Feature.CustomFilter)) {
                int custom_filter_shape = getCustomFilterShape();
                int custom_filter_symmetry = getCustomFilterSymmetry();
                int[] custom_filter_coefficients = new int[14];
                int i;
                for(i = 0; i < 14; i++) {
                    custom_filter_coefficients[i] = getCustomFilterCoeff(i);
                }
                setCustomFilterShape(custom_filter_shape);
                setCustomFilterSymmetry(custom_filter_symmetry);
                for(i = 0; i < 14; i++) {
                    setCustomFilterCoeff(i, custom_filter_coefficients[i]);
                }
            }
        } catch(Exception e) {
            Log.d(TAG, "QuadDAC: enable: " + e.toString());
        }
    }

    public static void disable() throws RemoteException
    {
        dac.setHifiDacState(false);
    }

    public static ArrayList<Integer> getSupportedFeatures() {
        return dac_features;
    }

    public static FeatureStates getSupportedFeatureValues(int feature) throws RemoteException
    {
        return dac.getSupportedFeatureValues(feature);
    }

    public static void setDACMode(int mode) throws RemoteException
    {
        dac.setFeatureValue(Feature.HifiMode, mode);
    }

    public static int getDACMode() throws RemoteException
    {
        return dac.getFeatureValue(Feature.HifiMode);
    }

    public static void setAVCVolume(int avc_volume) throws RemoteException
    {
        dac.setFeatureValue(Feature.AVCVolume, avc_volume);
    }

    public static int getAVCVolume() throws RemoteException
    {
        return dac.getFeatureValue(Feature.AVCVolume);
    }

    public static void setDigitalFilter(int filter) throws RemoteException
    {
        dac.setFeatureValue(Feature.DigitalFilter, filter);
        if(dac_features.contains(Feature.CustomFilter) && filter == 3) {/* Custom filter */
            /*
            * If it's a custom filter, we need to apply its settings. Any of the functions
            * below should suffice since it'll load all settings from memory by parsing its
            * data.
            */
            setCustomFilterShape(getCustomFilterShape());
        }
    }

    public static int getDigitalFilter() throws RemoteException
    {
        return dac.getFeatureValue(Feature.DigitalFilter);
    }

    public static void setSoundPreset(int preset) throws RemoteException
    {
        if(dac_features.contains(Feature.SoundPreset))
            dac.setFeatureValue(Feature.SoundPreset, preset);
    }

    public static int getSoundPreset() throws RemoteException
    {
        if(!dac_features.contains(Feature.SoundPreset))
            return 0;
        return dac.getFeatureValue(Feature.SoundPreset);
    }

    public static void setLeftBalance(int balance) throws RemoteException
    {
        dac.setFeatureValue(Feature.BalanceLeft, balance);
    }

    public static int getLeftBalance() throws RemoteException
    {
        return dac.getFeatureValue(Feature.BalanceLeft);
    }

    public static void setRightBalance(int balance) throws RemoteException
    {
        dac.setFeatureValue(Feature.BalanceRight, balance);
    }

    public static int getRightBalance() throws RemoteException
    {
        return dac.getFeatureValue(Feature.BalanceRight);
    }

    public static boolean isEnabled() throws RemoteException
    {
        return dac.getHifiDacState();
    }

    public static boolean setCustomFilterShape(int shape) throws RemoteException
    {
        return dac.setCustomFilterShape(shape);
    }

    public static int getCustomFilterShape() throws RemoteException
    {
        return dac.getCustomFilterShape();
    }

    public static boolean setCustomFilterSymmetry(int symmetry) throws RemoteException
    {
        return dac.setCustomFilterSymmetry(symmetry);
    }

    public static int getCustomFilterSymmetry() throws RemoteException
    {
        return dac.getCustomFilterSymmetry();
    }

    public static boolean setCustomFilterCoeff(int coeffIndex, int coeff_val) throws RemoteException
    {
        return dac.setCustomFilterCoeff(coeffIndex, coeff_val);
    }

    public static int getCustomFilterCoeff(int coeffIndex) throws RemoteException
    {
        return dac.getCustomFilterCoeff(coeffIndex);
    }

    public static void resetCustomFilterCoeffs() throws RemoteException
    {
        dac.resetCustomFilterCoeffs();
    }
}
