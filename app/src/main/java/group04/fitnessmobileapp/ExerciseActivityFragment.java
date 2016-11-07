package group04.fitnessmobileapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * Created by yd on 2016/10/21.
 */
public class ExerciseActivityFragment extends Fragment {

    public static final String ARG_OBJECT = "object";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // The last two arguments ensure LayoutParams are inflated
        // properly.
        View rootView = inflater.inflate(R.layout.fragment_exercise, container, false);
        Bundle args = getArguments();
        return rootView;
    }

    public String getArgObject() {
        return ARG_OBJECT;
    }
}
