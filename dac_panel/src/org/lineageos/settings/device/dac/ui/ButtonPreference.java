package org.lineageos.settings.device.dac.ui;


import android.content.Context;
import android.os.SystemProperties;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;
import android.widget.TextView;

import org.lineageos.settings.device.dac.R;
import org.lineageos.settings.device.dac.utils.Constants;
import org.lineageos.settings.device.dac.utils.QuadDAC;
import org.lineageos.settings.device.dac.QuadDACPanelFragment;

public class ButtonPreference extends Preference {

    private static final String TAG = "ButtonPreference";

    private Button reset_button;

    public ButtonPreference(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        setLayoutResource(R.layout.button_preference);
    }

    public ButtonPreference(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, defStyleAttr);
    }

    public ButtonPreference(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public ButtonPreference(Context context) {
        this(context, null);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);
        holder.itemView.setClickable(false);

        reset_button = (Button) holder.findViewById(R.id.reset_coefficients);
        reset_button.setClickable(true);

        reset_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resetCoefficients();
            }
        });
    }

    private void resetCoefficients()
    {
        try {
            QuadDAC.resetCustomFilterCoeffs();
        } catch (Exception e) {}
        for(int i = 0; i < 14; i++) {
            QuadDACPanelFragment.setCoeffSummary(i, 0);
        }
    }
}

