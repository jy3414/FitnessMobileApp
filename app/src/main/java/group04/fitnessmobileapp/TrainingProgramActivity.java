package group04.fitnessmobileapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TrainingProgramActivity extends AppCompatActivity {

    private ListView listView;
    private ArrayAdapter<String> adapter;
    private List<String> allTrainingPrograms;
    private String[] preDefined = {"Apple", "Banana", "Cat"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_training_program);

        listView = (ListView) findViewById(R.id.trainingProgramListView);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
                showExercise(arg1);
            }
        });

        allTrainingPrograms = new ArrayList<String>();
        Collections.addAll(allTrainingPrograms, preDefined);
        adapter = new ArrayAdapter<String>(listView.getContext(),
                android.R.layout.simple_list_item_1, allTrainingPrograms);
        listView.setAdapter(adapter);
    }

    @Override
    protected void onResume() {
        super.onResume();

        ArrayList<String> trainingProgram =
                getIntent().getStringArrayListExtra("NewTrainingProgram");
        if ((trainingProgram != null) && (trainingProgram.size() != 0)) {
            addTrainingProgram(trainingProgram);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        preDefined = new String[allTrainingPrograms.size()];
        preDefined = allTrainingPrograms.toArray(preDefined);

    }

    public void showExerciseSelection(View v) {

        Intent intent = new Intent(this, ExerciseSelectionActivity.class);
        startActivity(intent);

    }

    public void showExercise(View v) {

        Intent intent = new Intent(this, ExerciseActivity.class);
        startActivity(intent);

    }

    public void addTrainingProgram(ArrayList<String> trainingProgram) {
        String newTrainingProgram = "";
        for (String string : trainingProgram) {
            newTrainingProgram += string + "   ";
        }
        allTrainingPrograms.add(newTrainingProgram);
        adapter.notifyDataSetChanged();
    }
}
