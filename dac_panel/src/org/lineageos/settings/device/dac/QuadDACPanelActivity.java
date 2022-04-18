package org.lineageos.settings.device.dac;

import android.os.Bundle;
import com.android.settingslib.collapsingtoolbar.CollapsingToolbarBaseActivity;
import com.android.settingslib.collapsingtoolbar.R;

import androidx.appcompat.app.AppCompatActivity;

public class QuadDACPanelActivity extends CollapsingToolbarBaseActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getFragmentManager().beginTransaction().replace(R.id.content_frame,
                new QuadDACPanelFragment()).commit();
    }

}
