package org.lineageos.settings.device.dac.utils;

import android.media.AudioSystem;
import android.os.RemoteException;
import android.os.SystemProperties;

import vendor.lge.hardware.audio.dac.control.V1_0.AdvancedFeature;
import vendor.lge.hardware.audio.dac.control.V1_0.HalFeature;
import vendor.lge.hardware.audio.dac.control.V1_0.IDacAdvancedControl;
import vendor.lge.hardware.audio.dac.control.V1_0.IDacHalControl;

public class QuadDAC {

    private static final String TAG = "QuadDAC";

    private QuadDAC() {}

    public static void enable(IDacHalControl dhc, IDacAdvancedControl dac)
    {
        try {
            int digital_filter = getDigitalFilter(dhc);
            int sound_preset = getSoundPreset(dhc);
            int left_balance = getLeftBalance(dhc);
            int right_balance = getRightBalance(dhc);
            int mode = getDACMode(dac);
            int avc_vol = getAVCVolume(dac);
            dhc.setFeatureValue(HalFeature.QuadDAC, 1);
            setDACMode(dac, mode);
            setLeftBalance(dhc, left_balance);
            setRightBalance(dhc, right_balance);
            setDigitalFilter(dhc, digital_filter);
            setSoundPreset(dhc, sound_preset);
            setAVCVolume(dac, avc_vol);
        } catch(Exception e) {}
    }

    public static void disable(IDacHalControl dhc) throws RemoteException
    {
        dhc.setFeatureValue(HalFeature.QuadDAC, 0);
    }

    public static void setDACMode(IDacAdvancedControl dac, int mode) throws RemoteException
    {
        dac.setFeatureValue(AdvancedFeature.HifiMode, mode);
    }

    public static int getDACMode(IDacAdvancedControl dac) throws RemoteException
    {
        return dac.getFeatureValue(AdvancedFeature.HifiMode);
    }

    public static void setAVCVolume(IDacAdvancedControl dac, int avc_volume) throws RemoteException
    {
        dac.setFeatureValue(AdvancedFeature.AVCVolume, avc_volume);
    }

    public static int getAVCVolume(IDacAdvancedControl dac) throws RemoteException
    {
        return dac.getFeatureValue(AdvancedFeature.AVCVolume);
    }

    public static void setDigitalFilter(IDacHalControl dhc, int filter) throws RemoteException
    {
        dhc.setFeatureValue(HalFeature.DigitalFilter, filter);
    }

    public static int getDigitalFilter(IDacHalControl dhc) throws RemoteException
    {
        return dhc.getFeatureValue(HalFeature.DigitalFilter);
    }

    public static void setSoundPreset(IDacHalControl dhc, int preset) throws RemoteException
    {
        dhc.setFeatureValue(HalFeature.SoundPreset, preset);
    }

    public static int getSoundPreset(IDacHalControl dhc) throws RemoteException
    {
        return dhc.getFeatureValue(HalFeature.SoundPreset);
    }

    public static void setLeftBalance(IDacHalControl dhc, int balance) throws RemoteException
    {
        dhc.setFeatureValue(HalFeature.BalanceLeft, balance);
    }

    public static int getLeftBalance(IDacHalControl dhc) throws RemoteException
    {
        return dhc.getFeatureValue(HalFeature.BalanceLeft);
    }

    public static void setRightBalance(IDacHalControl dhc, int balance) throws RemoteException
    {
        dhc.setFeatureValue(HalFeature.BalanceRight, balance);
    }

    public static int getRightBalance(IDacHalControl dhc) throws RemoteException
    {
        return dhc.getFeatureValue(HalFeature.BalanceRight);
    }

    public static boolean isEnabled(IDacHalControl dhc) throws RemoteException
    {
        return dhc.getFeatureValue(HalFeature.QuadDAC) == 1;
    }

}
