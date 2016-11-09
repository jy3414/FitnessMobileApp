package group04.fitnessmobileapp;

import android.content.Intent;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class ExerciseSelectionActivity extends AppCompatActivity {

    private List<Button> exerciseList = new LinkedList<>();
    private ArrayList<CharSequence> exerciseTextList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_exercise_selection);

//        Button start = (Button) findViewById(R.id.readyButton);
//
//        if (start.getText().equals("Go!")) {
//            Intent intent = new Intent(this, ExerciseActivity.class);
//            for (int i = 0; i < exerciseList.size(); i++) {
//                exerciseTextList.add(exerciseList.get(i).getText());
//            }
//            intent.putCharSequenceArrayListExtra("exerciseList", exerciseTextList);
//            startActivity(intent);
//        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        while (!exerciseList.isEmpty()) {
            Button button = exerciseList.remove(0);
            button.setBackgroundColor(Color.rgb(145, 248, 234));
        }

    }

    public void addExerciseToList(View v) {
        Button button = (Button)v;
        if (exerciseList.contains(button)) {
            exerciseList.remove(button);
            button.setBackgroundColor(Color.rgb(145, 248, 234));
            button.setText(button.getText().subSequence(2, button.getText().length()));
            for (int i = 0; i < exerciseList.size(); i++) {
                button = exerciseList.get(i);
                button.setText(exerciseList.indexOf(button) + 1 + "." + button.getText().subSequence(2, button.getText().length()));
            }
        } else {
            exerciseList.add(button);
            button.setText(exerciseList.indexOf(button) + 1 + "." + button.getText());
            button.setBackgroundColor(Color.rgb(241, 244, 66));
        }
    }

    public void addExerciseToTrainingProgram(View v) {
        ArrayList<String> newTrainingProgram = new ArrayList<String>();
        for (int i = 0; i < exerciseList.size(); i++) {
            Button button = exerciseList.get(i);
            newTrainingProgram.add(newTrainingProgram.size(), String.valueOf(button.getText()));
        }
        Intent intent = new Intent(this, TrainingProgramActivity.class);
        intent.putStringArrayListExtra("NewTrainingProgram", newTrainingProgram);
        startActivity(intent);
    }
}