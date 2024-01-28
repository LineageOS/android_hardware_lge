package org.lineageos.settings.device.dac.ui;


import android.content.Context;
import android.filterfw.geometry.Quad;
import android.os.SystemProperties;
import android.os.health.HealthKeys;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragment;
import androidx.preference.PreferenceViewHolder;
import android.widget.TextView;
import android.widget.Toast;

import org.lineageos.settings.device.dac.R;
import org.lineageos.settings.device.dac.utils.Constants;
import org.lineageos.settings.device.dac.utils.QuadDAC;

import java.util.Map;

import vendor.lge.hardware.audio.dac.control.V2_0.FeatureStates;
import vendor.lge.hardware.audio.dac.control.V2_0.Feature;
import vendor.lge.hardware.audio.dac.control.V2_0.IDacControl;

public class BalancePreference extends Preference {

    private static final String TAG = "BalancePreference";

    private int left_balance = 0;
    private int right_balance = 0;

    private int max_allowed_value = 0;
    private int min_allowed_value = -12;

    private Button bt_left_plus, bt_left_minus, bt_right_plus, bt_right_minus;
    private TextView tv_left, tv_right;

    private IDacControl dac;

    public BalancePreference(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        setLayoutResource(R.layout.balance_preference);
    }

    public BalancePreference(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, defStyleAttr);
    }

    public BalancePreference(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public BalancePreference(Context context) {
        this(context, null);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);
        holder.itemView.setClickable(false);

        bt_left_plus = (Button) holder.findViewById(R.id.bt_left_plus);
        bt_left_minus = (Button) holder.findViewById(R.id.bt_left_minus);

        bt_right_plus = (Button) holder.findViewById(R.id.bt_right_plus);
        bt_right_minus = (Button) holder.findViewById(R.id.bt_right_minus);

        tv_left = (TextView) holder.findViewById(R.id.tv_left_val);
        tv_right = (TextView) holder.findViewById(R.id.tv_right_val);

        bt_left_plus.setClickable(true);
        bt_left_minus.setClickable(true);
        bt_right_plus.setClickable(true);
        bt_right_minus.setClickable(true);

        bt_left_plus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateLeftBalance(true);
            }
        });

        bt_left_minus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateLeftBalance(false);
            }
        });

        bt_right_plus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateRightBalance(true);
            }
        });

        bt_right_minus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateRightBalance(false);
            }
        });

        loadBalanceConfiguration();
    }

    private void loadBalanceConfiguration()
    {
        try {
            FeatureStates states = dac.getSupportedFeatureValues(Feature.BalanceLeft);
            min_allowed_value = (int)states.range.min;
            max_allowed_value = (int)states.range.max;

            left_balance = -QuadDAC.getLeftBalance(dac);
            right_balance = -QuadDAC.getRightBalance(dac);
        } catch(Exception e) {
            Log.d(TAG, "loadBalanceConfiguration: " + e.toString());
        }

        tv_left.setText(-((double)left_balance)/2 + " db");
        tv_right.setText(-((double)right_balance)/2 + " db");

        if(left_balance == max_allowed_value)
        {
            bt_left_plus.setEnabled(false);
        } else if(left_balance == min_allowed_value)
        {
            bt_left_minus.setEnabled(false);
        } else
        {
            bt_left_plus.setEnabled(true);
            bt_left_minus.setEnabled(true);
        }

        if(right_balance == max_allowed_value)
        {
            bt_right_plus.setEnabled(false);
        } else if(right_balance == min_allowed_value)
        {
            bt_right_minus.setEnabled(false);
        } else
        {
            bt_right_plus.setEnabled(true);
            bt_right_minus.setEnabled(true);
        }
    }

    private void updateLeftBalance(boolean increase)
    {
        StringBuilder sb = new StringBuilder();
        if(increase)
        {
            if(left_balance < max_allowed_value)
            {
                left_balance += 1;
                bt_left_minus.setEnabled(true);
                if(left_balance == max_allowed_value)
                {
                    bt_left_plus.setEnabled(false);
                }
            }

        } else {
            if(left_balance > min_allowed_value)
            {
                left_balance -= 1;
                bt_left_plus.setEnabled(true);
                if(left_balance == min_allowed_value)
                {
                    bt_left_minus.setEnabled(false);
                }
            }
        }
        try {
            QuadDAC.setLeftBalance(dac, -left_balance);
        } catch(Exception e) {
            Log.d(TAG, "updateLeftBalance: " + e.toString());
        }
        sb.append(((double)left_balance)/2);
        sb.append(" db");
        tv_left.setText(sb.toString());
    }

    private void updateRightBalance(boolean increase)
    {
        StringBuilder sb = new StringBuilder();
        if(increase)
        {
            if(right_balance < max_allowed_value)
            {
                right_balance += 1;
                bt_right_minus.setEnabled(true);
                if(right_balance == max_allowed_value)
                {
                    bt_right_plus.setEnabled(false);
                }
            }

        } else {
            if(right_balance > min_allowed_value)
            {
                right_balance -= 1;
                bt_right_plus.setEnabled(true);
                if(right_balance == min_allowed_value)
                {
                    bt_right_minus.setEnabled(false);
                }
            }
        }
        try {
            QuadDAC.setRightBalance(dac, -right_balance);
        } catch(Exception e) {
            Log.d(TAG, "updateRightBalance: " + e.toString());
        }
        sb.append(((double)right_balance)/2);
        sb.append(" db");
        tv_right.setText(sb.toString());
    }

    public void setDhc(IDacControl idac) {
        this.dac = idac;
    }
}
