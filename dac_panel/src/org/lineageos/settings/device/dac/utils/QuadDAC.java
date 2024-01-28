package org.lineageos.settings.device.dac.utils;

import android.media.AudioSystem;
import android.os.RemoteException;
import android.os.SystemProperties;

import vendor.lge.hardware.audio.dac.control.V2_0.Feature;
import vendor.lge.hardware.audio.dac.control.V2_0.IDacControl;

public class QuadDAC {

    private static final String TAG = "QuadDAC";

    private QuadDAC() {}

    public static void enable(IDacControl dac)
    {
        try {
            int digital_filter = getDigitalFilter(dac);
            int sound_preset = getSoundPreset(dac);
            int left_balance = getLeftBalance(dac);
            int right_balance = getRightBalance(dac);
            int mode = getDACMode(dac);
            int avc_vol = getAVCVolume(dac);
            dac.setFeatureValue(Feature.QuadDAC, 1);
            setDACMode(dac, mode);
            setLeftBalance(dac, left_balance);
            setRightBalance(dac, right_balance);
            setDigitalFilter(dac, digital_filter);
            setSoundPreset(dac, sound_preset);
            setAVCVolume(dac, avc_vol);
        } catch(Exception e) {}
    }

    public static void disable(IDacControl dac) throws RemoteException
    {
        dac.setFeatureValue(Feature.QuadDAC, 0);
    }

    public static void setDACMode(IDacControl dac, int mode) throws RemoteException
    {
        dac.setFeatureValue(Feature.HifiMode, mode);
    }

    public static int getDACMode(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.HifiMode);
    }

    public static void setAVCVolume(IDacControl dac, int avc_volume) throws RemoteException
    {
        dac.setFeatureValue(Feature.AVCVolume, avc_volume);
    }

    public static int getAVCVolume(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.AVCVolume);
    }

    public static void setDigitalFilter(IDacControl dac, int filter) throws RemoteException
    {
        dac.setFeatureValue(Feature.DigitalFilter, filter);
    }

    public static int getDigitalFilter(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.DigitalFilter);
    }

    public static void setSoundPreset(IDacControl dac, int preset) throws RemoteException
    {
        dac.setFeatureValue(Feature.SoundPreset, preset);
    }

    public static int getSoundPreset(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.SoundPreset);
    }

    public static void setLeftBalance(IDacControl dac, int balance) throws RemoteException
    {
        dac.setFeatureValue(Feature.BalanceLeft, balance);
    }

    public static int getLeftBalance(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.BalanceLeft);
    }

    public static void setRightBalance(IDacControl dac, int balance) throws RemoteException
    {
        dac.setFeatureValue(Feature.BalanceRight, balance);
    }

    public static int getRightBalance(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.BalanceRight);
    }

    public static boolean isEnabled(IDacControl dac) throws RemoteException
    {
        return dac.getFeatureValue(Feature.QuadDAC) == 1;
    }

}
