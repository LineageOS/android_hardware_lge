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

    public static void enable()
    {
        try {
            int digital_filter = getDigitalFilter();
            int left_balance = getLeftBalance();
            int right_balance = getRightBalance();
            int mode = getDACMode();
            int avc_vol = getAVCVolume();
            dac.setHifiDacState(true);
            setDACMode(mode);
            setLeftBalance(left_balance);
            setRightBalance(right_balance);
            setDigitalFilter(digital_filter);
            setAVCVolume(avc_vol);

            // Sound presets are disabled on the open-source audio HAL.
            if(dac_features.contains(Feature.SoundPreset)) {
                int sound_preset = getSoundPreset();
                setSoundPreset(sound_preset);
            }
        } catch(Exception e) {}
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

}
