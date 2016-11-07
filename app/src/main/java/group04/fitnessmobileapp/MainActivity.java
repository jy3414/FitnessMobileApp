package group04.fitnessmobileapp;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
   private boolean bluetoothSettled = false;
    private List<Button> exerciseList = new LinkedList<>();
    private ArrayList<CharSequence> exerciseTextList = new ArrayList<>();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onResume() {
        super.onResume();
        ImageView image = (ImageView) findViewById(R.id.body);
        image.setVisibility(View.VISIBLE);

        TextView text = (TextView) findViewById(R.id.instruction);
        text.setVisibility(View.VISIBLE);

        Button button1 = (Button) findViewById(R.id.ex1);
        button1.setVisibility(View.GONE);
        button1.setText("Ex1");
        Button button2 = (Button) findViewById(R.id.ex2);
        button2.setVisibility(View.GONE);
        button2.setText("Ex2");
        Button button3 = (Button) findViewById(R.id.ex3);
        button3.setVisibility(View.GONE);
        button3.setText("Ex3");
        Button button4 = (Button) findViewById(R.id.ex4);
        button4.setVisibility(View.GONE);
        button4.setText("Ex4");
        Button button5 = (Button) findViewById(R.id.ex5);
        button5.setVisibility(View.GONE);
        button5.setText("Ex5");
        Button button6 = (Button) findViewById(R.id.ex6);
        button6.setVisibility(View.GONE);
        button6.setText("Ex6");
        Button button7 = (Button) findViewById(R.id.ex7);
        button7.setVisibility(View.GONE);
        button7.setText("Ex7");
        Button button8 = (Button) findViewById(R.id.ex8);
        button8.setVisibility(View.GONE);
        button8.setText("Ex8");
        Button button9 = (Button) findViewById(R.id.ex9);
        button9.setVisibility(View.GONE);
        button9.setText("Ex9");

        while (!exerciseList.isEmpty()) {
            Button button = exerciseList.remove(0);
            button.setBackgroundColor(Color.rgb(145, 248, 234));
        }

        Button start = (Button) findViewById(R.id.button);
        start.setText("Ready");
    }

    public void showExercises(View view) {

        Button start = (Button) findViewById(R.id.button);


        if (start.getText().equals("Go!")) {
            Intent intent = new Intent(this, ExerciseActivity.class);
            for (int i = 0; i < exerciseList.size(); i++) {
                exerciseTextList.add(exerciseList.get(i).getText());
            }
            intent.putCharSequenceArrayListExtra("exerciseList", exerciseTextList);
            startActivity(intent);
        }

        start.setText("Go!");

        ImageView image = (ImageView) findViewById(R.id.body);
        image.setVisibility(View.GONE);

        TextView text = (TextView) findViewById(R.id.instruction);
        text.setVisibility(View.GONE);


        Button button1 = (Button) findViewById(R.id.ex1);
        button1.setVisibility(View.VISIBLE);
        Button button2 = (Button) findViewById(R.id.ex2);
        button2.setVisibility(View.VISIBLE);
        Button button3 = (Button) findViewById(R.id.ex3);
        button3.setVisibility(View.VISIBLE);
        Button button4 = (Button) findViewById(R.id.ex4);
        button4.setVisibility(View.VISIBLE);
        Button button5 = (Button) findViewById(R.id.ex5);
        button5.setVisibility(View.VISIBLE);
        Button button6 = (Button) findViewById(R.id.ex6);
        button6.setVisibility(View.VISIBLE);
        Button button7 = (Button) findViewById(R.id.ex7);
        button7.setVisibility(View.VISIBLE);
        Button button8 = (Button) findViewById(R.id.ex8);
        button8.setVisibility(View.VISIBLE);
        Button button9 = (Button) findViewById(R.id.ex9);
        button9.setVisibility(View.VISIBLE);
        if (!bluetoothSettled) {
            Intent intent = new Intent(this, BluetoothActivity.class);
            intent.putExtra("From Activity:","Main");
            startActivity(intent);
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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

}
