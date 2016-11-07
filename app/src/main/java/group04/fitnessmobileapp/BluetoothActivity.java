package group04.fitnessmobileapp;

import android.annotation.TargetApi;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;


public class BluetoothActivity extends AppCompatActivity {
    private List<BluetoothDevice> deviceList = new ArrayList<>();
    private ListView deviceListView;
    private ArrayList<String> mDeviceList = new ArrayList<>();
    private ArrayAdapter<String> arrayAdapter;
    private BluetoothAdapter mBluetoothAdapter;
    int REQUEST_ENABLE_BT = 2;

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mDeviceList);

        deviceListView = (ListView) findViewById(R.id.bluetooth_device_list_view);
        deviceListView.setAdapter(arrayAdapter);

        deviceListView.setOnItemClickListener(new ListView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                //TODO
            }

        });

        final BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            Toast.makeText(getApplicationContext(), "Bluetooth turned on", Toast.LENGTH_LONG).show();
        } else {
            Toast.makeText(getApplicationContext(), "Bluetooth is already turned on", Toast.LENGTH_LONG).show();
        }


    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_BT) {
            ((TextView) findViewById(R.id.warning_messgae)).setVisibility(View.VISIBLE);
            ((TextView) findViewById(R.id.notification_bluetooth_chest_text)).setVisibility(View.GONE);
            ((TextView) findViewById(R.id.select_bluetooth_device)).setVisibility(View.GONE);
            ((TextView) findViewById(R.id.scan_bluetooth_device)).setVisibility(View.GONE);
        }
    }

    final BroadcastReceiver bReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device == null) {
                    Toast.makeText(getApplicationContext(), "device is null", Toast.LENGTH_LONG).show();
                } else {
                    deviceList.add(device);
                    arrayAdapter.add(device.getName());
                    arrayAdapter.notifyDataSetChanged();
                }

            }
        }
    };


    public void scanLeDevice(View v) {
        if (mBluetoothAdapter.isDiscovering()) {
            Toast.makeText(getApplicationContext(), "Finding sensor", Toast.LENGTH_LONG).show();
        } else {
            arrayAdapter.clear();
            deviceList.clear();
            mBluetoothAdapter.startDiscovery();
            registerReceiver(bReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        unregisterReceiver(bReceiver);
    }
}