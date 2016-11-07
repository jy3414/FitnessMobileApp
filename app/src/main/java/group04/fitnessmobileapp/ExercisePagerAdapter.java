package group04.fitnessmobileapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

/**
 * Created by yd on 2016/10/21.
 */
public class ExercisePagerAdapter extends FragmentPagerAdapter {

    private int count = 0;

    public ExercisePagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int i) {
        ExerciseActivityFragment fragment = new ExerciseActivityFragment();
        Bundle args = new Bundle();
        // Our object is just an integer :-P
        args.putInt(fragment.getArgObject(), i + 1);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }
}
