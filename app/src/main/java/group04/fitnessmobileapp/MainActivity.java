package group04.fitnessmobileapp;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.PopupMenu;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import static android.R.id.button1;

public class MainActivity extends Activity {

    private List<Button> exerciseList = new LinkedList<>();
    private ArrayList<CharSequence> exerciseTextList = new ArrayList<>();
    private final static String TAG = MainActivity.class.getSimpleName();
    private BluetoothService bluetoothService;
    private BluetoothManager bluetoothManager;
    private BluetoothAdapter bluetoothAdapter;
    private ArrayAdapter<String> bluetoothArrayAdapter;
    private ArrayList<String> bluetoothList = new ArrayList<>();
    private int REQUEST_ENABLE_BT = 2;
    private ListView listView;
    private boolean isConnected = false;
    private final String FIRST_SENSOR = "FITNESS_APP_SENSOR";
    private final String SENSOR_MAC_ADDRESS_1 = "98:4F:EE:0F:C7:9E";
    private final String SENSOR_MAC_ADDRESS_2 = "98:4F:EE:0F:C7:86";
    private final String SENSOR_MAC_ADDRESS_3 = "98:4F:EE:0F:D6:73";
    private final UUID MY_UUID = UUID.fromString("826F63FA-0339-4AA0-BDAF-8588887262E1");
    private android.os.Handler handler;
    private boolean isScanning;
    //private LeDeviceListAdapter deviceListAdapter;
    private static final long SCAN_PERIOD = 10000;

    private boolean bluetoothSettled = false;
    private Button popup_button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        createPopupButton();

        handler = new android.os.Handler();
        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "Bluetooth not supported", Toast.LENGTH_SHORT).show();
            finish();
        }

        bluetoothAdapter = ((BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE)).getAdapter();

        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth not supported", Toast.LENGTH_SHORT).show();
            finish();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }

        bluetoothArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        listView = (ListView) findViewById(R.id.bluetooth_device_list);
        listView.setAdapter(bluetoothArrayAdapter);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        scanLeDevice(false);
        unregisterReceiver(gattUpdateReceiver);

    }

    @Override
    public void onPause() {
        super.onPause();
        scanLeDevice(false);
        //unregisterReceiver(gattUpdateReceiver);

        findViewById(R.id.progress_bar_1).setVisibility(View.GONE);
        findViewById(R.id.progress_bar_2).setVisibility(View.GONE);
        findViewById(R.id.progress_bar_3).setVisibility(View.GONE);
    }

    private void scanLeDevice(final boolean enable) {
        if (enable) {
            // Stops scanning after a pre-defined scan period.
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    isScanning = false;
                    bluetoothAdapter.stopLeScan(mLeScanCallback);
                    invalidateOptionsMenu();
                }
            }, SCAN_PERIOD);

            isScanning = true;
            bluetoothAdapter.startLeScan(mLeScanCallback);
        } else {
            isScanning = false;
            bluetoothAdapter.stopLeScan(mLeScanCallback);
        }
        invalidateOptionsMenu();
    }

    private BluetoothAdapter.LeScanCallback mLeScanCallback = new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (device.getAddress().equals(SENSOR_MAC_ADDRESS_3)) {
                        Log.d("==================", "found once");
                        if (isScanning) {
                            bluetoothAdapter.stopLeScan(mLeScanCallback);
                            isScanning = false;
                            findViewById(R.id.progress_bar_3).setVisibility(View.GONE);
                            findViewById(R.id.sensor3).setVisibility(View.GONE);

                            findViewById(R.id.button).setVisibility(View.GONE);
                            findViewById(R.id.startButton).setVisibility(View.VISIBLE);

                            Intent gattServiceIntent = new Intent(getApplicationContext(), BluetoothService.class);
                            bindService(gattServiceIntent, serviceConnection, BIND_AUTO_CREATE);

                            registerReceiver(gattUpdateReceiver, makeGattUpdateIntentFilter());
                            if (bluetoothService != null) {
                                final boolean result = bluetoothService.connect(SENSOR_MAC_ADDRESS_3);
                                Log.d(TAG, "Connect request result=" + result);
                            }
                        }
                    }
                    bluetoothArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                    bluetoothArrayAdapter.notifyDataSetChanged();
                    //deviceListAdapter.addDevice(device);
                    //deviceListAdapter.notifyDataSetChanged();
                }
            });
        }
    };

    public void searchSensor(View v) {

        scanLeDevice(true);

        findViewById(R.id.progress_bar_1).setVisibility(View.VISIBLE);
        findViewById(R.id.progress_bar_2).setVisibility(View.VISIBLE);
        findViewById(R.id.progress_bar_3).setVisibility(View.VISIBLE);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode == RESULT_OK) {
                Toast.makeText(getApplicationContext(), "Bluetooth turned on", Toast.LENGTH_LONG).show();
            } else {
                Toast.makeText(getApplicationContext(), "Bluetooth is not turned on", Toast.LENGTH_LONG).show();
            }
        }
    }

    private final ServiceConnection serviceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            bluetoothService = ((BluetoothService.LocalBinder) service).getService();
            if (!bluetoothService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            bluetoothService.connect(SENSOR_MAC_ADDRESS_3);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            bluetoothService = null;
        }
    };

    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (bluetoothService.ACTION_GATT_CONNECTED.equals(action)) {
                Log.d(TAG, "GATT connected");
                isConnected = true;
                //updateConnectionState(R.string.connected);
                //invalidateOptionsMenu();
            } else if (BluetoothService.ACTION_GATT_DISCONNECTED.equals(action)) {
                Log.d(TAG, "GATT disconnected");
                isConnected = false;
                //updateConnectionState(R.string.disconnected);
                //invalidateOptionsMenu();
                //clearUI();
            } else if (BluetoothService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                Log.d(TAG, "GATT services discovered");
                // Show all the supported services and characteristics on the user interface.
                //displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothService.ACTION_DATA_AVAILABLE.equals(action)) {
                Log.d(TAG, "GATT data available");
                TextView text = (TextView) findViewById(R.id.data);
                text.setText(intent.getStringExtra(BluetoothService.EXTRA_DATA));
                //displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    public void readData(View v) {
        if (bluetoothService != null) {
            bluetoothService.readCustomCharacteristic();
        }
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothService.ACTION_DATA_AVAILABLE);
        return intentFilter;
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

    private void createPopupButton() {
        popup_button = (Button) findViewById(R.id.popupButton);
        popup_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Creating the instance of PopupMenu
                PopupMenu popup = new PopupMenu(MainActivity.this, popup_button);
                //Inflating the Popup using xml file
                popup.getMenuInflater()
                        .inflate(R.menu.menu_popup, popup.getMenu());
                popup.show();
                //registering popup with OnMenuItemClickListener
                popup.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    public boolean onMenuItemClick(MenuItem item) {
                        if (item.getTitle().equals("Profile")) {
                            showProfile();
                        } else {
                            showSetting();
                        }
                        return true;
                    }
                });

                popup.show(); //showing popup menu
            }
        }); //closing the setOnClickListener method
    }

    public void showProfile() {

        Intent intent = new Intent(this, ProfileActivity.class);
        startActivity(intent);

    }

    public void showSetting() {

        Intent intent = new Intent(this, SettingActivity.class);
        startActivity(intent);

    }

    public void showTrainingProgram(View v) {

        Intent intent = new Intent(this, TrainingProgramActivity.class);
        startActivity(intent);

    }

}
